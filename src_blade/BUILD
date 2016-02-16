cc_library(
   name='simple_log',
   srcs=[
     'log_stream.cc',
     'time_stamp.cc',
   ],
   deps=[
     '//thirdparty/gflags:gflags',
     '//thirdparty/glog:glog',
     '#rt',
   ],
)

cc_test(
    name = 'simple_log_test',
    srcs = [
        'log_stream_test.cc',
        'time_stamp_test.cc',
    ],
    deps = [
        ':simple_log',
    ],
)

cc_test(
    name = 'log_stream_bench',
    srcs = [
        'log_stream_bench.cc',
    ],
    deps = [
        ':simple_log',
    ],
)

