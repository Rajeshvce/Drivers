# Semaphore

A semaphore is a synchronization tool used in concurrent programming to manage access to shared resources. It is a lock-based mechanism designed toachieve process synchronization, built on top of basic locking techniques (spin locks).

Semaphores use a counter to control access, allowing synchronization for multiple instances of a resource. Processes can attempt to access one instance, and if it is not available, they can try other instances. Unlike basic locks, which allow only one process to access one instance of a resource. Semaphores can handle more complex synchronization scenarios, involving multiple processes or threads. It help prevent problems like race conditions by controlling when and how processes access shared data.

The process of Semaphores provides two operations:
* wait (P): The wait operation decrements the value of the semaphore
* signal (V): The signal operation increments the value of the semaphore.

When the value of the semaphore is zero, any process that performs a wait operation will be blocked until another process performs a signal operation.

## Working of Semaphore

There are two primary operations that a semaphore can perform:

* **Wait (P operation):** This operation checks the semaphore’s value. If the value is greater than 0, the process is allowed to continue, and the semaphore’s value is decremented by 1. If the value is 0, the process is blocked (waits) until the semaphore value becomes greater than 0.
* **Signal (V operation):** After a process is done using the shared resource, it performs the signal operation. This increments the semaphore’s value by 1, potentially unblocking other waiting processes and allowing them to access the resource.

```
wait(Sempahore s){
    while(s==0); /* wait untill the resource is freed */
    s = s-1;
}
signal(Semaphore s){
    s = s+1;
    wakeup(from wait queue)
}
```
In this implementation whenever the process waits it is added to a waiting queue of processes associated with that semaphore. This is done through the system call block() on that process. When a process is completed it calls the signal function and one process in the queue is resumed. It uses the wakeup() system call.

## Types of semaphores

* Binary semaphore
* Counting semaphore

### Binary semaphore

This is also known as a mutex lock, as they are locks that provide mutual exclusion. It can have only two values – 0 and 1. Its value is initialized to 1. It is used to implement the solution of critical section problems with multiple processes and a single resource.

** Example program for Binary semaphore **
```
struct semaphore {
  
    enum value(0, 1);

    // q contains all Process Control Blocks (PCBs)
    // corresponding to processes got blocked
    // while performing down operation.
    Queue<process> q;

};
P(semaphore s)
{
    if (s.value == 1) {
        s.value = 0;
    }
    else {
        // add the process to the waiting queue
        q.push(P) sleep();
    }
}
V(semaphore s)
{
    if (s.q is empty) {
        s.value = 1;
    }
    else {

        // select a process from waiting queue
        Process p = q.front();
        // remove the process from waiting as it has been
        // sent for CS
        q.pop();
        wakeup(p);
    }
}
```
**Limitations:**
* One of the biggest limitations of semaphore is priority inversions.
* Deadlock, suppose a process is trying to wake up another process that is not in a sleep state. Therefore, a deadlock may be blocked indefinitely.
* The operating system has to keep track of all calls to wait and signal the semaphore.


### Counting semaphore

Counting semaphores can be used to control access to a given resource consisting of a finite number of instances. The semaphore is initialized to the number of resources available. Its value can range over an unrestricted domain.

** Example program for couting semaphore **
```
struct Semaphore {

    int value;

    // q contains all Process Control Blocks(PCBs)
    // corresponding to processes got blocked
    // while performing down operation.
    Queue<process> q;

};
P(Semaphore s)
{
    s.value = s.value - 1;
    if (s.value < 0) {

        // add process to queue
        // here p is a process which is currently executing
        q.push(p);
        block();
    }
    else
        return;
}

V(Semaphore s)
{
    s.value = s.value + 1;
    if (s.value <= 0) {

        // remove process p from queue
        Process p = q.pop();
        wakeup(p);
    }
    else
        return;
}
```

## Semaphore in the Linux Kernel

Semaphore is defined as follows in the linux kernel

```
struct semaphore {
    raw_spinlock_t        lock;
    unsigned int        count;
    struct list_head    wait_list;
};
```

* lock - spinlock for a semaphore data protection;
* count - amount available resources;
* wait_list - list of processes which are waiting to acquire a lock.

### Initializing the semaphore

We can initialize the semaphore in two ways

1. Static method
2. Dynamic method

#### Static Method

This method will be useful while using the global Semaphore. The Macro is defined below
```
// This call defines and initializes the sempahore
DEFINE_MUTEX(name);
```

#### Dynamic Method

This method initializes the semaphore at the run time, providing the flexibility for different scenarios
```
struct semaphore name;
sema_init(&name, initial_value);
```

### API's provided by the Linux kernel to modify the semaphore

```
void down(struct semaphore *sem);
void up(struct semaphore *sem);
int  down_interruptible(struct semaphore *sem);
int  down_killable(struct semaphore *sem);
int  down_trylock(struct semaphore *sem);
int  down_timeout(struct semaphore *sem, long jiffies);
```
    
## Uses of Semaphores

* **Mutual Exclusion :** Semaphore ensures that only one process accesses a shared resource at a time.
* **Process Synchronization :** Semaphore coordinates the execution order of multiple processes.
* **Resource Management :** Limits access to a finite set of resources, like printers, devices, etc.
* **Reader-Writer Problem :** Allows multiple readers but restricts the writers until no reader is present.
* **Avoiding Deadlocks :** Prevents deadlocks by controlling the order of allocation of resources.

## Advantages

* Flexible resource management is possible with the help of semaphores.
* Semaphores can be programmed in machine-independent codes of the microkernel.
* Multiple processes cannot be entered in the critical section of Semaphores.
* They permit more than one thread access to the crucial section.
* Semaphores strictly follow the mutual exclusion principle, making them more efficient than other methods of synchronization.
* There is no waste of resources due to busy waiting in semaphores. The processor does not waste time unnecessarily in checking if there are any conditions that will allow access to the critical section.

## Disadvantages

* Semaphores can lead to priority inversion, whereby low priority processes might access the critical sections first while high priority processes may access it later. This is one of the greatest limitations.
* The Wait and Signal operations must be performed in the correct order to avoid semaphore deadlocks.
* It is not practical to use semaphores on a large scale. This is because their use results in loss of modularity. This happens because wait() and signal() operations stop the creation of a structured layout for the system.
* They are not required to be used, but they are agreed upon.
* Unsanctioned use can cause a process to block indefinitely. This is known asDeadlock. In the next lessons, we will discuss deadlocks in greater detail.

