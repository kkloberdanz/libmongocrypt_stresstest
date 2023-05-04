#include <errno.h>
#include <getopt.h>
#include <omp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <mongocrypt/mongocrypt.h>

#define UNUSED(A) (void)(A)

const char *const PROGRAM_NAME = "libmongocrypt_stresstest";

static void print_usage(void) {
    fprintf(
        stderr,
        "%s: stress test libmongocrypt\n\n"
        "usage:\n"
        "%s [OPTIONS]\n\n"
        "OPTIONS\n"
        "\t-h\tdisplay this help message\n\n"
        "\t-j\tnumber of parallel jobs\n\n"
        "\t-n\ttotal number of iterations (default 100)\n\n"
        "\t-p\tpath to the crypt shared dynamic library\n\n",
        PROGRAM_NAME,
        PROGRAM_NAME
    );
}

static void log_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "%s: ", PROGRAM_NAME);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

static long long parse_ll(const char *const str) {
    errno = 0;
    long long num = 0;
    char *endptr = NULL;
    num = strtoll(str, &endptr, 10);
    if (errno == ERANGE) {
        log_error("argument: '%s' is out of range for a long long", str);
        print_usage();
        exit(EXIT_FAILURE);
    } else if (*endptr != '\0') {
        log_error("argument: '%s' is not a valid integer", str);
        print_usage();
        exit(EXIT_FAILURE);
    }
    return num;
}

struct opts {
    const char *path;
    long long num_iterations;
    long long num_threads;
};

static void parse_opts(
    int argc,
    char *const *const argv,
    struct opts *opts /* out */
) {
    int c;
    opts->path = NULL;
    opts->num_iterations = 100;
    opts->num_threads = -1;
    while ((c = getopt(argc, argv, "hj:n:p:")) != -1) {
        switch (c) {
        case 'h':
            print_usage();
            exit(EXIT_SUCCESS);
        case 'n':
            opts->num_iterations = parse_ll(optarg);
            if (opts->num_iterations < 0) {
                log_error("number of iterations must be at least 0");
                exit(EXIT_FAILURE);
            }
            break;
        case 'j':
            opts->num_threads = parse_ll(optarg);
            if (opts->num_threads <= 0) {
                log_error("number of threads must be at least 1");
                exit(EXIT_FAILURE);
            }
            break;
        case 'p':
            opts->path = optarg;
            break;
        default:
            print_usage();
            exit(EXIT_FAILURE);
        }
    }
}

static void _log_to_stderr(
    mongocrypt_log_level_t level,
    const char *message,
    uint32_t message_len,
    void *ctx
) {
    UNUSED(message_len);
    UNUSED(ctx);

    #pragma omp critical
    {
        switch (level) {
        case MONGOCRYPT_LOG_LEVEL_FATAL:
            log_error("FATAL: %s", message);
            break;
        case MONGOCRYPT_LOG_LEVEL_ERROR:
            log_error("ERROR: %s", message);
            break;
        case MONGOCRYPT_LOG_LEVEL_WARNING:
            log_error("WARNING: %s", message);
            break;
        case MONGOCRYPT_LOG_LEVEL_INFO:
            // log_error("INFO: %s", message);
            break;
        case MONGOCRYPT_LOG_LEVEL_TRACE:
            log_error("TRACE: %s", message);
            break;
        default:
            log_error("UNKNOWN: %s", message);
            break;
        }
    }
}

int main(int argc, char **argv) {
    struct opts opts;
    parse_opts(argc, argv, &opts);

    if (opts.num_threads > 0) {
        omp_set_num_threads(opts.num_threads);
    }

    #pragma omp parallel for
    for (long long i = 0; i < opts.num_iterations; i++) {
        mongocrypt_t *crypt = NULL;

        crypt = mongocrypt_new();
        if (!crypt) {
            #pragma omp critical
            {
                log_error("could not create a crypt object");
                exit(EXIT_FAILURE);
            }
        }

        if (!mongocrypt_setopt_kms_provider_aws(crypt, "example", -1, "example", -1)) {
            #pragma omp critical
            {
                log_error("could not set kms provider");
                exit(EXIT_FAILURE);
            }
        }

        if (!mongocrypt_setopt_log_handler(crypt, _log_to_stderr, NULL)) {
            #pragma omp critical
            {
                log_error("could not set log handler");
                exit(EXIT_FAILURE);
            }
        }

        if (opts.path) {
            mongocrypt_setopt_set_crypt_shared_lib_path_override(crypt, opts.path);
        }

        if (!mongocrypt_init(crypt)) {
            #pragma omp critical
            {
                log_error("mongocrypt_init failed");
                if (errno) {
                    perror("errno");
                }
                exit(EXIT_FAILURE);
            }
        }

        mongocrypt_destroy(crypt);
    }
    log_error("SUCCESS");
    return 0;
}
