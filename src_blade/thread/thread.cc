#include "thread.h"

#include <boost/weak_ptr.hpp>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

namespace simple_log {

__thread pid_t t_cachedTid = 0;

pid_t gettid() {
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

namespace CurrentThread {

__thread const char* t_threadName = "unknown";

pid_t tid() {
  if (t_cachedTid == 0) {
    t_cachedTid = gettid();
  }
  return t_cachedTid;
}

const char* name() {
  return t_threadName;
}

bool isMainThread() {
  return tid() == ::getpid();
}

} // namespace CurrentThread

void afterFork() {
  t_cachedTid = gettid();
  //printf("aflterFork\n");
  CurrentThread::t_threadName = "main";
}

class ThreadNameInitializer {
 public:
  ThreadNameInitializer() {
    //printf("ThreadNameInitializer\n");
    CurrentThread::t_threadName = "main";
    pthread_atfork(NULL, NULL, &afterFork);
  }
}; // class ThreadNameInitializer

ThreadNameInitializer init;

struct ThreadData {
  typedef Thread::ThreadFunc ThreadFunc;
  ThreadFunc func_;
  std::string name_;
  boost::weak_ptr<pid_t> wkTid_;

  ThreadData(const ThreadFunc& f, const std::string& name, const boost::shared_ptr<pid_t> tid)
    : func_(f),
      name_(name),
      wkTid_(tid) {
  }

  void runInThread() {
    pid_t tid = CurrentThread::tid();
    boost::shared_ptr<pid_t> ptid = wkTid_.lock();

    if (ptid) {
      *ptid = tid;
      ptid.reset();
    }

    CurrentThread::t_threadName = name_.c_str();
    func_();
    CurrentThread::t_threadName = "finished";
  }
};

void* startThread(void* obj) {
  ThreadData* data = static_cast<ThreadData*>(obj);
  data->runInThread();
  delete data;
  return NULL;
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(const ThreadFunc& f, const std::string& n)
  : started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(new pid_t(0)),
    func_(f),
    name_(n) {
  numCreated_.increment();
}

Thread::~Thread() {
  if (started_ && !joined_) {
    pthread_detach(pthreadId_);
  }
}

void Thread::start() {
  assert(!started_);
  started_ = true;

  ThreadData* data = new ThreadData(func_, name_, tid_);
  if (pthread_create(&pthreadId_, NULL, &startThread, data)) {
    started_ = false;
    delete data;
    abort();
  }
}

void Thread::join() {
  assert(started_);
  assert(!joined_);
  joined_ = true;
  pthread_join(pthreadId_, NULL);
}


} // namespace simple_log
