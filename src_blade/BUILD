cc_library(
   name='simple_log',
   srcs=[
     'log_stream.cc',
     'time_stamp.cc',
     'logging.cc',
     'log_file.cc',
   ],
   deps=[
     '//thirdparty/gflags:gflags',
     '//thirdparty/glog:glog',
     '//simple_log/thread:simple_thread',
     '#rt',
   ],
)

cc_test(
    name = 'simple_log_test',
    srcs = [
        #'log_stream_test.cc',
        #'time_stamp_test.cc',
        #'logging_test.cc',
        'async_logging_test.cc',
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

cc_binary(
   name='ptr_contain_test',
   srcs=[
     'ptr_contain_test.cc',
   ],
   deps=[
     '//thirdparty/gflags:gflags',
     '//thirdparty/glog:glog',
     '#rt',
   ],
)
