#ifndef THREAD_MUTEX_H_
#define THREAD_MUTEX_H_

#include "thread.h"

#include <boost/noncopyable.hpp>
#include <assert.h>
#include <pthread.h>

namespace simple_log {

class MutexLock : public boost::noncopyable {
 public:
  MutexLock() : holder_(0) {
    pthread_mutex_init(&mutex_, NULL);
  }

  ~MutexLock() {
    assert(holder_ == 0);
    pthread_mutex_destroy(&mutex_);
  }

  bool isLockedByThisThread() {
    return holder_ == CurrentThread::tid();
  }

  void assertLocked() {
    assert(isLockedByThisThread());
  }

  void lock() {
    pthread_mutex_lock(&mutex_);
    holder_ = CurrentThread::tid();
  }

  void unlock() {
    holder_ = 0;
    pthread_mutex_unlock(&mutex_);
  }

  pthread_mutex_t* getPhtreadMutex() {
    return &mutex_;
  }
 private:
  pthread_mutex_t mutex_;
  pid_t holder_;
}; // class MutexLock

class MutexLockGuard : public boost::noncopyable {
 public:
  explicit MutexLockGuard(MutexLock& mu) : mutex_(mu) {
    mutex_.lock();
  }

  ~MutexLockGuard() {
    mutex_.unlock();
  }

 private:
   MutexLock& mutex_;
}; // class MutexLockGuard

#define MutexLockGuard(x) error "Missing guard object name"

} // namespace simple_log

#endif // THREAD_MUTEX_H_
