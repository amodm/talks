/**
 * @file memory-access.c
 * @brief Code to demonstrate memory access patterns.
 * @author Amod Malviya
 *
 * @details
 * Memory access patterns are crucial for the performance of a program. The CPU
 * makes things a lot easier by using caches to store frequently accessed data,
 * but the performance can still be affected by the access pattern. This code
 * demonstrates the impact of different memory access patterns on the performance
 * of a program.
 *
 * @section usage Usage
 * ./build/memory-access [stride]
 *
 * @section env Environment Variables
 * - ARR_LEN: Length of the array to be used. Default is 1 << 28 (i.e. 256 MiB).
 * - TIMES: Number of times to run the comparison. Default is 1000.
 */

#include "common.h"

/**
 * Shuffles the array randomly in place.
 *
 * @param arr The array to fill & walk
 * @param count Number of elements in the array
 */
void shuffle(int *arr, size_t count) {
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

/**
 * Tests memory access (ns/iter and bandwidth).
 *
 * We do this by filling `arr` with indices to itself. We then do a walk
 * across those indices. If the indices were set consecutively, the walk
 * represents a consecutive memory access, but if the indices were set
 * randomly, we're doing random memory access.
 *
 * @param arr The array to fill & walk
 * @param count Number of elements in the array
 * @param stride If set to 0, leads to random walk, else walk is gapped
 *   by `stride` elements.
 */
void test_mem_access(int *arr, const int count, int stride) {
    struct timespec start, end;

    if (stride % 8 != 0) {
        die("error: stride must be a multiple of 8");
    }

    // fill the array with indices
    const int s = stride == 0 ? 1 : (stride >> 3);
    for (int i = 0; i < count; i++) {
        arr[i] = (i + s) % count;
    }
    if (stride == 0) {
        shuffle(arr, count);
    }

    // do profiled run
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    register int idx = 0;
    for (int i = 0; i < count; i++) {
        idx = arr[idx];
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    long time_us = ts_diff_us(&start, &end);
    long bandwidth = (long)(count) * sizeof(int) * 1000 * 1000 / time_us;
    double ns_per_iter = (double)time_us * 1000 / count;

    const Bytes count_b = bytes(count);
    const Bytes bw_b = bytes(bandwidth);
    printf("%.1f ns/iter (%ld%sB/s), when accessing %ld%sB with stride=%d %s\n", ns_per_iter, bw_b.sz_abbr, bw_b.suffix, count_b.sz_abbr, count_b.suffix, stride, idx ? "" : " ");
}

/**
 * The main entry point.
 */
int main(int argc, const char* argv[]) {
    // ensure that array len remains bounded to reasonable limits
    // 128Mi <= arr_len <= 1Gi
    size_t count = get_env_long("ARR_LEN", 1 << 28);
    if (count < (1 << 27)) {
        count = 1 << 27;
    } else if (count > (1 << 30)) {
        count = 1 << 30;
    }

    // allocate enough memory
    int *arr = mmap(NULL, count * sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (arr == MAP_FAILED) {
        die_perror("mmap");
    }

    // run test
    const long stride = argc > 1 ? atol(argv[1]) : 8;
    test_mem_access(arr, count, stride > 0 ? stride : 0);

    // clean up & exit
    munmap(arr, count * sizeof(int));
    return 0;
}
