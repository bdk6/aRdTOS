# aRdTOS

aRdTOS -- A simple RTOS for Arduino

Introduction:
ardtos is a very simple RTOS for Arduino.  It is intended to bring the
benefits of an RTOS to less-experienced programmers, without all the
complexity found with most RTOSs.  It should be easy to build a sketch
around ardtos without a lot of hassle.  The idea is to be as simple as
possible: easy to use, easy to understand.

What's an RTOS?
The definition of an RTOS varies, depending on who you ask and what they
are selling.  Most people would agree that an RTOS is a (usually small)
efficient operating system designed primarily to allow your program to meet
"deadlines."  In essence, it allows your code to do things WHEN you want or
need it done.

There are lots of ways to build an RTOS.  ardtos is one of the simplest but
it should be very effective.  I know of commercial products that are
rather large, but use an RTOS very similar.

Overview:
ardtos, like most RTOSs, is task based.  You create a set of "tasks" to
run, with various conditions on when they run.  In ardtos, there are two
types of tasks: normal tasks and interrupt tasks.

Normal tasks run in order in a loop.  If you create three normal tasks,
say task1, task2, and task3, in that order, then each will run in turn
until it finishes, followed by the next, in a circular pattern: task1,
task2, task3, task1, task2, ....
Unlike some RTOSs and most general purpose operating systems,
a normal task in ardtos will not be "preempted" by another normal task.  It
will continue running until it is complete.  It is basically a function
call, like any other.  It can, and probably will, be preempted by interrupt
tasks.  Also, a task can be ready to run or not ready to run.  If its turn
comes around and it is not ready to run for some reason, it will not run on
that trip through the loop.

Interrupt tasks are much like interrupt service routines.  The RTOS has a
source of interrupts every millisecond or so (called ticks.)  Each time the
tick interrupt happens, the list of interrupt tasks is scanned and all
interrupt tasks that are ready to run get executed in order.  Interupt tasks
will not be preempted by any other tasks in the system.  Each interrupt
task runs to completion before the next interrupt task is run.  After all
the ready interrupt tasks execute, the RTOS will return back to the normal
task that was running when the interrupt happened.

