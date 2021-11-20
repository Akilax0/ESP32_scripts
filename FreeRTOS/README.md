
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

Task is a function that simply gets called during setup or from another task.

Have to tell schedular to proritize and time slize appropriately.

vTaskDelay() -> non blocking function 
tells the schedular to run other tasks until specified time is up.

Tick Timer -> hardware timers allocated to interrupt the proccessor at a specific interval. Interval -> TICK
Schedular opportunity to run each tick to figure out which task to run at each tick.
Tick period = 1ms (default)
function accepts number of ticks

xTaskCreatePinToCore we want to run task on one core
not in vanilla freertos 
xTaskCreate
can call in esp as well but runs in either core.


xTaskCreatePinToCore{
	functioncalled - toggleLED,
	Name of Task - "String",
	stack size in bytes (number of words in vanilla) - [1024],
	pointer to memory,
	task prority - 0 to Max_priority-1(24),
	task handle - manage task from other task or main loop (can check status or end it)
	cpu core - 1 [not in vanilla]
}

In vanila have to call vTaskStartScheduler() after assigning tasks. Here already called in setup function.


##Task Scheduling


What actually happens 

Time is divided into ticks. At the begining of a tick task scheduler runs and looks for tasks to run. 
Runs the highest priority task for rest of the tick. 
On next tick scheduler runs again to check the tasks.
On vtaskDelay() if no other task is available to run core remains in an idle state.
If two tasks with same priority is available both are run in a round robin fashion until tasks are over. 
Known as preemptive scheduling.

All this is related to software but if hardware interrupt occurs it has the highest priority and the current task pause exectures ISR then continues for the rest of the tick.

As an advanced option can opt to run tasks with same priority on the two cores simultanously. Bit complicated but can be done.

-----------Task States------------

After task creation ready to run.
Task can be in between ready and running states.
If a delay is given to a task it becomes blocked where the taskremains until event occures(timeout).
[Done by calling API]


Alternatively can use 
	-vTaskSuspend()
	-vTaskResume()

Useful to suspend task without a specified time delay.


Context switching <--check on freeRTOS documentation


##Memory Management

Compiler knows how much memory should be allocated at the befining of a program


RAM 
  - STATIC Static allocation (global/static variable)
  - STACK automatic allocation (grows down) local variables
  - HEAP grows towards the stack (dynamic allocation where the programmer explicitly asks for memory) malloc / calloc in C/C++ have to free this or could cause memory leaks.


--------------------Memory at runtime RTOS----------------------------------

At xTaskCreate heap portion is alloacted to the task.
Divides into two
 - Stack - we assign stack size on creation (min 700 for base info) 
 - TCB - struct that keeps critical info about the task (stack mem location , task priority)

kernel objects also in heap. queues and semaphores

[https://happilyembedded.wordpress.com/2015/10/23/rtos-conecpts-kernel-objects/]

In newer versions possible to store static data for a task/kernel objcet

 - '#define configSUPPORT_STATIC_ALLOCATION 1'

dynamic allocate tries to find the largest unallocated heap.
If constantly allocates could end up fragmentting the heap.
Hence FreeRTOS gives memory allocation schemes.

[https://freertos.org/a00111.html]

On ESP has couple of different RAM hence not the same as vanilla FreeRTOS.
[https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/mem_alloc.html?highlight=memory]


## Queue

Can use global variables to pass between functions

Task A update Global variable 
Task B read Global variable

but what happens when another task wants to change the value ?

Esp32 32 bit wide little endian
if 64 bit 
	Have to use 2 locations

	but here if above happens when updating the second part of the data the data gets corrupted.Not thread safe.

Or data gets  written to the same location at the same time

Can be solved using atomic actions
or use kernel obejects to tell other tasks to not to mess with our data.


Queue - allows data to be sent between takss uninteruptedly


Writing to a queue is atomic
added by value not reference.

reading removes data from queue 
can also specify a timeout if queue empty 
and if full

Queue Management[https://freertos.org/a00018.html]



