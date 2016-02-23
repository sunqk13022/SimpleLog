#include "thirdparty/gtest/gtest.h"
#include "thirdparty/glog/logging.h"

#include <stdio.h>
#include <sys/resource.h>

#include "time_stamp.h"
#include "logging.h"
#include "async_logging_queue.h"

namespace simple_log {

int kRollSize = 500 * 1000 * 1000;

void* g_asyncLog = NULL;

template<typename ASYNC>
void asyncOutput(const char* msg, int len) {
  ASYNC* log = static_cast<ASYNC*>(g_asyncLog);
  log->append(msg, len);
}

template<typename ASYNC>
void bench(ASYNC* log) {
  g_asyncLog = log;
  Logger::setOutput(asyncOutput<ASYNC>);

  int cnt = 0;
  const int kBatch = 1000;
  const bool kLongLog = true;
  std::string empty = "";
  std::string logStr(3000, 'X');
  logStr += " ";

  for (int t = 0; t < 30; ++t) {
    Timestamp start = Timestamp::now();
    for (int i = 0; i < kBatch; ++i) {
      LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz "
        << logStr
        << cnt;
      ++cnt;
    }
    Timestamp end = Timestamp::now();
    printf("%f\n", timeDifference(end, start) * 1000000/kBatch);
    struct timespec ts = { 0, 500 * 1000 * 1000 };
    nanosleep(&ts, NULL);
  }
}

TEST(AsyncLoggingQueue, UnboundedQueue) {
  {
    size_t kOneGB = 1024 * 1024 * 1024;
    rlimit r1 = { 2.0 * kOneGB, 2.0 * kOneGB };
    setrlimit(RLIMIT_AS, &r1);
  }

  AsyncLoggingUnboundedQueue log1("log1", kRollSize);
  //AsyncLoggingboundedQueue log2("log2", kRollSize, 1024);
  log1.start();
  bench(&log1);
  log1.stop();
}

TEST(AsyncLoggingQueue, boundedQueue) {
  {
    size_t kOneGB = 1024 * 1024 * 1024;
    rlimit r1 = { 2.0 * kOneGB, 2.0 * kOneGB };
    setrlimit(RLIMIT_AS, &r1);
  }

  AsyncLoggingboundedQueue log2("log2", kRollSize, 1024);
  log2.start();
  bench(&log2);
  log2.stop();
}


TEST(AsyncLoggingQueue, boundedQueueL) {
  {
    size_t kOneGB = 1024 * 1024 * 1024;
    rlimit r1 = { 2.0 * kOneGB, 2.0 * kOneGB };
    setrlimit(RLIMIT_AS, &r1);
  }

  AsyncLoggingboundedQueueL log2("log4", kRollSize, 1024);
  log2.start();
  bench(&log2);
  log2.stop();
}
TEST(AsyncLoggingQueue, UnboundedQueueL) {
  {
    size_t kOneGB = 1024 * 1024 * 1024;
    rlimit r1 = { 2.0 * kOneGB, 2.0 * kOneGB };
    setrlimit(RLIMIT_AS, &r1);
  }

  AsyncLoggingUnboundedQueueL log2("log3", kRollSize);
  log2.start();
  bench(&log2);
  log2.stop();
}

} // namespace simple_log
