/* Custom user settings file written for Atmel Port by wolfSSL */

#ifndef WOLFSSL_USER_SETTINGS_H
#define WOLFSSL_USER_SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------- */
/* Platform */
/* ------------------------------------------------------------------------- */
#undef  WOLFSSL_ATMEL
#define WOLFSSL_ATMEL

#undef  WOLFSSL_ATECC508A
#define WOLFSSL_ATECC508A

#undef  WOLFCRYPT_ONLY
//#define WOLFCRYPT_ONLY

#undef  WOLFSSL_GENERAL_ALIGNMENT
#define WOLFSSL_GENERAL_ALIGNMENT   4

#undef  SINGLE_THREADED
#define SINGLE_THREADED

#undef  WOLFSSL_SMALL_STACK
#define WOLFSSL_SMALL_STACK


/* ------------------------------------------------------------------------- */
/* Network */
/* ------------------------------------------------------------------------- */
#undef  WOLFSSL_USER_IO
#define WOLFSSL_USER_IO


/* ------------------------------------------------------------------------- */
/* Math Configuration */
/* ------------------------------------------------------------------------- */
#undef  USE_FAST_MATH
#define USE_FAST_MATH

#undef  TFM_TIMING_RESISTANT
#define TFM_TIMING_RESISTANT


/* ------------------------------------------------------------------------- */
/* Enable Features */
/* ------------------------------------------------------------------------- */
#undef  KEEP_PEER_CERT
#define KEEP_PEER_CERT


/* ------------------------------------------------------------------------- */
/* Crypto */
/* ------------------------------------------------------------------------- */
/* ECC */
#if 1
    #undef  HAVE_ECC
    #define HAVE_ECC

    /* Custom Curve Config */
    #undef  ECC_USER_CURVES
    #define ECC_USER_CURVES

    #undef  ALT_ECC_SIZE
    #define ALT_ECC_SIZE
#endif

/* RSA */
#undef NO_RSA
#if 0
    #undef  FP_MAX_BITS
    #define FP_MAX_BITS     4096
#else
    #define NO_RSA
#endif


/* ------------------------------------------------------------------------- */
/* Benchmark / Test */
/* ------------------------------------------------------------------------- */
/* Use reduced benchmark / test sizes */
#undef  BENCH_EMBEDDED
#define BENCH_EMBEDDED

#undef  USE_CERT_BUFFERS_2048
#define USE_CERT_BUFFERS_2048

#undef  NO_CRYPT_TEST
//#define NO_CRYPT_TEST

#undef  NO_CRYPT_BENCHMARK
//#define NO_CRYPT_BENCHMARK


/* ------------------------------------------------------------------------- */
/* Debugging */
/* ------------------------------------------------------------------------- */
/* To debug: Enable the WOLFSSL_DEBUG and disable NO_ERROR_STRINGS */
#undef  WOLFSSL_DEBUG
//#define WOLFSSL_DEBUG

#undef  NO_ERROR_STRINGS
#define NO_ERROR_STRINGS

/* Use this to measure / print heap usage (comment out NO_WOLFSSL_MEMORY) */
#undef  USE_WOLFSSL_MEMORY
//#define USE_WOLFSSL_MEMORY
#undef  WOLFSSL_TRACK_MEMORY
//#define WOLFSSL_TRACK_MEMORY
#undef  NO_WOLFSSL_MEMORY
#define NO_WOLFSSL_MEMORY


/* ------------------------------------------------------------------------- */
/* RNG */
/* ------------------------------------------------------------------------- */
/* Use HW RNG Only */
#if 1
    /* Override P-RNG with HW RNG */
    #undef  CUSTOM_RAND_GENERATE_BLOCK
    #define CUSTOM_RAND_GENERATE_BLOCK  atmel_get_random_block
	extern int atmel_get_random_block(unsigned char* output, unsigned int sz);

/* Use P-RNG + HW RNG (adds 8K) */
#else
    /* Use built-in P-RNG (SHA256 based) with HW RNG */
    #undef  HAVE_HASHDRBG
    #define HAVE_HASHDRBG
#endif


/* ------------------------------------------------------------------------- */
/* Disable Features */
/* ------------------------------------------------------------------------- */
#undef  NO_FILESYSTEM
#define NO_FILESYSTEM

#undef  NO_WRITEV
#define NO_WRITEV

#undef  NO_MAIN_DRIVER
#define NO_MAIN_DRIVER

#undef  NO_DSA
#define NO_DSA

#undef  NO_DH
#define NO_DH

#undef  NO_DES3
#define NO_DES3

#undef  NO_RC4
#define NO_RC4

#undef  NO_OLD_TLS
#define NO_OLD_TLS

#undef  NO_HC128
#define NO_HC128

#undef  NO_RABBIT
#define NO_RABBIT

#undef  NO_PSK
#define NO_PSK

#undef  NO_MD4
#define NO_MD4

#undef  NO_PWDBASED
#define NO_PWDBASED


/* ------------------------------------------------------------------------- */
/* Port */
/* ------------------------------------------------------------------------- */
/* Include the ATECC508A Port (this include needs to be at bottom) */
//#include <wolfssl/wolfcrypt/port/atmel/atmel.h>

/* Override Current Time */
/* Allows custom_time() function to be defined in port layer */
#define WOLFSSL_USER_CURRTIME


#ifdef __cplusplus
}
#endif

#endif /* WOLFSSL_USER_SETTINGS_H */
