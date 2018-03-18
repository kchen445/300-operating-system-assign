-A process does not store it's state as it can be easily
extrapolated from its location in the system.
i.e ready if its in ready queue, blocked if it's in blocked queue

-This simulation uses a priority based round robin queue system.
All high priority processes will have to leave or block
before quantum allows a medium priority process to run.
This design can cause starvation but was chosen because 
high priority jobs are meant to be done as soon as possible. 
It may not be desirable to allow a new low priority job to run
while there are high priority jobs that are not yet done.

-Multiple processes can send to the same process without any problems
however, the receiving process can only claim one message at a time.
i.e must call r multiple times to get multiple messages.

-Calling reply on a process with an unclaimed message will unblock the
process and destroy its message.

-Any process can reply to a process doing a blocking send.

-semaphores block when its value is less than or equal to 0 as described
in lecture 8's implementation, therefore it is possible for a semaphore 
to be initialized such that calling P before V on it will block the 
calling process

-there are no restrictions on calling P and V on semaphores
i.e process 1 can call P and gain control of the semaphore only to switch
out for another process immediately after,
nothing stops this new process from calling V on the same semaphore.

