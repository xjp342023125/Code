# CMake generated Testfile for 
# Source directory: E:/git/third/libevent
# Build directory: E:/git/third/libevent/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test-changelist__WIN32 "E:/git/third/libevent/build/bin/test-changelist")
set_tests_properties(test-changelist__WIN32 PROPERTIES  ENVIRONMENT "EVENT_SHOW_METHOD=1")
add_test(test-eof__WIN32 "E:/git/third/libevent/build/bin/test-eof")
set_tests_properties(test-eof__WIN32 PROPERTIES  ENVIRONMENT "EVENT_SHOW_METHOD=1")
add_test(test-fdleak__WIN32 "E:/git/third/libevent/build/bin/test-fdleak")
set_tests_properties(test-fdleak__WIN32 PROPERTIES  ENVIRONMENT "EVENT_SHOW_METHOD=1")
add_test(test-init__WIN32 "E:/git/third/libevent/build/bin/test-init")
set_tests_properties(test-init__WIN32 PROPERTIES  ENVIRONMENT "EVENT_SHOW_METHOD=1")
add_test(test-time__WIN32 "E:/git/third/libevent/build/bin/test-time")
set_tests_properties(test-time__WIN32 PROPERTIES  ENVIRONMENT "EVENT_SHOW_METHOD=1")
add_test(test-weof__WIN32 "E:/git/third/libevent/build/bin/test-weof")
set_tests_properties(test-weof__WIN32 PROPERTIES  ENVIRONMENT "EVENT_SHOW_METHOD=1")
add_test(test-dumpevents__WIN32_no_check "E:/git/third/libevent/build/bin/test-dumpevents")
set_tests_properties(test-dumpevents__WIN32_no_check PROPERTIES  ENVIRONMENT "EVENT_SHOW_METHOD=1")
add_test(test-ratelim__group_lim "E:/git/third/libevent/build/bin/test-ratelim" "-g" "30000" "-n" "30" "-t" "100" "--check-grouplimit" "1000" "--check-stddev" "100")
add_test(test-ratelim__con_lim "E:/git/third/libevent/build/bin/test-ratelim" "-c" "1000" "-n" "30" "-t" "100" "--check-connlimit" "50" "--check-stddev" "50")
add_test(test-ratelim__group_con_lim "E:/git/third/libevent/build/bin/test-ratelim" "-c" "1000" "-g" "30000" "-n" "30" "-t" "100" "--check-grouplimit" "1000" "--check-connlimit" "50" "--check-stddev" "50")
add_test(test-ratelim__group_con_lim_drain "E:/git/third/libevent/build/bin/test-ratelim" "-c" "1000" "-g" "35000" "-n" "30" "-t" "100" "-G" "500" "--check-grouplimit" "1000" "--check-connlimit" "50" "--check-stddev" "50")
