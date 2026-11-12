# Linker launcher for Ninja builds of webservice and roblox.
#
# The NMake generator hands the linker a target's own objects first and the objects of
# its OBJECT libraries after them. The Ninja generator does the reverse. Link order
# decides which alias survives an ICF fold, so a Ninja build scores differently from
# CI's NMake one. This moves the target's own objects to the front of the response
# file, the order NMake uses, then runs the link command unchanged.
#
# cmake -P ninja-link-order.cmake -- <target> <link command...>

set(target "")
set(command)
set(past_separator FALSE)
math(EXPR last "${CMAKE_ARGC} - 1")
foreach(i RANGE ${last})
  set(arg "${CMAKE_ARGV${i}}")
  if(NOT past_separator)
    if(arg STREQUAL "--")
      set(past_separator TRUE)
    endif()
  elseif(target STREQUAL "")
    set(target "${arg}")
  else()
    list(APPEND command "${arg}")
  endif()
endforeach()

list(LENGTH command command_length)
if(target STREQUAL "" OR command_length EQUAL 0)
  message(FATAL_ERROR "usage: cmake -P ninja-link-order.cmake -- <target> <link command...>")
endif()

set(own_pattern "CMakeFiles[/\\\\]${target}\\.dir[/\\\\][^ \t\r\n\"]+")
foreach(arg IN LISTS command)
  if(arg MATCHES "^@(.+)$")
    set(rsp "${CMAKE_MATCH_1}")
    file(READ "${rsp}" content)
    string(REGEX MATCHALL "(^|[ \t\r\n])${own_pattern}" own "${content}")
    if(own)
      string(REGEX REPLACE "(^|[ \t\r\n])${own_pattern}" "" rest "${content}")
      list(TRANSFORM own STRIP)
      list(JOIN own "\n" own)
      file(WRITE "${rsp}" "${own}\n${rest}")
    endif()
  endif()
endforeach()

execute_process(COMMAND ${command} RESULT_VARIABLE result)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "link failed: ${result}")
endif()
