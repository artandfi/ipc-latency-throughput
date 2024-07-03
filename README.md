# IPC Latency, Throughput & Capacity
This is a repo for my implementation of a lab work on "Modern Operating Systems" course at Taras Shevchenko National University of Kyiv on the Faculty of Computer Science and Cybernetics at Computer Science Master's.

The goal is to measure latency, throughput & capacity of different IPC media, such as:
- mmap, both in ``SHARED`` and ``ANONYMOUS`` modes
- shared memory
- file
- named pipes
- message queues

## Setup
We utilized two processes in a producer-consumer fashion: to the first one we will refer to as 'Writer', and to the second, 'Reader'. Their purpose was to send back and forth a 4 MiB packet the ping way: from Writer to Reader, then from Reader to Writer.
Then, on Writer's side, we estimate latency as round-trip time divided by 2.
For these IPC media, we assume that the data transfer rate is the same in either direction, since we're using the same medium in both of them, and thus we can estimate throughput as the packet's size divided by half the round-trip time
(or, equivalently, double the packet's size divided by total round-trip time).

Some of these IPC media require certain coordination of their actions, which we provided using two semaphores:

![image](https://github.com/artandfi/ipc-latency-throughput/assets/28676554/22939c6b-16fa-499f-8f0e-18cae4bd62fb)

On the other hand, message queues and mmap in ``ANONYMOUS`` mode don't require such setup: message queues' function ``msgrcv`` inherently waits for ``msgsnd``'s results to arrive, and, for mmap in ``ANONYMOUS`` mode, we can simply await for the child process to send its result using
``wait(NULL)`` (assuming it terminates right after sending the result).

For each of the chosen IPC techniques, we conducted 5 measurements on 64-bit Ubuntu 20.04.5 LTS on an Oracle VM VirtualBox virtual machine with 7.8 GB RAM.

**Latency, ms**
|mmap SHARED|mmap SHARED&#124;ANONYMOUS|shmem|file|named pipes|message queues|
|-----------|--------------------------|-----|----|-----------|--------------|
|2.416|1.747|2.004|3.896|1.386|1.393|
|1.691|1.929|1.985|3.41|1.452|1.407|
|1.453|1.764|1.856|2.329|1.205|0.789|
|2.324|2.559|1.93|4.353|1.419|0.982|
|2.612|1.631|1.981|4.176|1.306|1.386|

**Throughput, Gbps**
|mmap SHARED|mmap SHARED&#124;ANONYMOUS|shmem|file|named pipes|message queues|
|-----------|--------------------------|-----|----|-----------|--------------|
|13.89128319|19.2124289|16.74793976|8.613674353|24.20079854|24.07924761|
|19.84291668|17.39470143|16.89977356|9.838578893|23.11707427|23.83975285|
|23.10113591|19.02175584|18.0788763|14.40726647|27.84598426|42.5547373|
|14.44132422|13.11491752|17.38572562|7.70745303|23.6381746|34.16948474|
|12.84379771|20.56660189|16.94238769|8.035028173|25.68273002|24.21824684|

![image](https://github.com/artandfi/ipc-latency-throughput/assets/28676554/ea084729-0695-4c22-ab30-f280250e3b87)

![image](https://github.com/artandfi/ipc-latency-throughput/assets/28676554/caf2f302-da4d-4c85-a6fe-f860e0021fa5)

**Summary**
|Technique|Average Latency, ms|Average Throughput, Gbps|Capacity (max size of data to transfer)|
|---------|-----------|----------------|---------------------------------------|
|mmap SHARED|2.0992|16.82409154|2<sup>b</sup> B, b = bitness of the machine;<br>mine is 64-bit, hence 2<sup>64</sup> B = 16EiB|
|mmap SHARED&#124;ANONYMOUS|1.926|17.86208112|>>|
|shared memory|1.9512|17.2109406|16 EiB - 16 777 217 B &#8776; 16EiB - 16 MiB<br>(from proc/sys/kernel/shmmax, can be changed there)|
|file|3.6328|9.72040018|Depends on the filesystem.<br>I have ext4, hence max is 16 TiB|
|named pipes|1.3536|24.8969523|64 KiB (from proc/sys/fs/pipe-max-size, can be changed there)|
|message queues|1.1914|29.77229387|8 KiB (from proc/sys/kernel/msgmax, can be changed there)|

A few takeaways:
- Obviously file-based IPC is the slowest due to the inherent slowness of disk read/writes.
- The best results for message queues might be due to a certain overhead of semaphores and ``wait`` in other methods.
- All throughputs listed here are quite huge (up to tens of Gbps) compared to common Internet speeds (typically hundreds of Mbps), since our setup might experience fewer impeding factors than the Internet networks.
- Default capacity for named pipes and message queues is quite low by default but can nonetheless be increased manually if necessary.

## Also
Shoutout to [Jacob Sorber](https://www.youtube.com/@JacobSorber) for his awesome tutorials on programming in C and IPC specifically!
