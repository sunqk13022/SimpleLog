cc_library(
   name='simple_thread',
   srcs=[
     'thread.cc',
   ],
   deps=[
     '//thirdparty/gflags:gflags',
     '//thirdparty/glog:glog',
     '#rt',
   ],
)

cc_test(
    name = 'thread_test',
    srcs = [
        'thread_test.cc',
        'blocking_queue_test.cc',
        'bounded_blocking_queue_test.cc',
    ],
    deps = [
        ':simple_thread',
    ],
)


