# Lecture Series - Hardware/Software
This is a series of 4 lectures I've created for SST (Scaler School of Technology), that brings attention to how hardware functions underneath all the software we write.

## Design
The series is designed to give a breadth first view, with only minor seasoning of details. It's not designed as a full curriculum, but as an entry point which students can use to then figure out which areas to explore further.

Each lecture has a set of pre-reads that act as a primer, allowing students to enter the class with curiosity and at least some basic knowledge of the facts. The pre-reads are not designed to be _test oriented_, so students should approach it without prejudice. Entering the class without the pre-reads will hurt the student's ability to fully absorb the class.

Each lecture also has a set of assignments, designed to explore some of the concepts at their own time, and in their own way. Most assignments would be useful to do in a group based setting. The assignments should be explored not as a _test_, but as _thinking material_.

## List of lectures
* 2024-02-24 [Inside the Machine](lecture-1-inside-the-machine) - this lecture covers basic computer architecture, designed to bring out principles that are useful in hardware & software. In terms of hardware, we cover CPU, Memory, PCIe, Storage. In terms of concepts, we cover Amdahl's Law, Latency, and how Throughput can be used to hide latency.
* TBD **Inside the GPU/NPU** - here we explore what makes a GPU distinct from a CPU at a hardware level, is it even different at all, and why they excel in areas where they do. During this process, we try to imagine GPU programming models should be like, and in what ways our imagination vs reality agree & disagree.
* TBD **The VMs around us** - the tech world is filled with virtual machines (e.g. JVM), and it isn't always obvious - the ways in which they're useful. Here we explore a few of these VMs, and try to do a survey of their characteristics. In the process, we develop some intuitions about what's needed to make a VM.
* TBD **Popular Programming Constructs** - here we explore __under the hood__ details of selected programming constructs, try to imagine how they might be implemented at a hardware level, and validate our understanding with actuals. In the process, we develop a deeper understanding of the constructs in programming languages.

