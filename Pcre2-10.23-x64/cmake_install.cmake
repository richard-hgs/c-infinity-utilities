# Install script for directory: H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "H:/Programming/VSProjects/InfinityProjectsCpp/InfinityCmake/out/install/x86-Debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/pcre2-8d.lib")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/pcre2-posixd.lib")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/pcre2grep.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/pcre2test.exe")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/pcre2.h"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/src/pcre2posix.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man1" TYPE FILE FILES
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2-config.1"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2grep.1"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2test.1"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man3" TYPE FILE FILES
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_callout_enumerate.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_code_copy.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_code_copy_with_tables.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_code_free.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_compile.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_compile_context_copy.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_compile_context_create.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_compile_context_free.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_config.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_dfa_match.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_general_context_copy.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_general_context_create.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_general_context_free.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_get_error_message.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_get_mark.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_get_ovector_count.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_get_ovector_pointer.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_get_startchar.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_jit_compile.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_jit_free_unused_memory.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_jit_match.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_jit_stack_assign.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_jit_stack_create.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_jit_stack_free.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_maketables.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_match.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_match_context_copy.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_match_context_create.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_match_context_free.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_match_data_create.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_match_data_create_from_pattern.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_match_data_free.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_pattern_info.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_serialize_decode.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_serialize_encode.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_serialize_free.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_serialize_get_number_of_codes.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_set_bsr.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_set_callout.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_set_character_tables.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_set_compile_recursion_guard.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_set_match_limit.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_set_max_pattern_length.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_set_newline.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_set_offset_limit.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_set_parens_nest_limit.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_set_recursion_limit.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_set_recursion_memory_management.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_substitute.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_substring_copy_byname.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_substring_copy_bynumber.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_substring_free.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_substring_get_byname.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_substring_get_bynumber.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_substring_length_byname.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_substring_length_bynumber.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_substring_list_free.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_substring_list_get.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_substring_nametable_scan.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2_substring_number_from_name.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2api.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2build.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2callout.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2compat.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2demo.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2jit.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2limits.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2matching.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2partial.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2pattern.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2perform.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2posix.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2sample.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2serialize.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2stack.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2syntax.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2unicode.3"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/pcre2unicode.3"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/pcre2/html" TYPE FILE FILES
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/index.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2-config.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_callout_enumerate.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_code_copy.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_code_copy_with_tables.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_code_free.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_compile.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_compile_context_copy.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_compile_context_create.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_compile_context_free.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_config.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_dfa_match.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_general_context_copy.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_general_context_create.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_general_context_free.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_get_error_message.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_get_mark.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_get_ovector_count.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_get_ovector_pointer.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_get_startchar.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_jit_compile.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_jit_free_unused_memory.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_jit_match.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_jit_stack_assign.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_jit_stack_create.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_jit_stack_free.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_maketables.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_match.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_match_context_copy.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_match_context_create.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_match_context_free.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_match_data_create.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_match_data_create_from_pattern.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_match_data_free.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_pattern_info.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_serialize_decode.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_serialize_encode.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_serialize_free.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_serialize_get_number_of_codes.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_set_bsr.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_set_callout.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_set_character_tables.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_set_compile_recursion_guard.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_set_match_limit.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_set_max_pattern_length.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_set_newline.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_set_offset_limit.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_set_parens_nest_limit.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_set_recursion_limit.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_set_recursion_memory_management.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_substitute.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_substring_copy_byname.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_substring_copy_bynumber.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_substring_free.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_substring_get_byname.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_substring_get_bynumber.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_substring_length_byname.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_substring_length_bynumber.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_substring_list_free.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_substring_list_get.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_substring_nametable_scan.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2_substring_number_from_name.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2api.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2build.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2callout.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2compat.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2demo.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2grep.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2jit.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2limits.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2matching.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2partial.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2pattern.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2perform.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2posix.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2sample.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2serialize.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2stack.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2syntax.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2test.html"
    "H:/Programming/VSProjects/InfinityProjectsCpp/Pcre2-10.23-x86/doc/html/pcre2unicode.html"
    )
endif()

