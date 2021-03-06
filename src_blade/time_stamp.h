#ifndef SIMPLE_LOG_TIME_STAMP_H
#define SIMPLE_LOG_TIME_STAMP_H

#include <stdint.h>
#include <string>
#include <algorithm>

namespace simple_log {

class Timestamp {
 public:
  Timestamp();

  explicit Timestamp(int64_t microSecondsSinceEpoch);

  void swap(Timestamp& that) {
    std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
  }

  std::string toString() const;
  std::string toFormattedString() const;

  bool valid() const { return microSecondsSinceEpoch_ > 0; }

  int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }

  static Timestamp now();

  static Timestamp invalid();

  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  int64_t microSecondsSinceEpoch_;
}; // class Timestamp

inline bool operator < (Timestamp lhs, Timestamp rhs) {
  return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator == (Timestamp lhs, Timestamp rhs) {
  return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

inline double timeDifference(Timestamp high, Timestamp low) {
  int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
  return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

inline Timestamp addTime(Timestamp timestamp, double seconds) {
  int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

} // namespace simple_log

#endif // SIMPLE_LOG_TIME_STAMP_H
