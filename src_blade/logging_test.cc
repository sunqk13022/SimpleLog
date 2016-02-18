#include "thirdparty/gtest/gtest.h"
#include "thirdparty/glog/logging.h"

#include "logging.h"

namespace simple_log {

TEST(Logging, Stdcout) {
  FLAGS_logtostderr = true;
  int N = 10;
  //LOG(ERROR) << "SUCCEED";
  std::string line = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  for (int i = 0; i < N; ++i) {
    //LOG_INFO << line << "\t" << i;
  }
  //LOG(ERROR) << "SUCCEED";
  for (int i = 0; i < N; ++i) {
    //LOG(INFO) << line << "\t" << i;
  }
  //LOG(ERROR) << "SUCCEED";
}

} // namespace simple_log
