#include "thirdparty/gtest/gtest.h"
#include "thirdparty/glog/logging.h"

#include "time_stamp.h"
#include <stdio.h>
#include <vector>

namespace simple_log {

TEST(Timestamp, Benchmack) {
  Timestamp now(Timestamp::now());
  printf("%s\n", now.toString().c_str());

  const int kNumber = 1000 * 1000;

  std::vector<Timestamp> stamps;
  stamps.reserve(kNumber);
  for (int i = 0; i < kNumber; ++i) {
    stamps.push_back(Timestamp::now());
  }

  printf("%s\n", stamps.front().toString().c_str());
  printf("%s\n", stamps.back().toString().c_str());
  printf("%f\n", timeDifference(stamps.back(), stamps.front()));

  int increments[100] = { 0 };
  int64_t start = stamps.front().microSecondsSinceEpoch();
  for (int i = 1; i < kNumber; ++i) {
    int64_t next = stamps.at(i).microSecondsSinceEpoch();
    int inc = next - start;
    if (inc < 0) {
      printf("reverse!!!!!!!!!!!!!!!!\n");
    } else if (inc < 100) {
      ++increments[inc];
    } else {
      ++increments[99];
    }
  }

  for (int i = 0; i < 100; ++i) {
    //printf("%02d: %d\n", i, increments[i]);
  }
}

} // namespace simple_log
