#include "threadsync.h"
#include "thread.h"
#include "assert.h"
#include "cpu/atomic.h"
#include "printk.h"
#include "time.h"
#include "export.h"
#include "cpu/control_regs.h"

#define MUTEX_DEBUG 0
#if MUTEX_DEBUG
#define MUTEX_DTRACE(...) printdbg("mutex: " __VA_ARGS__)
#else
#define MUTEX_DTRACE(...) ((void)0)
#endif

#define DEBUG_CONDVAR 0
#if DEBUG_CONDVAR
#define CONDVAR_DTRACE(...) printdbg("condvar: " __VA_ARGS__)
#else
#define CONDVAR_DTRACE(...) ((void)0)
#endif

#define SPINCOUNT_MAX   4096
#define SPINCOUNT_MIN   4

//
// Wait chain

static void thread_wait_add(thread_wait_link_t *root,
                            thread_wait_link_t *node)
{
    thread_wait_link_t *insafter = root->prev;
    node->next = root;
    node->prev = insafter;
    insafter->next = node;
    root->prev = node;
}

static thread_wait_link_t *thread_wait_del(
        thread_wait_link_t *node)
{
    thread_wait_link_t *next = node->next;
    thread_wait_link_t *prev = node->prev;
    prev->next = next;
    next->prev = prev;
    node->next = nullptr;
    node->prev = nullptr;
    return next;
}

//
// Mutex

EXPORT void mutex_init(mutex_t *mutex)
{
    mutex->owner = -1;
    mutex->lock = 0;
    mutex->spin_count = spincount_mask &
            (SPINCOUNT_MIN + ((SPINCOUNT_MAX-SPINCOUNT_MIN)>>1));
    mutex->link.next = &mutex->link;
    mutex->link.prev = &mutex->link;
}

EXPORT bool mutex_try_lock(mutex_t *mutex)
{
    bool result;

    thread_t this_thread_id = thread_get_id();

    assert(mutex->owner != this_thread_id);

    spinlock_lock(&mutex->lock);

    if (mutex->owner < 0) {
        mutex->owner = this_thread_id;
        result = true;
    } else {
        result = false;
    }

    spinlock_unlock(&mutex->lock);

    return result;
}

_hot
EXPORT bool mutex_lock(mutex_t *mutex, int64_t timeout_time)
{
    bool result = true;
    assert(mutex->owner != thread_get_id());

    for (int spin = 0; result; pause(), ++spin) {
        // Spin outside lock until spin limit
        if (spin < mutex->spin_count && mutex->owner >= 0)
            continue;

        // Lock the mutex to acquire it or manipulate wait chain
        spinlock_lock(&mutex->lock);

        // Check again inside lock
        if (spin < mutex->spin_count && mutex->owner >= 0) {
            // Racing thread beat us, go back to spinning outside lock
            spinlock_unlock(&mutex->lock);
            continue;
        }

        if (mutex->owner < 0) {
            // Take ownership
            mutex->owner = thread_get_id();

            MUTEX_DTRACE("Took ownership of %p, tid=%d\n",
                         (void*)mutex, mutex->owner);

            // Increase spin count
            if (mutex->spin_count < SPINCOUNT_MAX)
                mutex->spin_count -= spincount_mask;

            break;
        }

        // Mutex is owned
        thread_wait_t wait;

        // Decrease spin count
        if (mutex->spin_count > SPINCOUNT_MIN)
            mutex->spin_count += spincount_mask;

        MUTEX_DTRACE("Adding to waitchain of %p\n", (void*)mutex);

        // Add state to mutex wait chain
        thread_wait_add(&mutex->link, &wait.link);

        MUTEX_DTRACE("Waitchain for %p\n", (void*)mutex);

        // Wait
        // note: returns with mutex->lock unlocked!
        result = thread_sleep_release(&mutex->lock, &wait.thread, timeout_time);

        if (result) {
            assert(mutex->lock != 0);
            assert(wait.link.next == nullptr);
            assert(wait.link.prev == nullptr);
            assert(mutex->owner == wait.thread);
        }

        return result;
    }

    // Release lock
    spinlock_unlock(&mutex->lock);

    return result;
}

