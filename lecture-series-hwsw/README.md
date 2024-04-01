# Lecture Series - Hardware/Software
This is a series of 4 lectures I've created for SST (Scaler School of Technology), that brings attention to how hardware functions underneath all the software we write.

## Design
The series is designed to give a breadth first view, with only minor seasoning of details. It's not designed as a full curriculum, but as an entry point which students can use to then figure out which areas to explore further.

Each lecture has a set of pre-reads that act as a primer, allowing students to enter the class with curiosity and at least some basic knowledge of the facts. The pre-reads are not designed to be _test oriented_, so students should approach it without prejudice. Entering the class without the pre-reads will hurt the student's ability to fully absorb the class.

Each lecture also has a set of assignments, designed to explore some of the concepts at their own time, and in their own way. Most assignments would be useful to do in a group based setting. The assignments should be explored not as a _test_, but as _thinking material_.

## List of lectures
* 2024-02-24 [Inside the Machine](lecture-1-inside-the-machine) - this lecture covers basic computer architecture, designed to bring out principles that are useful in hardware & software. In terms of hardware, we cover CPU, Memory, PCIe, Storage. In terms of concepts, we cover Amdahl's Law, Latency, and how Throughput can be used to hide latency.
* 2024-03-02 [Inside the GPU](lecture-2-inside-the-gpu) - here we explore GPU architecture & programming models for GPUs. We explore what makes a GPU distinct from a CPU at a hardware level, is it even different at all, and why they excel in areas where they do. During this process, we try to imagine GPU programming models should be like, and in what ways our imagination vs reality agree & disagree. Throughout all of this, we use WebGPU as a way to explore the software API side of things.
* 2024-03-16 [The VMs around us](lecture-3-the-vms-around-us) - this one covers the design & architecture of Virtual Machines. We start with trying to describe the key components of a VM, and apply that understanding to do a survey of existing VMs, and take a peek into their design decisions.
* 2024-04-01 [Programming Constructs](lecture-4-programming-constructs) - here we explore under the hood details of selected programming constructs, such as variables, functions, classes, try/catch etc. We try to imagine how they might be implemented at a hardware level, and validate our understanding with actuals. In the process, we develop a deeper understanding of these constructs in programming languages.

