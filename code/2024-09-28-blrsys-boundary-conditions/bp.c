/**
 * @file bp.c
 * @brief Code to demonstrate branch prediction in CPU.
 * @author Amod Malviya
 *
 * @details
 * Branch Prediction (stemming from control flow logic) is a crucial aspect of CPU
 * performance. It is the capability of a processor to guess the direction of a branch
 * and thus allows the backend to speculatively execute the instructions for the taken
 * branch. This is done to avoid the pipeline stall that would occur if the processor
 * had to wait for the branch to be resolved.
 *
 * @section usage Usage
 * ./build/bp
 *
 * @section env Environment Variables
 * - ARR_LEN: Length of the array to be used. Default is to have enough integers to
 *     fill a page.
 * - THRESHOLD: Threshold value to use for the comparison (branch). Default is 0.
 * - TIMES: Number of times to run the comparison. Default is 100000.
 */

#include "common.h"

/**
 * Main entry point
 */
int main(int UNUSED(argc), const char* UNUSED(argv[])) {
    // initialize the array via mmap
    const int ARR_LEN = get_env_int("ARR_LEN", getpagesize() / sizeof(int));
    int *arr = mmap(NULL, ARR_LEN * sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // fill with random integers, each between 0 and RAND_MAX
    fill_random(arr, ARR_LEN);

    const int mid = RAND_MAX / 2;
    const int threshold = get_env_int("THRESHOLD", 0);

    // we compare two scenarios:
    // 1. unpredictable: by comparing with RAND_MAX / 2, we
    //    make it 50-50 whether the branch is taken or not.
    // 2. user-chosen: by comparing with a user-chosen threshold,
    //    we can control the degree of predictability, e.g. 0
    //    or RAND_MAX will make it fully predictable.
    COMPARE_TWO_N_TIMES(
        "branch-prediction",
        "unpredictable",
        ({
            int count = 0;
            for (int i = 0; i < ARR_LEN; i++) {
                if (arr[i] < mid) {
                    count++;
                }
            }
            count;
        }),
        "user-chosen",
        ({
            int count = 0;
            for (int i = 0; i < ARR_LEN; i++) {
                if (arr[i] < threshold) {
                    count++;
                }
            }
            count;
        }),
        get_env_long("TIMES", 100000)
    );

    // cleanup and exit
    munmap(arr, ARR_LEN * sizeof(int));
    return 0;
}
