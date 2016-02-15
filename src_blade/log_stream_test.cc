#include "thirdparty/gtest/gtest.h"
#include "thirdparty/glog/logging.h"

#include "log_stream.h"
#include <limits>

namespace simple_log {

TEST(LogStream, BooleanTest) {
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();

  EXPECT_EQ(buf.asString(), std::string(""));

  os << true;
  EXPECT_EQ(buf.asString(), std::string("1"));

  os << '\n';
  EXPECT_EQ(buf.asString(), std::string("1\n"));

  os << false;
  EXPECT_EQ(buf.asString(), std::string("1\n0"));
}

TEST(LogStream, IntergerTest) {
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();

  EXPECT_EQ(buf.asString(), std::string(""));

  os << 1;
  EXPECT_EQ(buf.asString(), std::string("1"));

  os << 0;
  EXPECT_EQ(buf.asString(), std::string("10"));

  os << -1;
  EXPECT_EQ(buf.asString(), std::string("10-1"));

  os.resetBuffer();
  EXPECT_EQ(buf.asString(), std::string(""));

  os << 0 << " " << 123 << 'x' << 0x64;
  EXPECT_EQ(buf.asString(), std::string("0 123x100"));
}

TEST(LogStream, LimitTest) {
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();

  os << -2147483647;
  EXPECT_EQ(buf.asString(), std::string("-2147483647"));

  os << (int)-2147483648;
  EXPECT_EQ(buf.asString(), std::string("-2147483647-2147483648"));

  os << ' ';
  os << 2147483647;
  EXPECT_EQ(buf.asString(), std::string("-2147483647-2147483648 2147483647"));

  os.resetBuffer();
  os << std::numeric_limits<int16_t>::min();
  EXPECT_EQ(buf.asString(), std::string("-32768"));

  os.resetBuffer();
  os << std::numeric_limits<int16_t>::max();
  EXPECT_EQ(buf.asString(), std::string("32767"));

  os.resetBuffer();
  os << std::numeric_limits<unsigned int16_t>::min();
  EXPECT_EQ(buf.asString(), std::string("0"));

  os.resetBuffer();
  os << std::numeric_limits<unsigned int16_t>::max();
  EXPECT_EQ(buf.asString(), std::string("65535"));

  os.resetBuffer();
  os << std::numeric_limits<int32_t>::max();
  EXPECT_EQ(buf.asString(), std::string("2147483647"));

  os.resetBuffer();
  os << std::numeric_limits<int32_t>::min();
  EXPECT_EQ(buf.asString(), std::string("-2147483648"));

  os.resetBuffer();
  os << std::numeric_limits<unsigned int32_t>::min();
  EXPECT_EQ(buf.asString(), std::string("0"));

  os.resetBuffer();
  os << std::numeric_limits<unsigned int32_t>::max();
  EXPECT_EQ(buf.asString(), std::string("4294967295"));

  os.resetBuffer();
  os << std::numeric_limits<int64_t>::max();
  EXPECT_EQ(buf.asString(), std::string("9223372036854775807"));

  os.resetBuffer();
  os << std::numeric_limits<int64_t>::min();
  EXPECT_EQ(buf.asString(), std::string("-9223372036854775808"));

  os.resetBuffer();
  os << std::numeric_limits<unsigned int64_t>::min();
  EXPECT_EQ(buf.asString(), std::string("0"));

  os.resetBuffer();
  os << std::numeric_limits<unsigned int64_t>::max();
  EXPECT_EQ(buf.asString(), std::string("18446744073709551615"));

  os.resetBuffer();
  int16_t a = 0;
  int32_t b = 0;
  int64_t c = 0;
  os << a << b << c;
  EXPECT_EQ(buf.asString(), std::string("000"));
}

TEST(LogStream, FloatTest) {
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();
  EXPECT_EQ(buf.asString(), std::string(""));

  os << 0.0;
  EXPECT_EQ(buf.asString(), std::string("0"));

  os.resetBuffer();
  os << 1.0;
  EXPECT_EQ(buf.asString(), std::string("1"));

  os.resetBuffer();
  os << 0.1;
  EXPECT_EQ(buf.asString(), std::string("0.1"));

  os.resetBuffer();
  os << 0.05;
  EXPECT_EQ(buf.asString(), std::string("0.05"));

  os.resetBuffer();
  os << 0.15;
  EXPECT_EQ(buf.asString(), std::string("0.15"));

  os.resetBuffer();
  os << (double)0.1;
  EXPECT_EQ(buf.asString(), std::string("0.1"));

  os.resetBuffer();
  os << (double)0.05;
  EXPECT_EQ(buf.asString(), std::string("0.05"));

  os.resetBuffer();
  os << (double)0.15;
  EXPECT_EQ(buf.asString(), std::string("0.15"));

  EXPECT_NE((double)0.05 + (double)0.1, (double)0.15);

  os.resetBuffer();
  os << 1.23456789;
  EXPECT_EQ(buf.asString(), std::string("1.23456789"));

  os.resetBuffer();
  os << -123.456;
  EXPECT_EQ(buf.asString(), std::string("-123.456"));
}

TEST(LogStream, VoidTest) {
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();

  os << static_cast<void*>(0);
  EXPECT_EQ(buf.asString(), std::string("0x0"));

  os.resetBuffer();
  os << reinterpret_cast<void*>(8888);
  EXPECT_EQ(buf.asString(), std::string("0x22B8"));
}

TEST(LogStream, StringTest) {
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();

  os << "Hello ";
  EXPECT_EQ(buf.asString(), std::string("Hello "));

  os.resetBuffer();
  os << std::string("gege");
  EXPECT_EQ(buf.asString(), std::string("gege"));
}

TEST(LogStream, FmtTest) {
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();

  os << Fmt("%4d", 1);
  EXPECT_EQ(buf.asString(), std::string("   1"));

  os.resetBuffer();
  os << Fmt("%4.2f",1.2);
  EXPECT_EQ(buf.asString(), std::string("1.20"));

  os.resetBuffer();
  os << Fmt("%4.2f",1.2) << Fmt("%4d", 43);
  EXPECT_EQ(buf.asString(), std::string("1.20  43"));
}

TEST(LogStream, LengthTest) {
  LogStream os;
  const LogStream::Buffer& buf = os.buffer();

  for (int i = 0; i < 399; ++i) {
    os << "123456789 ";
    EXPECT_EQ(buf.length(), 10*(i + 1));
    EXPECT_EQ(buf.avail(), 4000 - 10*(i + 1));
  }

  //os.resetBuffer();
  os << "abcdefghi ";
  EXPECT_EQ(buf.length(), 3990);
  EXPECT_EQ(buf.avail(), 10);

  os << "abcdefghi";
  EXPECT_EQ(buf.length(), 3999);
  EXPECT_EQ(buf.avail(), 1);
}

} // namespace simple_log
