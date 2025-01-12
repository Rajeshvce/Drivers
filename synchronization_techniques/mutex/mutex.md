# Mutexes

A mutex is a synchronization primitive that controls access to shared resources in concurrent programming. It is short for mutual exclusion. 
Mutex is mainly used to provide mutual exclusion to a specific portion of the code so that the process can execute and work with a particular section of the code at a particular time. A mutex enforces strict ownership. Only the thread that locks the mutex can unlock it. It is specifically used for locking a resource to ensure that only one thread accesses it at a time. Due to this strict ownership, a mutex is not only typically used for signaling between threads, but it is used for mutual exclusion also to ensuring that a resource is accessed by only one thread at a time.

Other processes which are trying to access the same critical section of the code will be pushed into the wait queue and slept.

## Advantages of mutex

* No race condition arises, as only one process is in the critical section at a time.
* Data remains consistent and it helps in maintaining integrity.
* It is a simple locking mechanism that into a critical section and is released while leaving the critical section.

## Disadvantages of Mutex

* If after entering into the critical section, the thread sleeps or gets preempted by a high-priority process, no other thread can enter into the critical section. This can lead to starvation.
* When the previous thread leaves the critical section, then only other processes can enter into it, there is no other mechanism to lock or unlock the critical section.
* Implementation of mutex can lead to busy waiting, which leads to the wastage of the CPU cycle. 


## Mutex in the Linux Kernel

Actual mutex type is defined as follows in the linux kernel

```
// defined in the include/linux/mutex.h
struct mutex {
    atomic_t        count;
    spinlock_t      wait_lock;
    struct list_head    wait_list;
};
```
### Initializing the mutex

We can initialize the mutex in two ways

1. Static method
2. Dynamic method

#### Static method

This method will be useful while using the global Mutex. The Macro is defined below

```
// This call defines and initializes a mutex
DEFINE_MUTEX(name);
```

#### Dynamic method

This method will be useful for per-object mutexes when the mutex is just a field in a heap
allocated object. This macro is defined below.
```
mutex_init(struct mutex *lock);
```
* It is not allowed to initialize an already locked mutex.

Example:
```
struct mutex etx_mutex; 
mutex_init(&etx_mutex);
```

### Different function definations for the Mutex locking and unlocking

```
void mutex_lock(struct mutex *lock);
int mutex_lock_interruptible(struct mutex *lock);
int mutex_trylock(struct mutex *lock);
void mutex_unlock(struct mutex *lock);
int mutex_is_locked(struct mutex *lock);
```
* Suffix _interruptible in Linux kernel means that waiting by the function will be interrupted if thread(process) receives the signal.



## Wait queue and Ready queue

### Wait queue
* A wait queue holds threads that are waiting to acquire a mutex or satisfy a condition to proceed.
* When a thread attempts to lock a mutex that is already locked by another thread, it may be placed in a wait queue.
* The thread remains in this queue until the mutex becomes available or a condition (signaled by another thread) is met.

### Ready queue
* A ready queue holds threads that are ready to run but waiting for CPU time.
* Threads in the ready queue are not blocked; they have all necessary resources (including mutex locks) to execute.
* They compete for CPU time based on the system's scheduling policy (e.g., round-robin, priority-based scheduling).

## Priority inversion
    
Priority inversion can happen when an operating system uses preemptive scheduling, which means that a higher-priority task can interrupt a lower-priority task and take over the CPU. However, if the lower-priority task is holding a lock or a semaphore that the higher-priority task needs, the higher-priority task has to wait until the lower-priority task releases it. This can create a deadlock situation if there is another medium-priority task that can preempt the lower-priority task, but not the higher-priority task. In this case, the medium-priority task can prevent the lower-priority task from finishing and releasing the resource, while the higher-priority task is blocked indefinitely.

Using the priority inheritance technique we can boost the priority of the low level task and avoid the priority inheritance. There are also several another methodsto avoid this issue.
