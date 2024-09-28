/**
 * @file superscalar.c
 * @brief Demonstrates the impact of superscalar execution on performance.
 * @author Amod Malviya
 *
 * @details
 * Superscalar execution is a CPU architecture feature that allows the processor
 * to execute multiple instructions in parallel. This code demonstrates the impact
 * of superscalar execution on performance.
 *
 * @section usage Usage
 * ./build/superscalar
 *
 * @section env Environment Variables
 * - TIMES: Number of times to run the comparison. Default is 1000.
 */

#include "common.h"

/**
 * Main entry point of the program.
 */
int main(int UNUSED(argc), char const* UNUSED(argv[])) {
    // we initialise array len to fit a memory page
    const int ARR_LEN = getpagesize() / sizeof(int);
    int *arr = mmap(NULL, ARR_LEN * sizeof(int), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    fill_random(arr, ARR_LEN);

    // initialise random ints
    int x1 = rand();
    int x2 = rand();
    int x3 = rand();
    int x4 = rand();
    int x5 = rand();
    int x6 = rand();
    int x7 = rand();

    // we demonstrate superscalar execution by comparing the following two
    // approaches:
    // 1. not optimised: in this, we create data hazards by ensuring that
    //    one instr depends on the result of the prev instruction, thus
    //    creating pipeline stalls.
    // 2. optimised: in this, we do the exact same logic, but without any
    //    data hazards. Ideally, this should turn out to be faster on a
    //    superscalar CPU.
    COMPARE_TWO(
        "superscalar",
        "not optimised",
        ({
            int last = 0;
            for (int i = 0; i < ARR_LEN; i+=8) {
                last = arr[i];
                arr[i] += last & 0xc9fc291d;
                arr[i+1] += arr[i] & 0x26ee3af5;
                arr[i+2] -= arr[i+1] & 0x6f209a8d;
                arr[i+3] += arr[i+2] & 0xa81eb73f;
                arr[i+4] -= arr[i+3] & 0x7e5a1e1c;
                arr[i+5] += arr[i+4] & 0xe110b8bb;
                arr[i+6] -= arr[i+5] & 0x2bc0dcbf;
                arr[i+7] += arr[i+6] & 0xfb3bc4fc;
            }
            last;
        }),
        "optimised",
        ({
            int last = 0;
            for (int i = 0; i < ARR_LEN; i+=8) {
                last = arr[i];
                arr[i] += last & 0xc9fc291d;
                arr[i+1] += x1 & 0x26ee3af5;
                arr[i+2] -= x2 & 0x6f209a8d;
                arr[i+3] += x3 & 0xa81eb73f;
                arr[i+4] -= x4 & 0x7e5a1e1c;
                arr[i+5] += x5 & 0xe110b8bb;
                arr[i+6] -= x6 & 0x2bc0dcbf;
                arr[i+7] += x7 & 0xfb3bc4fc;
            }
            last;
        })
    );

    // clean up & exit
    munmap(arr, ARR_LEN * sizeof(int));
    return 0;
}
