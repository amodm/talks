# Programming Constructs
This lecture is part of a [4-lecture series](..) covering hardware/software interaction. In this specific lecture, we explore under the hood details of selected programming constructs, such as variables, functions, classes, try/catch etc. We try to imagine how they might be implemented at a hardware level, and validate our understanding with actuals. In the process, we develop a deeper understanding of these constructs in programming languages.

[![Link to video](https://img.youtube.com/vi/B4wex3APO0Q/0.jpg)](https://youtu.be/B4wex3APO0Q)

## Pre-read
* Which language is faster? C? Java? Rust? Go? Is speed a "language" thing?
* ⁠Go through [this article](https://www.scaler.com/topics/c/memory-layout-in-c/) to understand memory layouts of running programs. You don't have to understand the C part, focus on just the high level picture & understanding.
* ⁠Stack vs Heap - is one faster than the other? If yes, which one?
* ⁠Other than infinite recursion, are there other ways to encounter a stack overflow? What's really happening behind the scene?
* ⁠For all the pre-reads, going through them as a group (of 2-4) is highly encouraged.

## Files
* [memlens.cpp](memlens.cpp) - the main program we use to explore internals of programs. See [Compilation](#compilation) step.
* [memlens.hpp](memlens.hpp) - header file for memlens.
* [memlens-linux.hpp](memlens-linux.hpp) - header file for linux specific implementation pieces. Not required to be understood for this lecture.
* [memlens-macos.hpp](memlens-macos.hpp) - header file for macos specific implementation pieces. Not required to be understood for this lecture.
* [memlens-windows.hpp](memlens-windows.hpp) - header file for windows specific implementation pieces. Not required to be understood for this lecture.

## Compilation
* On Linux/macOS: `make memlens`
* On Windows (in Visual Studio cmd prompt): `cl /EHsc memlens.cpp`

## Assignments
* Is JVM a Harvard Architecture? Use lecture3 (slide 18) to refresh the JVM's memory model.
* Figure out how non-primitive data types are returned - via registers? via stack? If the latter, where is the allocation made (caller or callee stack)?
* Figure out the `$default` mechanism in Kotlin for handling default parameter values
* Given what you know about functions, discuss how lambdas (anonymous functions) get implemented.
* `async/await` is an increasingly popular programming construct which allows one to "schedule" blocks of logic (code). Knowing what you know about `code`, `stack` and `heap`, how would you think about the underlying implementation of such a construct.
* Research why address of `code` (also called `text`) section doesn't usually start from address 0. While researching, be thoughtful to critically assess the explanation, instead of just blindly accepting it.
