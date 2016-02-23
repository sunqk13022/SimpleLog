#ifndef SIMPLE_LOG_ASYNC_LOGGING_QUEUE_H_
#define SIMPLE_LOG_ASYNC_LOGGING_QUEUE_H_

#include "log_file.h"

#include "thread/blocking_queue.h"
#include "thread/bounded_blocking_queue.h"
#include "thread/count_down_latch.h"
#include "thread/thread.h"

#include <string>
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace simple_log {

template<typename MSG, template <typename> class QUEUE>
class AsyncLoggingT : public boost::noncopyable {
 public:
  AsyncLoggingT(const std::string basename, size_t rollSize)
    : running_(false),
      basename_(basename),
      rollSize_(rollSize),
      thread_(boost::bind(&AsyncLoggingT::threadFunc, this), "Logging"),
      latch_(1)
  {
  }

  AsyncLoggingT(const std::string basename, size_t rollSize, int queueSize)
    : running_(false),
      basename_(basename),
      rollSize_(rollSize),
      thread_(boost::bind(&AsyncLoggingT::threadFunc, this), "Logging"),
      latch_(1),
      queue_(queueSize)
  {
  }

  ~AsyncLoggingT() {
    if (running_) {
      stop();
    }
  }

  void append(const char* line, int len) {
    queue_.put(MSG(line, len));
  }

  void start() {
    running_ = true;
    thread_.start();
    latch_.wait();
  }

  void stop() {
    running_ = false;
    queue_.put(MSG());
    thread_.join();
  }

 private:

   void threadFunc() {
     assert(running_ == true);
     latch_.countDown();
     LogFile output(basename_, rollSize_, false);

     while (true) {
       MSG msg(queue_.take());
       if (msg.empty()) {
         assert(running_ == false);
         break;
       }
       output.append(msg.data(), msg.length());
     }
     output.flush();
   }

   bool              running_;
   std::string       basename_;
   size_t            rollSize_;
   Thread            thread_;
   CountDownLatch    latch_;
   QUEUE<MSG>        queue_;
}; // class AsyncLoggingT

typedef AsyncLoggingT<std::string, BlockingQueue> AsyncLoggingUnboundedQueue;
typedef AsyncLoggingT<std::string, BoundedBlockingQueue> AsyncLoggingboundedQueue;

struct LogMessage {
  LogMessage(const char* msg, int len) : length_(len) {
    assert(length_ <= sizeof(data_));
    memcpy(data_, msg, length_);
  }

  LogMessage() : length_(0) {}

  LogMessage(const LogMessage& rhs) : length_(rhs.length_) {
    assert(length_ <= sizeof(data_));
    memcpy(data_, rhs.data_, length_);
  }

  LogMessage& operator = (const LogMessage& rhs) {
    length_ = rhs.length_;
    assert(length_ <= sizeof(data_));
    memcpy(data_, rhs.data_, length_);
    return *this;
  }

  const char* data() const { return data_; }
  int length() const { return length_; }
  bool empty() const { return length_ == 0; }

  char   data_[4000];
  size_t length_;
}; // struct LogMessage

typedef AsyncLoggingT<LogMessage, BlockingQueue> AsyncLoggingUnboundedQueueL;
typedef AsyncLoggingT<LogMessage, BoundedBlockingQueue> AsyncLoggingboundedQueueL;

} // namespace simple_log

#endif // SIMPLE_LOG_ASYNC_LOGGING_QUEUE_H_
