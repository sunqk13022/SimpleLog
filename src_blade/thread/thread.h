#ifndef THREAD_THREAD_H_
#define THREAD_THREAD_H_

#include <assert.h>
#include <string.h>
#include <string>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <pthread.h>

#include "atomic.h"

namespace simple_log {

class Thread : public boost::noncopyable {
 public:
  typedef boost::function<void ()> ThreadFunc;

  explicit Thread(const ThreadFunc& f, const std::string& name = std::string());
  ~Thread();

  void start();
  void join();

  bool started() const { return started_; }
  pid_t tid() const { return *tid_; }
  const std::string& name() const { return name_; }

  static int numCreated() { return numCreated_.get(); }

 private:
  bool      started_;
  bool      joined_;
  pthread_t pthreadId_;
  boost::shared_ptr<pid_t> tid_;
  ThreadFunc func_;
  std::string name_;

  static AtomicInt32 numCreated_;
}; // class Thread

namespace CurrentThread {

pid_t tid();
const char* name();
bool isMainThread();

} // namespace CurrentThread

} // namespace simple_log

#endif // THREAD_THREAD_H_
