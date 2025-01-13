# SPINLOCK

Spinlock is a synchronization mechanism used in operating systems to protect shared resources from single access by multiple threads or processes. Unlike other synchronization methods such as semaphores or mutexes, spinlocks use a busy-wait method, where a thread continuously selects a lock until it becomes available. It is used in interrupt context things.

**Example code to understand the spinlock**
```
int lock(lock)
{
    while (test_and_set(lock) == 1)
        ;
    return 0;
}

int unlock(lock)
{
    lock=0;

    return lock;
}
```

* The first thread will execute the test_and_set which will set the lock to 1. When the second thread calls the lock function, it will spin in the while loop, until the first thread calls the unlock function and the lock will be equal to 0. 

**Note:** Don't ever use the sleep in the spinlock code as it is used only for the short duration tasks unlike mutex and sempahore.

## Primary Terminologies

* **Critical Section:** A critical section refers to a part of the code that must be executed atomically to maintain data consistency. It is the region of code protected by a spinlock to ensure exclusive access.

* **Lock State:** The lock state indicates whether a spinlock is currently held (locked) or available (unlocked). It is typically represented by a binary variable, with – 0 denoting an unlocked state and 1 denoting a locked state.

* **Acquiring and Releasing:** Acquiring a spinlock involves trying to find a special way to acquire a key by setting the state of the lock to 1. Releasing a spinlock involves releasing the lock, allowing other threads to acquire it.

* **Spin-waiting:** Spin-waiting refers to the process of repeatedly checking the lock state in a loop until it becomes available. Threads in the waiting state are said to be spinning.

* ** Context Switching:** In operating system context switching is used to manage multiple threads or processes. When a thread is waiting for a lock in a spinlock scenario, it keeps checking repeatedly if the lock is available. But if the thread gets preempted or interrupted frequently by the operating system.

* ** Busy Waiting:** Busywaiting means that a process is waiting for a condition to be satisfied in a tight loop without relinquishing the processor. 

## Spinlock in Linux Kernel

### Initialization

We can initialize the SpinLock in Linux Kernel in two ways
    1. Static Method
    2. Dynamic Method

#### Static Method

Statically we can define the spinlock like below 
```
DEFINE_SPINLOCK(etx_spinlock);
```
* The macro given above will create a spinlock_t variable in the name of etx_spinlock and initialize to UNLOCKED STATE. Take a look at the expansion of DEFINE_SPINLOCK below.
```
#define DEFINE_SPINLOCK(x)      spinlock_t x = __SPIN_LOCK_UNLOCKED(x)
```

#### Dynamic Method

We can dynamically initialize the spinlock as follows
```
spinlock_t etx_spinlock;
spin_lock_init(&etx_spinlock);
```

### Different API's to modify the spinlock in the Kernel

```
spin_lock(spinlock_t *lock)
spin_trylock(spinlock_t *lock)
spin_unlock(spinlock_t *lock)
spin_is_locked(spinlock_t *lock)
spin_lock_bh(spinlock_t *lock)
spin_unlock_bh(spinlock_t *lock)
spin_lock_irq(spinlock_t *lock)
spin_unlock_irq(spinlock_t *lock)
spin_lock_irqsave( spinlock_t *lock, unsigned long flags );
spin_unlock_irqrestore( spinlock_t *lock, unsigned long flags );
```


* spin_lock_init - produces initialization of the given spinlock;
* spin_lock - acquires given spinlock;
* spin_lock_bh - disables software interrupts and acquire given spinlock;
* spin_lock_irqsave and spin_lock_irq - disable interrupts on local processor and preserve/not preserve previous interrupt state in the flags;
* spin_unlock - releases given spinlock and acquire given spinlock;
* spin_unlock_bh - releases given spinlock and enables software interrupts;
* spin_is_locked - returns the state of the given spinlock;

### **Advantages of Spinlock:**

1. **Low Overhead:**  
   No context switching occurs while waiting, reducing system overhead compared to traditional blocking locks.

2. **Efficient for Short Wait Times:**  
   If the lock is expected to be held for a short duration, spinlocks can be faster because they avoid the cost of putting threads to sleep and waking them up.

3. **No Sleeping or Scheduling:**  
   Spinlocks are ideal for real-time systems or interrupt contexts where sleeping is not permitted.

4. **Simple Implementation:**  
   Spinlocks are simpler to implement compared to other synchronization mechanisms like semaphores.

5. **Multiprocessor Support:**  
   Useful in multiprocessor environments where threads may spin on different processors, leading to minimal contention and quick access.

### **Disadvantages of Spinlock:**

1. **CPU Intensive:**  
   Spinlocks waste CPU cycles by continuously polling, which can degrade system performance if the lock is held for long periods.

2. **Not Suitable for Single Processor Systems:**  
   In single-core environments, they can lead to deadlocks because the thread holding the lock cannot release it while another thread is spinning.

3. **No Priority Handling:**  
   Spinlocks do not provide any mechanism for priority inheritance or fairness, potentially leading to priority inversion.

4. **Busy-Wait Loop:**  
   The constant polling in a busy-wait loop can increase power consumption and heat generation.

5. **Starvation Risks:**  
   Threads may starve if they continuously fail to acquire the lock due to high contention.

### **Use Cases:**  
Spinlocks are typically used in kernel development, low-level systems programming, or scenarios with short critical sections where minimizing locking overhead is crucial. However, they are generally avoided in user-space applications due to their inefficiency in high-contention or long-wait scenarios.
