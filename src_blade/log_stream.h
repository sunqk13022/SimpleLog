#ifndef SIMPLELOG_LOG_STREAM_H
#define SIMPLELOG_LOG_STREAM_H

#include <assert.h>
#include <string.h>
#include <string>
#include <boost/noncopyable.hpp>

namespace simple_log {

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

template<int SIZE>
class FixedBuffer : public boost::noncopyable {
 public:
  FixedBuffer() : cur_(data_) {
    setCookie(cookieStart);
  }

  ~FixedBuffer() {
    setCookie(cookieEnd);
  }

  void append(const char* buf, int len) {
    if (avail() > len) {
      memcpy(cur_, buf, len);
      cur_ += len;
    }
  }

  const char* data() const { return data_; }
  int length() const { return cur_ - data_; }

  char* current() { return cur_; }
  int avail() const { return static_cast<int>(end() - cur_); }
  void add(size_t len) { cur_ += len; }

  void reset() { cur_ = data_; }
  void bzero() { ::bzero(data_, sizeof(data_)); }

  const char* debugString();
  void setCookie(void (*cookie)()) { cookie_ = cookie; }
  std::string asString() const { return std::string(data_, length()); }
 private:
  const char* end() const { return data_ + sizeof(data_); }

  static void cookieStart();
  static void cookieEnd();

  void  (*cookie_)();
  char  data_[SIZE];
  char* cur_;
}; // class FixedBuffer

class T {
 public:
  T(const char* str, int len)
   :  str_(str),
     len_(len) {
    assert(strlen(str_) == len_);
  }

  const char* str_;
  const size_t len_;
}; // class T

class LogStream : public boost::noncopyable {
 public:
  typedef FixedBuffer<kSmallBuffer> Buffer;

  LogStream& operator << (bool v) {
    buffer_.append(v ? "1": "0", 1);
    return *this;
  }

  LogStream& operator << (short);
  LogStream& operator << (unsigned short);
  LogStream& operator << (int);
  LogStream& operator << (unsigned int);
  LogStream& operator << (long);
  LogStream& operator << (unsigned long);
  LogStream& operator << (long long);
  LogStream& operator << (unsigned long long);

  LogStream& operator << (const void*);

  LogStream& operator << (float v) {
    *this << static_cast<double>(v);
    return *this;
  }

  LogStream& operator << (double);

  LogStream& operator << (char v) {
    buffer_.append(&v, 1);
    return *this;
  }

  LogStream& operator << (const char* v) {
    buffer_.append(v, strlen(v));
    return *this;
  }

  LogStream& operator << (const T& v) {
    buffer_.append(v.str_, v.len_);
    return *this;
  }

  LogStream& operator << (const std::string& v) {
    buffer_.append(v.c_str(), v.size());
    return *this;
  }

  void append(const char* data, int len) { buffer_.append(data, len); }
  const Buffer& buffer() const { return buffer_; }
  void resetBuffer() { buffer_.reset(); }

 private:
  void staticCheck();

  template<typename T>
  void formatInteger(T);

  Buffer buffer_;
  static const int kMaxNumericSize = 32;
}; // class LogStream

class Fmt {
 public:
  template<typename T>
  Fmt(const char* fmt, T val);

  const char* data() const { return buf_; }
  int length() const { return length_; }
 private:
  char buf_[32];
  int length_;
}; // class Fmt

inline LogStream& operator<< (LogStream& s, const Fmt& fmt) {
  s.append(fmt.data(), fmt.length());
  return s;
}

} // namespace simple_log

#endif // SIMPLELOG_LOG_STREAM_H
