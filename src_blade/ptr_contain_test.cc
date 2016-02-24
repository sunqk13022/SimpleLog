#include "thirdparty/gtest/gtest.h"
#include "thirdparty/glog/logging.h"

#include <stdio.h>
#include <vector>
#include <iostream>
#include <sys/time.h>
#include <vector>

#include <boost/ptr_container/ptr_vector.hpp>

uint64_t GetTimeStampInUs() {
  struct timeval tv = {0, 0};
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

class Buffer {
 public:
  Buffer() {
    memset(data_, 'X', sizeof(data_));
  }

  char data_[1024];
}; // class Buffer

int main() {
  int kCount = 1000 * 10;
  {
    int64_t start_t = GetTimeStampInUs();
    boost::ptr_vector<Buffer> contaner;
    for (int i = 0; i < kCount; ++i) {
      boost::ptr_vector<Buffer>::auto_type tempobj(new Buffer());
      contaner.push_back(tempobj.release());
    }
    int64_t end_t = GetTimeStampInUs();
    std::cout << "ptr_contain use time = " << end_t - start_t <<"US, count = " << kCount << "\n";
  }
  {
    int64_t start_t = GetTimeStampInUs();
    boost::ptr_vector<Buffer> contaner;
    boost::ptr_vector<Buffer>::auto_type current(new Buffer());
    boost::ptr_vector<Buffer>::auto_type next(new Buffer());
    contaner.reserve(kCount);
    for (int i = 0; i < kCount; ++i) {
      contaner.push_back(current.release());
      if (current) {
        std::cout << "current must be NULL\n";
      }
      if (!next) {
        std::cout << "next must not be NULL\n";
      }
      current = boost::ptr_container::move(next);
      if (next) {
        std::cout << "next must  be NULL\n";
      }
      next.reset(new Buffer());
    }
    int64_t end_t = GetTimeStampInUs();
    std::cout << "Move time = " << end_t - start_t <<"US, count = " << kCount << "\n";
  }
  {
    int64_t start_t = GetTimeStampInUs();
    std::vector<Buffer*> contaner;
    for (int i = 0; i < kCount; ++i) {
      contaner.push_back(new Buffer());
    }
    for (int i = 0; i < kCount; ++i) {
      delete contaner[i];
    }
    int64_t end_t = GetTimeStampInUs();
    std::cout << "vector use time = " << end_t - start_t <<"US, count = " << kCount << "\n";
  }
}
