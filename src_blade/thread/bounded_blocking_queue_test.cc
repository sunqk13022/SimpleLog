#include "thirdparty/gtest/gtest.h"
#include "thirdparty/glog/logging.h"

#include "thread.h"
#include "bounded_blocking_queue.h"
#include "count_down_latch.h"

#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <string>
#include <stdio.h>

namespace simple_log {

class QueueServer2 {
 public:
  QueueServer2(int numThreads)
    : latch_(numThreads),
      queue_(20),
      threads_(numThreads)
  {
    for (int i = 0; i < numThreads; ++i) {
      char name[32];
      snprintf(name, sizeof(name), "work thread %d", i);
      threads_.push_back(new Thread(
          boost::bind(&QueueServer2::threadFunc, this), std::string(name)
      ));
    }

    for_each(threads_.begin(), threads_.end(), boost::bind(&Thread::start, _1));
  }

  void run(int times) {
    printf("waiting for count down latch\n");
    latch_.wait();
    printf("all threads started\n");

    for (int i = 0; i < times; ++i) {
      char buf[32];
      snprintf(buf, sizeof(buf), "hello %d", i);
      queue_.put(buf);
      printf("tid=%d, put data=%s, size = %zd\n", CurrentThread::tid(), buf, queue_.size());
    }
  }

  void joinAll() {
    for (int i = 0; i < threads_.size(); ++i) {
      queue_.put("stop");
    }
    for_each(threads_.begin(), threads_.end(), boost::bind(&Thread::join, _1));
  }

 private:

  void threadFunc() {
    printf("tid=%d, %s started\n", CurrentThread::tid(), CurrentThread::name());
    latch_.countDown();

    bool running = true;
    while (running) {
      std::string d(queue_.take());
      printf("tid=%d, get data=%s, size=%zd\n", CurrentThread::tid(), d.c_str(), queue_.size());
      running = (d != "stop");
    }
    printf("tid=%d, %s stop\n", CurrentThread::tid(), CurrentThread::name());
  }

  CountDownLatch             latch_;
  BoundedBlockingQueue<std::string> queue_;
  boost::ptr_vector<Thread> threads_;
}; // class QueueServer2

TEST(BoundBlockingQueue, Test) {
  printf("pid=%d, tid=%d\n", getpid(), CurrentThread::tid());
  QueueServer2 server(5);
  server.run(100);
  server.joinAll();

  printf("number of created threads %d\n", Thread::numCreated());
}


} // namespace simple_log