EXPORT void mutex_unlock(mutex_t *mutex)
{
    spinlock_lock(&mutex->lock);

    atomic_barrier();
    assert(mutex->owner == thread_get_id());

    // See if any threads are waiting
    if (mutex->link.next != &mutex->link) {
        // Wake up the first waiter
        thread_wait_t *waiter = (thread_wait_t*)mutex->link.next;
        thread_wait_del(&waiter->link);
        thread_t waked_thread = waiter->thread;
        MUTEX_DTRACE("Mutex unlock waking waiter, old_tid=%d, new_tid=%d\n",
                     mutex->owner, waked_thread);
        mutex->owner = waked_thread;
        spinlock_unlock(&mutex->lock);
        thread_resume(waked_thread, 1);
    } else {
        // No waiters
        MUTEX_DTRACE("Mutex unlock waking waiter, old_tid=%d, new_tid=none\n",
                     mutex->owner);
        mutex->owner = -1;
        spinlock_unlock(&mutex->lock);
    }
}

EXPORT void mutex_destroy(mutex_t *mutex)
{
    (void)mutex;
    assert(mutex->link.next == mutex->link.prev);
    assert(mutex->owner == -1);
}

EXPORT int mutex_held(mutex_t *mutex)
{
    return mutex->owner == thread_get_id();
}

//
// Reader/Writer lock

EXPORT void rwlock_init(rwlock_t *rwlock)
{
    rwlock->lock = 0;
    rwlock->ex_link.next = &rwlock->ex_link;
    rwlock->ex_link.prev = &rwlock->ex_link;
    rwlock->sh_link.next = &rwlock->sh_link;
    rwlock->sh_link.prev = &rwlock->sh_link;
    rwlock->reader_count = 0;
    rwlock->spin_count = spincount_mask &
            (SPINCOUNT_MIN + ((SPINCOUNT_MAX-SPINCOUNT_MIN)>>1));
}

EXPORT void rwlock_destroy(rwlock_t *rwlock)
{
    (void)rwlock;
    assert(rwlock->ex_link.next == rwlock->ex_link.prev);
    assert(rwlock->sh_link.next == rwlock->sh_link.prev);
}

EXPORT bool rwlock_ex_try_lock(rwlock_t *rwlock)
{
    if (rwlock->reader_count < 0)
        return false;

    spinlock_lock(&rwlock->lock);

    thread_t tid = thread_get_id();

    bool result = false;

    if (rwlock->reader_count == 0 &&
            rwlock->ex_link.next == &rwlock->ex_link) {
        // Acquire exclusive lock
        rwlock->reader_count = -tid;

        result = true;
    }

    spinlock_unlock(&rwlock->lock);

    return result;
}

EXPORT bool rwlock_ex_lock(rwlock_t *rwlock, int64_t timeout_time)
{
    bool result = true;
    thread_t tid = thread_get_id();

    int spin = 0;
    for (bool done = false; result && !done; pause(), ++spin) {
        // Spin only if someone has exclusive lock
        // and there are no waiting writers
        if (spin < rwlock->spin_count &&
                rwlock->reader_count < 0 &&
                rwlock->ex_link.next == &rwlock->ex_link)
            continue;

        spinlock_lock(&rwlock->lock);

        // If there is no owner and there is no next writer
        if (rwlock->reader_count == 0 &&
                rwlock->ex_link.next == &rwlock->ex_link) {
            // Acquire exclusive lock
            rwlock->reader_count = -tid;

            done = true;
        } else if (spin >= rwlock->spin_count) {
            thread_wait_t wait;

            // Spinning did not help, so try to reduce spin count by one
            if (rwlock->spin_count > SPINCOUNT_MIN)
                rwlock->spin_count += spincount_mask;

            thread_wait_add(&rwlock->ex_link, &wait.link);

            // note: returns with rwlock->lock unlocked!
            result = thread_sleep_release(&rwlock->lock, &wait.thread,
                                          timeout_time);

            if (result) {
                assert(wait.thread == tid);
                assert(rwlock->lock != 0);
                assert(wait.link.next == nullptr);
                assert(wait.link.prev == nullptr);
                assert(rwlock->reader_count == -wait.thread);
            }

            return result;
        }

        spinlock_unlock(&rwlock->lock);
    }

    return result;
}

