#include "thirdparty/gtest/gtest.h"
#include "thirdparty/glog/logging.h"

#include "log_stream.h"
#include "time_stamp.h"
#include <limits>
#include <sstream>

namespace simple_log {

//const int N = 1000*1000;
const int N = 1000;

template<typename T>
void benchPrintf(const char* fmt) {
  char buf[32];
  Timestamp start(Timestamp::now());

  for (int i = 0; i < N; ++i) {
    snprintf(buf, sizeof(buf), fmt, (T)(i));
  }

  Timestamp end(Timestamp::now());
  printf("benchPrintf use time=%f\n", timeDifference(end, start));
}

template<typename T>
void benchStringStream() {
  Timestamp start(Timestamp::now());

  std::ostringstream os;
  for (int i = 0; i < N; ++i) {
    os << (T)(i);
    os.seekp(0, std::ios_base::beg);
  }

  Timestamp end(Timestamp::now());
  printf("benchStringStream use time=%f\n", timeDifference(end, start));
}

template<typename T>
void benchLogStream() {
  Timestamp start(Timestamp::now());

  LogStream os;
  for (int i = 0; i < N; ++i) {
    os << (T)(i);
    os.resetBuffer();
  }

  Timestamp end(Timestamp::now());
  printf("benchLogStream use time=%f\n", timeDifference(end, start));
}

TEST(LogStreamBench, Int) {
  LOG(ERROR) << "bench int";
  benchPrintf<int>("%d");
  benchStringStream<int>();
  benchLogStream<int>();

  LOG(ERROR) << "bench double";
  benchPrintf<double>("%.12g");
  benchStringStream<double>();
  benchLogStream<double>();

  LOG(ERROR) << "bench int64_t";
  benchPrintf<int64_t>("%ld");
  benchStringStream<int64_t>();
  benchLogStream<int64_t>();

  LOG(ERROR) << "bench void*";
  benchPrintf<void*>("%p");
  benchStringStream<void*>();
  benchLogStream<void*>();

}

} // namespace simple_log
