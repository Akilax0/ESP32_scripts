
INTRODUCTION


General Purpose Operating System
    Software that runs on a machine(microcontrollers & controllers) to handle various functions
         1) Task scheduling -> OS figure out how slices of time is given to each task
         2) Resource Management -> How files are accessed for each task
         3) Device Drivers->

These OSes are designed for humans in mind. Tasks priority for human interaction. Might miss some tasks.
alternatively task scheduler non-deterministic can't determine when tasks are executed or how long.


Hence these are not suitable for application-specific tasks.


Super Loop

    round-robin forever loop
         Easy to implement
         Great if only a handful of tasks are to be done
         much easier to debug
    ISR to break the flow


Downside
    Cannot execute tasks concurrently. Lags. Polling for user input might miss data


RTOS

multicore processors

Can prioritize the tasks.
Task: set of program instructions loaded in mem
Thread: unit of CPU utilization with its own program counter and stack
Process: Instance of a computer program


Usually process can have one or more threads to accomplish a task

FreeRTOS: Task = Thread
ISR still works as long as a higher priority at current

Why RTOS
To do things concurrently
SD card, Wireless, motors, inputs


Getting Started

Most ESP32 would not use vanilla FreeRTOS.
Modified version inside IDF.
Symmetric multiprocessing architecture to support the two cores.
Read ESP32 IDF to SMP changes page after FreeRTOS




##Blink Program