EXPORT bool rwlock_upgrade(rwlock_t *rwlock, int64_t timeout_time)
{
    bool result = true;
    thread_t tid = thread_get_id();

    spinlock_lock(&rwlock->lock);

    assert(rwlock->reader_count > 0);

    if (rwlock->reader_count == 1) {
        // I am the only reader, direct upgrade
        rwlock->reader_count = -tid;
    } else {
        thread_wait_t wait;

        // Add self to exclusive wait chain and suspend
        thread_wait_add(&rwlock->ex_link, &wait.link);

        // note: returns with rwlock->lock unlocked!
        result = thread_sleep_release(&rwlock->lock, &wait.thread,
                                      timeout_time);

        if (result) {
            assert(wait.thread == tid);
            assert(wait.link.next == nullptr);
            assert(wait.link.prev == nullptr);
            assert(rwlock->reader_count == -wait.thread);
        }

        return result;
    }

    spinlock_unlock(&rwlock->lock);

    return result;
}

EXPORT void rwlock_ex_unlock(rwlock_t *rwlock)
{
    spinlock_lock(&rwlock->lock);

    atomic_barrier();
    assert(rwlock->reader_count == -thread_get_id());

    if (rwlock->ex_link.next != &rwlock->ex_link) {
        // Wake and transfer ownership to next writer
        thread_wait_t *waiter = (thread_wait_t*)rwlock->ex_link.next;
        thread_wait_del(&waiter->link);
        thread_t waked_thread = waiter->thread;
        rwlock->reader_count = -waked_thread;
        spinlock_unlock(&rwlock->lock);
        thread_resume(waked_thread, 1);
    } else if (rwlock->sh_link.next != &rwlock->sh_link) {
        // Wake all readers
        rwlock->reader_count = 0;
        do {
            thread_wait_t *waiter = (thread_wait_t*)rwlock->sh_link.next;
            thread_wait_del(&waiter->link);
            thread_t waked_thread = waiter->thread;
            ++rwlock->reader_count;
            thread_resume(waked_thread, 1);
        } while (rwlock->sh_link.next != &rwlock->sh_link);
        spinlock_unlock(&rwlock->lock);
    } else {
        // No waiters
        rwlock->reader_count = 0;
        atomic_barrier();
        spinlock_unlock(&rwlock->lock);
    }
}

EXPORT bool rwlock_sh_try_lock(rwlock_t *rwlock)
{
    if (rwlock->reader_count < 0)
        return false;

    spinlock_lock(&rwlock->lock);

    bool result = false;

    if (rwlock->reader_count >= 0 &&
            rwlock->ex_link.next == &rwlock->ex_link) {
        ++rwlock->reader_count;
        result = true;
    }

    spinlock_unlock(&rwlock->lock);

    return result;
}

EXPORT bool rwlock_sh_lock(rwlock_t *rwlock, int64_t timeout_time)
{
    bool result = true;
    int spin = 0;
    for (bool done = false; result && !done; pause(), ++spin) {
        // Spin outside lock until spin limit
        //  while the exclusive lock is held, and,
        //  there is a next writer
        if (spin < rwlock->spin_count &&
                rwlock->reader_count < 0 &&
                rwlock->ex_link.next == &rwlock->ex_link)
            continue;

        spinlock_lock(&rwlock->lock);

        // If there is no exclusive owner and there is no next writer
        if (rwlock->reader_count >= 0 &&
                rwlock->ex_link.next == &rwlock->ex_link) {
            // Acquire exclusive lock
            ++rwlock->reader_count;

            done = true;
        } else if (spin >= rwlock->spin_count) {
            thread_wait_t wait;

            // Decrease spin count
            if (rwlock->spin_count > SPINCOUNT_MIN)
                rwlock->spin_count += spincount_mask;

            // Register to be awakened
            thread_wait_add(&rwlock->sh_link, &wait.link);

            // Wait
            // note: returns with rwlock->lock unlocked!
            result = thread_sleep_release(&rwlock->lock, &wait.thread,
                                          timeout_time);

            if (result) {
                // We were awakened
                assert(wait.link.next == nullptr);
                assert(wait.link.prev == nullptr);
                assert(rwlock->reader_count > 0);
            }

            return result;
        }

        spinlock_unlock(&rwlock->lock);
    }

    return result;
}

