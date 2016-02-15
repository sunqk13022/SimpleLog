cc_library(
   name='simple_log',
   srcs=[
     'log_stream.cc',
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
    ],
    deps = [
        ':simple_log',
    ],
)

