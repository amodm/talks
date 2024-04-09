# The VMs around us
This lecture is part of a [4-lecture series](..) covering hardware/software interaction. This one covers the design & architecture of Virtual Machines. We start with trying to describe the key components of a VM, and apply that understanding to do a survey of existing VMs, and take a peek into their design decisions.

[![Link to video](https://img.youtube.com/vi/9srvmoItiZ0/0.jpg)](https://youtu.be/9srvmoItiZ0)

## Pre-read
* Browse through the [JVM's spec](https://docs.oracle.com/javase/specs/jvms/se8/html/). Feel free to take a look around whatever you feel like, but definitely give [Chapter 2](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-2.html) a read (2.5, 2.6 & 2.11 being the most important). As always, you don't have to understand everything (or even anything) - but it'll help prime your mind for our class.
* Java & Kotlin are two prominent languages (amongst others) that run on top of JVM - proof that JVM is a bit disjointed from the Java language itself. Is it possible to create a non-garbage-collected language on the JVM?
* ⁠Is JVM's bytecode the same as a "machine language"? If yes, why even have it, instead of just reusing the x86 machine language? We'll try to answer this question during our class, but would be useful if you can try to think of your reasons first.
* ⁠What are some other VMs you might know of?
* ⁠For all the pre-reads, going through them as a group (of 2-4) is highly encouraged.

## Files
* [brainfuck](brainfuck) contains a simple VM implementation where the ISA is the 8 instruction-set of [Brainfuck](https://en.wikipedia.org/wiki/Brainfuck).
* [JavaIR.java](JavaIR.java) contains a super simple Java code to take a peek into the compiled output.

## Assignments
* Implement addition of 2 numbers for Brainfuck. You can use the provided python script to test.
* Implement multiply for Brainfuck.
* Think about as many differences as you can find between stack vs register based machines. Don’t just read them, do try to “understand” them.
* Figure out how the JVM interacts with the “external world”, e.g. how is System.io.println implemented?
* Discuss: if we were to do AOT compilation of Java bytecode, are we not essentially creating a native binary? If yes, in such a situation, what role does GC play, and who's doing GC, if _java_ is not needed anymore (as we have a native binary)?
* You write code for Android in Java (often, but not always). Questions emerge:
    - Does it mean that Android is running on top of a JVM? Or is it that something like a 'java' program runs your class files?
    - If the answer to the above isn't java, is it a VM? If the answer is yes, in what ways is this VM different than JVM? If no, what mechanism is letting bytecode run on the processor directly?
    - Doing research on the above will likely take you down some history lane. Instead of just passively absorbing the history, do reflect on why the engineers made the design decisions at any point in time, and try to do secondary research on whether your guess was true or not.