EXPORT void rwlock_sh_unlock(rwlock_t *rwlock)
{
    spinlock_lock(&rwlock->lock);

    atomic_barrier();

    assert(rwlock->reader_count > 0);
    --rwlock->reader_count;

    if (!rwlock->reader_count && rwlock->ex_link.next != &rwlock->ex_link) {
        // Wake first exclusive waiter
        thread_wait_t *waiter = (thread_wait_t*)rwlock->ex_link.next;
        thread_wait_del(&waiter->link);
        thread_t waked_thread = waiter->thread;
        rwlock->reader_count = -waked_thread;
        spinlock_unlock(&rwlock->lock);
        thread_resume(waked_thread, 1);
    } else {
        // No waiters
        atomic_barrier();
        spinlock_unlock(&rwlock->lock);
    }
}

EXPORT bool rwlock_have_ex(rwlock_t *rwlock)
{
    return rwlock->reader_count == -thread_get_id();
}

//
// Condition variable

EXPORT void condvar_init(condition_var_t *var)
{
    var->lock = 0;
    var->link.next = &var->link;
    var->link.prev = &var->link;
    atomic_barrier();
}

EXPORT void condvar_destroy(condition_var_t *var)
{
    if (unlikely(var->link.prev != &var->link)) {
        spinlock_lock(&var->lock);
        for (thread_wait_link_t *node = var->link.next;
             node != &var->link; node = thread_wait_del(node));
        spinlock_unlock(&var->lock);
    }
    assert(var->link.next == &var->link);
    assert(var->link.prev == &var->link);
}

class condvar_spinlock_t {
public:
    condvar_spinlock_t(spinlock_t *spinlock)
        : spinlock(spinlock)
    {
    }

    _always_inline void lock()
    {
        spinlock_lock(spinlock);
    }

    _always_inline void unlock()
    {
        spinlock_unlock(spinlock);
    }

protected:
    spinlock_t *spinlock;
};

class condvar_ticketlock_t {
public:
    condvar_ticketlock_t(ticketlock_t *ticketlock)
        : ticketlock(ticketlock)
    {
    }

    _always_inline void lock()
    {
        ticketlock_lock_restore(ticketlock, saved_lock);
    }

    _always_inline void unlock()
    {
        saved_lock = ticketlock_unlock_save(ticketlock);
    }

protected:
    ticketlock_t *ticketlock;
    ticketlock_value_t saved_lock;
};

class condvar_mcslock_t {
public:
    condvar_mcslock_t(mcs_queue_ent_t * volatile *root, mcs_queue_ent_t * node)
        : root(root)
        , node(node)
    {
    }

    _always_inline void lock()
    {
        mcslock_lock(root, node);
    }

    _always_inline void unlock()
    {
        mcslock_unlock(root, node);
    }

protected:
    mcs_queue_ent_t * volatile *root;
    mcs_queue_ent_t * node;
};

class condvar_mutex_t {
public:
    condvar_mutex_t(mutex_t *mutex)
        : mutex(mutex)
    {
    }

    _always_inline void lock()
    {
        mutex_lock(mutex);
    }

    _always_inline void unlock()
    {
        mutex_unlock(mutex);
    }

protected:
    mutex_t *mutex;
};

