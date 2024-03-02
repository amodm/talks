# Inside the GPU
This lecture is part of a [4-lecture series](..) covering hardware/software interaction. This one covers GPU architecture & programming models for GPUs. We explore what makes a GPU distinct from a CPU at a hardware level, is it even different at all, and why they excel in areas where they do. During this process, we try to imagine GPU programming models should be like, and in what ways our imagination vs reality agree & disagree. Throughout all of this, we use WebGPU as a way to explore the software API side of things.

## Pre-read
* Which is faster - CPU or GPU? Try to do whatever research you can on the subject.
* Look up the specs of a few GPUs. Which specs do you think are more important in characterising the expected performance?
* Think about how should the performance of an internal GPU (the ones that are part of the CPU die/package) compare with an external discrete one.
* Try to read up about WebGPU, whatever you can. It's ok if you don't understand it fully, or even at all, we'll cover some basics of it during our session anyways.

## Assignments
* Try removing the `if` guard inside the kernel, and see what happens. What's your reasoning about the behaviour?
* Implement a kernel logic that uses __bad__ spatial locality within the same workgroup.
* Puzzle: Why is CPU time relatively so high for Num-Add-Mul, but GPU time not so?
* Extend the CPU vs GPU html to implement basic matrix multiplication as a new test (assuming row major representation).
* Can merge sort can be considered as parallelisable? Why, or why not? If yes, try implementing and compare with CPU based mergesort.
* Why should GPU memory need to be mapped before copy to CPU?
