#ifndef SIMPLE_LOG_FILE_H_
#define SIMPLE_LOG_FILE_H_

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "thread/mutex.h"

namespace simple_log {

class LogFile : public boost::noncopyable {
 public:
  LogFile(const std::string& basename, size_t rollSize, bool threadSafe = false, int flushInteral = 3);
  ~LogFile();

  void append(const char* logline, int len);
  void flush();

 private:

  void append_unlocked(const char* logline, int len);

  static std::string getLogFileName(const std::string& basename, time_t* now);
  void rollFile();

  const std::string basename_;
  const size_t      rollSize_;
  const int         flushInterval_;

  int               count_;

  boost::scoped_ptr<MutexLock> mutex_;
  time_t            startOfPeriod_;
  time_t            lastRoll_;
  time_t            lastFlush_;
  class File;
  boost::scoped_ptr<File> file_;

  const static int kCheckTimeRoll_ = 1024;
  const static int kRollPerSeconds_ = 60 * 60 * 24;
}; // class LogFile

} // namespace simple_log

#endif // SIMPLE_LOG_FILE_H_
