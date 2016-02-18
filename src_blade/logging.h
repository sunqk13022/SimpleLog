#ifndef SIMPLELOG_LOGGING_H_
#define SIMPLELOG_LOGGING_H_

#include <boost/scoped_ptr.hpp>
#include "time_stamp.h"
#include "log_stream.h"

namespace simple_log {

class Logger {
 public:
  enum LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
  };

  Logger(const char* file, int line);
  Logger(const char* file, int line, LogLevel level);
  Logger(const char* file, int line, LogLevel level, const char* func);
  Logger(const char* file, int line, bool toAbort);

  ~Logger();

  LogStream& stream() { return impl_.stream_; }

  static LogLevel logLevel();
  static void setLogLevel(LogLevel level);

  typedef void (*OutputFunc) (const char* msg, int len);
  typedef void (*FlushFunc) ();

  static void setOutput(OutputFunc);
  static void setFlush(FlushFunc);

 private:

  class Impl {
   public:
    typedef Logger::LogLevel LogLevel;
    Impl(LogLevel level, int old_error, const char* file, int line);

    void formatTime();
    void finish();

    Timestamp time_;
    LogStream stream_;
    LogLevel level_;
    int line_;
    const char* fullname_;
    const char* basename_;
  }; // class Impl

  Impl impl_;
}; // class Logger

#define LOG_TRACE if (simple_log::Logger::logLevel() <= simple_log::Logger::TRACE) \
  simple_log::Logger(__FILE__, __LINE__, simple_log::Logger::TRACE, __FUNCTION__).stream()

#define LOG_DEBUG if (simple_log::Logger::logLevel() <= simple_log::Logger::DEBUG) \
  simple_log::Logger(__FILE__, __LINE__, simple_log::Logger::DEBUG, __FUNCTION__).stream()

#define LOG_INFO if (simple_log::Logger::logLevel() <= simple_log::Logger::INFO) \
  simple_log::Logger(__FILE__, __LINE__).stream()

#define LOG_WARN simple_log::Logger(__FILE__, __LINE__, simple_log::Logger::WARN).stream()
#define LOG_ERROR simple_log::Logger(__FILE__, __LINE__, simple_log::Logger::ERROR).stream()
#define LOG_FATAL simple_log::Logger(__FILE__, __LINE__, simple_log::Logger::FATAL).stream()
#define LOG_SYSERR simple_log::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL simple_log::Logger(__FILE__, __LINE__, true).stream()

const char* strerror_tl(int savedErrno);

#define CHECK_NOTNULL(val) \
  simple_log::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be not NULL". (val))

template<typename T>
T* CheckNotNull(const char* file, int line, const char* names, T* ptr) {
  if (ptr == NULL) {
    Logger(file, line, Logger::FATAL).stream() << names;
  }
  return ptr;
}

template<typename To, typename From>
inline To implicit_cast(From const &f) {
  return f;
}

} // namespace simple_log

#endif // SIMPLELOG_LOGGING_H_