template<typename T>
static bool condvar_wait_ex(condition_var_t *var, T& lock_upd,
                            uint64_t timeout_time)
{
    uintptr_t result;

    // Lock the condition variable
    spinlock_lock(&var->lock);

    // Uninterruptible code ahead
    cpu_scoped_irq_disable irq_dis;

    thread_wait_t wait;
    thread_wait_add(&var->link, &wait.link);

    // Unlock whatever lock is protecting the condition
    lock_upd.unlock();

    // Atomically unlock the condition variable and suspend
    // note: returns with var->lock unlocked!
    CONDVAR_DTRACE("%p: Suspending\n", (void*)&wait);
    result = thread_sleep_release(&var->lock, &wait.thread, timeout_time);
    CONDVAR_DTRACE("%p: Awoke\n", (void*)&wait);

    // (do not) Release (not) reacquired condition variable lock
    //spinlock_unlock(&var->lock);

    // Sanely disconnected nodes will have nulled links
    if (result) {
        assert(wait.link.next == nullptr);
        assert(wait.link.prev == nullptr);
    } else {
        // Reacquire condition variable lock to remove this thread from the
        // notification list
        spinlock_lock(&var->lock);

        // Possible that condition was notified after timer expiry
        if (likely(wait.link.next))
            thread_wait_del(&wait.link);
        else
            result = true;

        spinlock_unlock(&var->lock);
    }

    // Reacquire lock protecting condition before returning
    lock_upd.lock();

    return result;
}

EXPORT bool condvar_wait_spinlock(condition_var_t *var, spinlock_t *lock,
                                  int64_t timeout_time)
{
    condvar_spinlock_t state(lock);
    return condvar_wait_ex(var, state, timeout_time);
}

EXPORT bool condvar_wait_ticketlock(condition_var_t *var, ticketlock_t *lock,
                                    int64_t timeout_time)
{
    condvar_ticketlock_t state(lock);
    return condvar_wait_ex(var, state, timeout_time);
}

EXPORT bool condvar_wait_mcslock(condition_var_t *var,
                                 mcs_queue_ent_t * volatile *root,
                                 mcs_queue_ent_t *node, int64_t timeout_time)
{
    condvar_mcslock_t state(root, node);
    return condvar_wait_ex(var, state, timeout_time);
}

EXPORT bool condvar_wait(condition_var_t *var, mutex_t *mutex,
                         int64_t timeout_time)
{
    assert(mutex->owner == thread_get_id());
    condvar_mutex_t state(mutex);
    return condvar_wait_ex(var, state, timeout_time);
}

EXPORT void condvar_wake_one(condition_var_t *var)
{
    spinlock_lock(&var->lock);

    thread_wait_t *wait = (thread_wait_t*)var->link.next;
    if ((void*)wait != (void*)&var->link) {
        CONDVAR_DTRACE("%p: Removing wait\n", (void*)wait);
        thread_wait_del(&wait->link);
        CONDVAR_DTRACE("%p: Waking id %d\n", (void*)wait, wait->thread);
        thread_resume(wait->thread, 1);
    } else {
        CONDVAR_DTRACE("No waiters when waking\n");
    }
    spinlock_unlock(&var->lock);
}

EXPORT void condvar_wake_n(condition_var_t *var, size_t n)
{
    spinlock_lock(&var->lock);

    thread_wait_t *next_wait;
    for (thread_wait_t *wait = (thread_wait_t*)var->link.next;
         wait != (void*)&var->link && n--;
         wait = next_wait) {
        CONDVAR_DTRACE("%p: Waking id %d\n", (void*)wait, wait->thread);
        next_wait = (thread_wait_t*)thread_wait_del(&wait->link);
        thread_resume(wait->thread, 1);
    }

    spinlock_unlock(&var->lock);
}

EXPORT void condvar_wake_all(condition_var_t *var)
{
    spinlock_lock(&var->lock);

    thread_wait_t *next_wait;
    for (thread_wait_t *wait = (thread_wait_t*)var->link.next;
         wait != (void*)&var->link;
         wait = next_wait) {
        CONDVAR_DTRACE("%p: Waking id %d\n", (void*)wait, wait->thread);
        next_wait = (thread_wait_t*)thread_wait_del(&wait->link);
        thread_resume(wait->thread, 1);
    }

    spinlock_unlock(&var->lock);
}

