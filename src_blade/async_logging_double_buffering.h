#ifndef SIMPLE_LOG_ASYNC_LOGGING_DOUBLE_BUFFERING_H_
#define SIMPLE_LOG_ASYNC_LOGGING_DOUBLE_BUFFERING_H_

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "log_stream.h"
#include "thread/blocking_queue.h"
#include "thread/bounded_blocking_queue.h"
#include "thread/count_down_latch.h"
#include "thread/mutex.h"
#include "thread/thread.h"

namespace simple_log {

class AsyncLoggingDoubleBuffering: public boost::noncopyable {
 public:

 private:

  const int      flushInterval_;
  bool           running_;
  std::string    basename_;
  size_t         rollSize_;
  Thread         thread_;
  CountDownLatch latch_;
  MutexLock      mutex_;
  Condition      cond_;
  BufferPtr      currentBuffer_;
  BufferPtr      nextBuffer_;
  BufferVector   buffers_;
}; // class AsyncLoggingDoubleBuffering

} // namespace simple_log

#endif // SIMPLE_LOG_ASYNC_LOGGING_DOUBLE_BUFFERING_H_
