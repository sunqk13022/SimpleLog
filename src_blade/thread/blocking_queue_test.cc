#include "thirdparty/gtest/gtest.h"
#include "thirdparty/glog/logging.h"

#include "thread.h"

namespace simple_log {

TEST(ThreadTest, New) {
  LOG(ERROR) << "tid=" << CurrentThread::tid() << ", name=" << CurrentThread::name() << ",ismain=" << CurrentThread::isMainThread();
}


} // namespace simple_log
