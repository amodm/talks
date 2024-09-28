/**
 * @file common.h
 * @brief Common functions for the benchmarks.
 * @author Amod Malviya
 */

#ifndef COMMON_H
#define COMMON_H

#ifdef __linux__
#define _DEFAULT_SOURCE
#include <sys/mman.h>
#include <stdint.h>

#undef MIN
#define MIN(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})

#undef MAX
#define MAX(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#ifdef __APPLE__
#include <sys/sysctl.h>
#endif

/**
 * Prints the message and exits the program.
 */
_Noreturn void die(const char *msg) {
    fprintf(stderr, "error: %s\n", msg);
    exit(1);
}

/**
 * Assuming that we need to die because of a critical syscall failure,
 * this function prints the error message and exits the program.
 */
_Noreturn void die_perror(const char *msg) {
    perror(msg);
    exit(1);
}

/**
 * Fills the array with random integers.
 */
void fill_random(int *arr, int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = rand();
    }
}

/**
 * Fills the array with random 64-bit integers.
 */
void fill_rand64(uint64_t *arr, size_t n) {
    srand(time(NULL));
    for (size_t i = 0; i < n; i++) {
        arr[i] = ((uint64_t)rand()) << 32 | rand();
    }
}

/**
 * Returns the difference between two timespecs in microseconds.
 */
inline unsigned long ts_diff_us(struct timespec *start, struct timespec *end) {
    return (end->tv_sec - start->tv_sec) * 1000000l + (end->tv_nsec - start->tv_nsec) / 1000l;
}

/**
 * Returns the sum of the array.
 */
inline int sum_array(int *arr, int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum;
}

/**
 * Returns the value of the environment variable by `name` as an integer.
 */
inline int get_env_int(const char *name, int default_val) {
    const char *env_val = getenv(name);
    return env_val != NULL ? atoi(env_val) : default_val;
}

/**
 * Returns the value of the environment variable by `name` as a long.
 */
inline long get_env_long(const char *name, long default_val) {
    const char *env_val = getenv(name);
    return env_val != NULL ? atol(env_val) : default_val;
}

typedef struct {
    uint64_t bytes;
    size_t sz_abbr;
    const char *suffix;
} Bytes;

const char *BYTE_SUFFIXES[] = {"", "Ki", "Mi", "Gi", "Ti", "Pi", "Ei"};

/**
 * Converts the size to a human-readable format.
 */
Bytes bytes(size_t size) {
    int i = 0;
    size_t sz_abbr = size;
    while (sz_abbr >= 1024 && i < 6) {
        sz_abbr = sz_abbr >> 10;
        i++;
    }
    return (Bytes){size, sz_abbr, BYTE_SUFFIXES[i]};
}

/*
 * Executes `body` n times and prints the time taken to execute it.
 */
#define CLOCK_N_TIMES(n, msg, body) ({ \
    struct timespec start, end; \
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start); \
    long count = n; \
    int _sum = 0; \
    while (count--) { \
        _sum += body; \
    } \
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end); \
    long time_us = ts_diff_us(&start, &end); \
    if (msg != NULL) { \
        printf("%s: %9ld us [for x%ld] %s\n", (const char*) msg, time_us, count, _sum > 0 ? "" : " "); \
    } \
    time_us -= _sum > 0 ? 1 : 0; \
    time_us; \
})

/*
 * Executes `body` n/16 times to warm up the cache and then executes it
 * n times.
 */
#define WARMUP_AND_CLOCK_N_TIMES(n, msg, body) ({ \
    CLOCK_N_TIMES((MAX(1, n >> 4)), NULL, body); \
    CLOCK_N_TIMES((n), msg, body); \
})

/*
 * Same as WARMUP_AND_CLOCK_N_TIMES but determines the `times`
 * from the `TIMES` environment variable.
 */
#define WARMUP_AND_CLOCK_ENV_TIMES(msg, body) ({ \
    long times = get_env_long("TIMES", 1000l); \
    body; \
    CLOCK_N_TIMES(times, msg, body); \
})

/**
 * Compares the time taken by two bodies n times and prints the result.
 */
#define COMPARE_TWO_N_TIMES(msg, id1, body1, id2, body2, n_times) { \
    long times = (n_times); \
    long us1 = (WARMUP_AND_CLOCK_N_TIMES(times, NULL, body1)); \
    long us2 = (WARMUP_AND_CLOCK_N_TIMES(times, NULL, body2)); \
    if (msg != NULL) { \
        if (us1 < us2) { \
            int faster_by = (us2 - us1) * 100 / us1; \
            printf("%s: %s is faster than %s by %3d%% (%ld vs %ld us)\n", msg, id1, id2, faster_by, us1, us2); \
        } else { \
            int faster_by = (us1 - us2) * 100 / us2; \
            printf("%s: %s is faster than %s by %3d%% (%ld vs %ld us)\n", msg, id2, id1, faster_by, us2, us1); \
        } \
    } \
}

/**
 * Compares the time taken by two bodies and prints the result. The
 * number of times to run the comparison is determined by the `TIMES`
 * environment variable.
 */
#define COMPARE_TWO(msg, id1, body1, id2, body2) { \
    long _env_times = get_env_long("TIMES", 1000l); \
    COMPARE_TWO_N_TIMES(msg, id1, body1, id2, body2, _env_times); \
}

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

#endif // COMMON_H
