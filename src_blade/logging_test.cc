#include "thirdparty/gtest/gtest.h"
#include "thirdparty/glog/logging.h"

#include "logging.h"
#include "log_file.h"
#include "time_stamp.h"

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

boost::scoped_ptr<LogFile> g_logFile;

void outputFunc(const char* msg, int len) {
  g_logFile->append(msg, len);
}

void flushFunc() {
  g_logFile->flush();
}

TEST(Logging, File) {
  //FLAGS_logtostderr = true;
  int N = 10;
  g_logFile.reset(new LogFile("tttt", 256*1024, true));
  Logger::setOutput(outputFunc);
  Logger::setFlush(flushFunc);

  //LOG(ERROR) << "SUCCEED";
  std::string line = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  for (int i = 0; i < N; ++i) {
    LOG_INFO << line << "\t" << i;
  }
  //LOG(ERROR) << "SUCCEED";
  for (int i = 0; i < N; ++i) {
    //LOG(INFO) << line << "\t" << i;
  }
  //LOG(ERROR) << "SUCCEED";
}

long  g_total;
FILE* g_file;

void dummyOutput(const char* msg, int len) {
  g_total += len;
  if (g_file) {
    fwrite(msg, 1, len, g_file);
  } else if (g_logFile) {
    g_logFile->append(msg, len);
  }
}

void bench() {
  Logger::setOutput(dummyOutput);
  Timestamp start(Timestamp::now());
  g_total = 0;

  const int batch = 1000 * 1;
  std::string longStr(3000, 'X');

  longStr += " ";
  for (int i = 0; i < batch; ++i) {
    LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz "
      << longStr
      << i;
  }

  Timestamp end(Timestamp::now());
  double seconds = timeDifference(end, start);
  printf("%f seconds, %ld bytes, %.2f msg/s, %.2f MiB/s\n",
         seconds, g_total, batch / seconds, g_total / seconds / 1024 / 1024);
}

TEST(Logging, Bench) {
  getppid();

  bench();

  char buffer[64*1024];
  g_file = fopen("/dev/null", "w");
  setbuffer(g_file, buffer, sizeof(buffer));
  bench();
  fclose(g_file);

  //char buffer[64*1024];
//  g_file = fopen("/da1/sunqiankun/bench", "w");
//  setbuffer(g_file, buffer, sizeof(buffer));
//  bench();
//  fclose(g_file);

  g_file = NULL;
  g_logFile.reset(new LogFile("test_log", 500*1000*1000));
  bench();

  g_logFile.reset(new LogFile("test_log_mt", 500*1000*1000, true));
  bench();

  g_logFile.reset();
}

} // namespace simple_log
