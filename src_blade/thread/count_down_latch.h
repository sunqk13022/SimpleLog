#ifndef THREAD_COUNT_DOWN_LATCH_H_
#define THREAD_COUNT_DOWN_LATCH_H_

#include "mutex.h"
#include "condition.h"

#include <boost/noncopyable.hpp>

namespace simple_log {

class CountDownLatch : public boost::noncopyable {
 public:
  explicit CountDownLatch(int count)
    : mutex_(),
      condition_(mutex_),
      count_(count)
  {
  }

  void wait() {
    MutexLockGuard lock(mutex_);
    while(count_ > 0) {
      condition_.wait();
    }
  }

  void countDown() {
    MutexLockGuard lock(mutex_);
    --count_;
    if (count_ == 0) {
      condition_.notifyAll();
    }
  }

  int getCount() const {
    MutexLockGuard lock(mutex_);
    return count_;
  }

 private:
  mutable MutexLock mutex_;
  Condition condition_;
  int count_;
}; // class CountDownLatch


} // namespace simple_log

#endif // THREAD_COUNT_DOWN_LATCH_H_
