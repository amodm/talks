/**
 * @file stack-heap.c
 * @brief Demonstrates that there's no real difference between stack and heap, assuming both are cached.
 * @author Amod Malviya
 *
 * @section usage Usage
 * ./build/stack-heap
 *
 * @section env Environment Variables
 * - ARR_LEN: Length of the array to be used. Default is 1 << 12 (i.e. 4 KiB).
 * - TIMES: Number of times to run the comparison. Default is 1000.
 */

#include "common.h"

/**
 * Main entry point of the program.
 */
int main(int UNUSED(argc), char const* UNUSED(_argv[])) {
    // get the length of the array
    const int len = get_env_int("ARR_LEN", getpagesize() / sizeof(int));
    char msg[128];

    // allocate on stack
    int *stack_arr = alloca(len * sizeof(int));

    // allocate on heap. We're using mmap() instead of malloc(), because
    // we're testing access speed, not allocation speed.
    int *heap_arr = mmap(NULL, len * sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // fill both with random ints
    fill_random(stack_arr, len);
    fill_random(heap_arr, len);

    // run test
    sprintf(msg, "stack-vs-heap (size=%d)", len);
    COMPARE_TWO(
        msg,
        "stack", sum_array(stack_arr, len),
        "heap", sum_array(heap_arr, len)
    );

    // clean up & exit
    munmap(heap_arr, len * sizeof(int));
    return 0;
}
