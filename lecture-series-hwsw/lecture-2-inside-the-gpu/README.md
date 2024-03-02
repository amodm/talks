# Inside the GPU
This lecture is part of a [4-lecture series](..) covering hardware/software interaction. This one covers GPU architecture & programming models for GPUs. We explore what makes a GPU distinct from a CPU at a hardware level, is it even different at all, and why they excel in areas where they do. During this process, we try to imagine GPU programming models should be like, and in what ways our imagination vs reality agree & disagree. Throughout all of this, we use WebGPU as a way to explore the software API side of things.

## Pre-read
* Which is faster - CPU or GPU? Try to do whatever research you can on the subject.
* Look up the specs of a few GPUs. Which specs do you think are more important in characterising the expected performance?
* Think about how should the performance of an internal GPU (the ones that are part of the CPU die/package) compare with an external discrete one.
* Try to read up about WebGPU, whatever you can. It's ok if you don't understand it fully, or even at all, we'll cover some basics of it during our session anyways.

## Files
* [vec-multiply.html](vec-multiply.html) contains an end to end structure of a basic WebGPU program (focussed on computation, not rendering). You can use this to make a copy and quickly try out a new kernel.
* [cpu-vs-gpu.html](cpu-vs-gpu.html) contains a set of test cases, and has slightly more complicated code to account for multiple kernels. Some of the assignments require you to go run these tests.

## Assignments
* Try removing the `if` guard inside the kernel, and see what happens. What's your reasoning about the behaviour?
* Implement a kernel logic that uses __bad__ spatial locality within the same workgroup. If you manage to do so in a way that demonstrates usefulness of spatial locality in kernels, raise a PR which contains both -ve (bad spatial locality) and +ve (good spatial locality), as tests under [cpu-vs-gpu.html](cpu-vs-gpu.html). I'll continue accepting PRs so long as the logic is different than existing ones.
* Puzzle: Why is CPU time relatively so high for Num-Add-Mul, but GPU time not so?
* Extend the [cpu-vs-gpu.html](cpu-vs-gpu.html) to implement basic matrix multiplication as a new test (assuming row major representation). Raise a PR if it's working fine (I'll accept the first one, and from there on only improvements to the previous one).
* Can merge sort can be considered as parallelisable? Why, or why not? If yes, try implementing and compare with CPU based mergesort. Raise a PR as a test under [cpu-vs-gpu.html](cpu-vs-gpu.html).
* Why should GPU memory need to be mapped before copy to CPU?
