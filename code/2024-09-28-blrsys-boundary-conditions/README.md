This folder includes code for the talk I gave at [BlrSys #3](https://hasgeek.com/bengalurusystemsmeetup/bangalore-systems-meetup-3/). Also see the corresponding [discussion thread around it](https://github.com/amodm/talks/discussions/1).

## Compile
To compile, you need to have `make` and `clang` installed. Run `make all` to compile all programs, which are then available under `build` folder.

## Structure
The code is largely in C, but fairly straightforward to understand. It consists of the following files:
* [stack-heap.c](stack-heap.c) - shows that from a memory POV, there's no difference between stack or heap, and the advantage of stack comes only because of its frequency of use as a function call frame storage, enabling memory caching to kick in.
* [memory-access.c](memory-access.c) - demonstrates memory-access times, and how it is influenced by cache behaviour.
* [superscalar.c](superscalar.c) - demonstrates superscalar behaviour.
* [bp.c](bp.c) - demonstrates branch prediction.
* [common.h](common.h) - basic common functions used across other files.
