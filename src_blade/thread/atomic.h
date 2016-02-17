#ifndef THREAD_ATOMIC_H_
#define THREAD_ATOMIC_H_

#include <boost/noncopyable.hpp>
#include <stdint.h>

namespace simple_log {

template<typename T>
class AtomicIntegerT : public boost::noncopyable {
 public:
  AtomicIntegerT() : value_(0) {}

  T get() const {
    return __sync_val_compare_and_swap(const_cast<volatile T*>(&value_), 0, 0);
  }

  T getAndAdd(T x) {
    return __sync_fetch_and_add(&value_, x);
  }

  void add(T x) { getAndAdd(x); }

  T addAndGet(T x) { return getAndAdd(x) + x; }

  T incrementAndGet() { return addAndGet(1); }

  void increment() { incrementAndGet(); }
  void decrement() { getAndAdd(-1); }

  T getAndSet(T newValue) {
    return __sync_lock_test_and_set(&value_, newValue);
  }

 private:
  volatile T value_;
}; // class AtomicIntegerT

typedef AtomicIntegerT<int32_t> AtomicInt32;
typedef AtomicIntegerT<int64_t> AtomicInt64;

} // namespace simple_log

#endif // THREAD_ATOMIC_H_
