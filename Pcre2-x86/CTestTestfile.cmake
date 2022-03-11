# CMake generated Testfile for 
# Source directory: H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86
# Build directory: H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-x86
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(pcre2_test_bat "pcre2_test.bat")
  set_tests_properties(pcre2_test_bat PROPERTIES  PASS_REGULAR_EXPRESSION "RunTest\\.bat tests successfully completed" _BACKTRACE_TRIPLES "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/CMakeLists.txt;675;ADD_TEST;H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(pcre2_test_bat "pcre2_test.bat")
  set_tests_properties(pcre2_test_bat PROPERTIES  PASS_REGULAR_EXPRESSION "RunTest\\.bat tests successfully completed" _BACKTRACE_TRIPLES "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/CMakeLists.txt;675;ADD_TEST;H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(pcre2_test_bat "pcre2_test.bat")
  set_tests_properties(pcre2_test_bat PROPERTIES  PASS_REGULAR_EXPRESSION "RunTest\\.bat tests successfully completed" _BACKTRACE_TRIPLES "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/CMakeLists.txt;675;ADD_TEST;H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(pcre2_test_bat "pcre2_test.bat")
  set_tests_properties(pcre2_test_bat PROPERTIES  PASS_REGULAR_EXPRESSION "RunTest\\.bat tests successfully completed" _BACKTRACE_TRIPLES "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/CMakeLists.txt;675;ADD_TEST;H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/CMakeLists.txt;0;")
else()
  add_test(pcre2_test_bat NOT_AVAILABLE)
endif()
