# libmongocrypt_stresstest

Stress test libmongocrypt to search for bugs.

## Build

```bash
make \
    CFLAGS='-I/path/to/libmongocrypt/include' \
    LDFLAGS='-L/path/to/libmongocrypt/lib/'
```

## Example

```bash
$ ./libmongocrypt_stresstest -p /home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: INFO: Opened CSFLE dynamic library [/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so]
libmongocrypt_stresstest: mongocrypt_init failed
AddressSanitizer:DEADLYSIGNAL
=================================================================
==1362551==ERROR: AddressSanitizer: SEGV on unknown address 0x608037800057 (pc 0x7f462c7ae551 bp 0x7f462eef39d0 sp 0x7f462eef3978 T12)
==1362551==The signal is caused by a READ memory access.
    #0 0x7f462c7ae550 in std::_Function_handler<void (mongo::ServiceContext*), mongo::(anonymous namespace)::disableAuthMechanismsRegisterer::{lambda(mongo::ServiceContext*)#1}>::_M_invoke(std::_Any_data const&, mongo::ServiceContext*&&) (/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so+0x2fae550)
    #1 0x7f462c9c1ca8 in mongo::ServiceContext::make() (/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so+0x31c1ca8)
    #2 0x7f462a9e3fde in mongo_crypt_v1_lib_create (/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so+0x11e3fde)
    #3 0x7f46384685e4 in _csfle_replace_or_take_validate_singleton (/usr/local/lib/libmongocrypt.so.0+0x685e4)
    #4 0x7f4638468bbf in _try_enable_csfle (/usr/local/lib/libmongocrypt.so.0+0x68bbf)
    #5 0x7f4638468eb6 in mongocrypt_init (/usr/local/lib/libmongocrypt.so.0+0x68eb6)
    #6 0x5559ce3edceb in main._omp_fn.0 /home/kyle-mongo/src/debug-crypt-error/main.c:172
    #7 0x7f46394b778d  (/lib/x86_64-linux-gnu/libgomp.so.1+0x1a78d)
    #8 0x7f46394e7608 in start_thread /build/glibc-SzIz7B/glibc-2.31/nptl/pthread_create.c:477
    #9 0x7f463832d132 in __clone (/lib/x86_64-linux-gnu/libc.so.6+0x11f132)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV (/home/kyle-mongo/crypt_shared/6.3.0/lib/mongo_crypt_v1.so+0x2fae550) in std::_Function_handler<void (mongo::ServiceContext*), mongo::(anonymous namespace)::disableAuthMechanismsRegisterer::{lambda(mongo::ServiceContext*)#1}>::_M_invoke(std::_Any_data const&, mongo::ServiceContext*&&)
Thread T12 created by T0 here:
    #0 0x7f4638a3a815 in __interceptor_pthread_create ../../../../src/libsanitizer/asan/asan_interceptors.cc:208
    #1 0x7f46394b7dea  (/lib/x86_64-linux-gnu/libgomp.so.1+0x1adea)
    #2 0x7f46394af8e0 in GOMP_parallel (/lib/x86_64-linux-gnu/libgomp.so.1+0x128e0)
    #3 0x7f4638253f24 in __GI_getenv /build/glibc-SzIz7B/glibc-2.31/stdlib/getenv.c:35

==1362551==ABORTING
```

## Usage

```
$ ./libmongocrypt_stresstest -h
libmongocrypt_stresstest: stress test libmongocrypt

usage:
libmongocrypt_stresstest [OPTIONS]

OPTIONS
	-h	display this help message

	-j	number of parallel jobs

	-n	total number of iterations (default 100)

	-p	path to the crypt shared dynamic library

```
