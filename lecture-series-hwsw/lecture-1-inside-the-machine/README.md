# Inside the Machine
This lecture is part of a [4-lecture series](..) covering hardware/software interaction. This one covers basic computer architecture, designed to bring out principles that are useful in hardware & software. In terms of hardware, we cover CPU, Memory, PCIe, Storage. In terms of concepts, we cover Amdahl's Law, Latency, and how Throughput can be used to hide latency.

## Pre-read
* Look at how Intel describes the specs of a typical CPU, say [Core i9 1300](https://ark.intel.com/content/www/us/en/ark/products/230499/intel-core-i9-13900-processor-36m-cache-up-to-5-60-ghz.html). Think about which factors you would consider as more important than the others. How does your current CPU rank on those factors? Compare notes with friends.
* Compare the Core i9 1300 above (which is a consumer class CPU) with server processors (say [Xeon 6530](https://ark.intel.com/content/www/us/en/ark/products/237249/intel-xeon-gold-6530-processor-160m-cache-2-1-ghz.html)). What are the differences? Why do you think those differences exist?
* Discuss with friends: what is faster - a CPU or a GPU?
* What kind of permanent storage exists in your laptop? How do you characterise it? Compare notes with friends.
* You need to calculate the sum of all bytes of the content in a file. Would it be faster if the file was compressed, or if it was uncompressed? Should the answer vary? Can you write a program to test your hypotheses?
* When Apple first launched its M1 series of processors, there was a lot of buzz about it being significantly faster than some of the best CPUs out there. Search online as much as you can about it, read through differing analysis about the reasons, and try to make up your own mind. Discuss with friends, particularly if they have a different opinion.

## Assignments
* In Amdahl's Law slide, if the webserver was 40-60 (40% cpu, 60% I/O), what amount of CPU improvement will lead to 80% of max gain possible?
* Find two examples where the code is written in a way that optimises for cache lines. Avoid googling directly. Be smart - try thinking about which areas should benefit the most from such optimisations, and then try to search for that topic along with _"cache line"_.
* We discussed that memory exposes a synchronous interface, i.e. the hardware doesn't tell the kernel that it's awaiting memory. Reflect on why that should be the case. What's wrong with CPU telling the kernel _"hey, I'm awaiting data from memory, so you can do some other stuff"_.
* Complete the `7+10` scenario in the food stall latency slide.
* Find 3 examples of latency hiding via throughput (except the ones we already discussed).
* Analogous to the food stall scenario, imagine writing an HTTP server. Start from the crudest possible design (take request -> process -> take next request), and evolve the design to see how much latency you can hide via throughput. You don't have to write code, just think about the design.
* How would you think about bandwidth needed to drive a 8k monitor @60Hz (assume 8b each for RGB)? Compare your calculated bandwidth with the spec of the cable you use to connect. How many lanes of PCIe 4.0 would be needed to pump that throughput?
* Go through [this puzzle](https://twitter.com/amodm/status/1469522129670397952). Knowing what you know now about memory latencies and size of cache lines, how would you think about this? FYI: don't answer on Twitter, as I'm no more active on it.
* Lookup a few mobile processors' datasheets, compare architecture, and write an essay/blog.
* Figure out latency vs bandwidth vs throughput of items marked in red on slide 20.

