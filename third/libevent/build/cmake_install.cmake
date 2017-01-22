# Install script for directory: E:/git/third/libevent

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/libevent")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "lib" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/libevent/lib/event.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/git/third/libevent/build/lib/Debug/event.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/libevent/lib/event.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/git/third/libevent/build/lib/Release/event.lib")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "bin" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/libevent/bin/event.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/bin" TYPE SHARED_LIBRARY FILES "E:/git/third/libevent/build/bin/Debug/event.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/libevent/bin/event.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/bin" TYPE SHARED_LIBRARY FILES "E:/git/third/libevent/build/bin/Release/event.dll")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files/libevent/include/event2/buffer.h;C:/Program Files/libevent/include/event2/bufferevent.h;C:/Program Files/libevent/include/event2/bufferevent_compat.h;C:/Program Files/libevent/include/event2/bufferevent_struct.h;C:/Program Files/libevent/include/event2/buffer_compat.h;C:/Program Files/libevent/include/event2/dns.h;C:/Program Files/libevent/include/event2/dns_compat.h;C:/Program Files/libevent/include/event2/dns_struct.h;C:/Program Files/libevent/include/event2/event.h;C:/Program Files/libevent/include/event2/event_compat.h;C:/Program Files/libevent/include/event2/event_struct.h;C:/Program Files/libevent/include/event2/http.h;C:/Program Files/libevent/include/event2/http_compat.h;C:/Program Files/libevent/include/event2/http_struct.h;C:/Program Files/libevent/include/event2/keyvalq_struct.h;C:/Program Files/libevent/include/event2/listener.h;C:/Program Files/libevent/include/event2/rpc.h;C:/Program Files/libevent/include/event2/rpc_compat.h;C:/Program Files/libevent/include/event2/rpc_struct.h;C:/Program Files/libevent/include/event2/tag.h;C:/Program Files/libevent/include/event2/tag_compat.h;C:/Program Files/libevent/include/event2/thread.h;C:/Program Files/libevent/include/event2/util.h;C:/Program Files/libevent/include/event2/visibility.h;C:/Program Files/libevent/include/event2/event-config.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/include/event2" TYPE FILE FILES
    "E:/git/third/libevent/include/event2/buffer.h"
    "E:/git/third/libevent/include/event2/bufferevent.h"
    "E:/git/third/libevent/include/event2/bufferevent_compat.h"
    "E:/git/third/libevent/include/event2/bufferevent_struct.h"
    "E:/git/third/libevent/include/event2/buffer_compat.h"
    "E:/git/third/libevent/include/event2/dns.h"
    "E:/git/third/libevent/include/event2/dns_compat.h"
    "E:/git/third/libevent/include/event2/dns_struct.h"
    "E:/git/third/libevent/include/event2/event.h"
    "E:/git/third/libevent/include/event2/event_compat.h"
    "E:/git/third/libevent/include/event2/event_struct.h"
    "E:/git/third/libevent/include/event2/http.h"
    "E:/git/third/libevent/include/event2/http_compat.h"
    "E:/git/third/libevent/include/event2/http_struct.h"
    "E:/git/third/libevent/include/event2/keyvalq_struct.h"
    "E:/git/third/libevent/include/event2/listener.h"
    "E:/git/third/libevent/include/event2/rpc.h"
    "E:/git/third/libevent/include/event2/rpc_compat.h"
    "E:/git/third/libevent/include/event2/rpc_struct.h"
    "E:/git/third/libevent/include/event2/tag.h"
    "E:/git/third/libevent/include/event2/tag_compat.h"
    "E:/git/third/libevent/include/event2/thread.h"
    "E:/git/third/libevent/include/event2/util.h"
    "E:/git/third/libevent/include/event2/visibility.h"
    "E:/git/third/libevent/build/include/event2/event-config.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "lib" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/libevent/lib/event_core.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/git/third/libevent/build/lib/Debug/event_core.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/libevent/lib/event_core.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/git/third/libevent/build/lib/Release/event_core.lib")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "bin" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/libevent/bin/event_core.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/bin" TYPE SHARED_LIBRARY FILES "E:/git/third/libevent/build/bin/Debug/event_core.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/libevent/bin/event_core.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/bin" TYPE SHARED_LIBRARY FILES "E:/git/third/libevent/build/bin/Release/event_core.dll")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files/libevent/include/event2/buffer.h;C:/Program Files/libevent/include/event2/bufferevent.h;C:/Program Files/libevent/include/event2/bufferevent_compat.h;C:/Program Files/libevent/include/event2/bufferevent_struct.h;C:/Program Files/libevent/include/event2/buffer_compat.h;C:/Program Files/libevent/include/event2/dns.h;C:/Program Files/libevent/include/event2/dns_compat.h;C:/Program Files/libevent/include/event2/dns_struct.h;C:/Program Files/libevent/include/event2/event.h;C:/Program Files/libevent/include/event2/event_compat.h;C:/Program Files/libevent/include/event2/event_struct.h;C:/Program Files/libevent/include/event2/http.h;C:/Program Files/libevent/include/event2/http_compat.h;C:/Program Files/libevent/include/event2/http_struct.h;C:/Program Files/libevent/include/event2/keyvalq_struct.h;C:/Program Files/libevent/include/event2/listener.h;C:/Program Files/libevent/include/event2/rpc.h;C:/Program Files/libevent/include/event2/rpc_compat.h;C:/Program Files/libevent/include/event2/rpc_struct.h;C:/Program Files/libevent/include/event2/tag.h;C:/Program Files/libevent/include/event2/tag_compat.h;C:/Program Files/libevent/include/event2/thread.h;C:/Program Files/libevent/include/event2/util.h;C:/Program Files/libevent/include/event2/visibility.h;C:/Program Files/libevent/include/event2/event-config.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/include/event2" TYPE FILE FILES
    "E:/git/third/libevent/include/event2/buffer.h"
    "E:/git/third/libevent/include/event2/bufferevent.h"
    "E:/git/third/libevent/include/event2/bufferevent_compat.h"
    "E:/git/third/libevent/include/event2/bufferevent_struct.h"
    "E:/git/third/libevent/include/event2/buffer_compat.h"
    "E:/git/third/libevent/include/event2/dns.h"
    "E:/git/third/libevent/include/event2/dns_compat.h"
    "E:/git/third/libevent/include/event2/dns_struct.h"
    "E:/git/third/libevent/include/event2/event.h"
    "E:/git/third/libevent/include/event2/event_compat.h"
    "E:/git/third/libevent/include/event2/event_struct.h"
    "E:/git/third/libevent/include/event2/http.h"
    "E:/git/third/libevent/include/event2/http_compat.h"
    "E:/git/third/libevent/include/event2/http_struct.h"
    "E:/git/third/libevent/include/event2/keyvalq_struct.h"
    "E:/git/third/libevent/include/event2/listener.h"
    "E:/git/third/libevent/include/event2/rpc.h"
    "E:/git/third/libevent/include/event2/rpc_compat.h"
    "E:/git/third/libevent/include/event2/rpc_struct.h"
    "E:/git/third/libevent/include/event2/tag.h"
    "E:/git/third/libevent/include/event2/tag_compat.h"
    "E:/git/third/libevent/include/event2/thread.h"
    "E:/git/third/libevent/include/event2/util.h"
    "E:/git/third/libevent/include/event2/visibility.h"
    "E:/git/third/libevent/build/include/event2/event-config.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "lib" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/libevent/lib/event_extra.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/git/third/libevent/build/lib/Debug/event_extra.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/libevent/lib/event_extra.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/git/third/libevent/build/lib/Release/event_extra.lib")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "bin" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/libevent/bin/event_extra.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/bin" TYPE SHARED_LIBRARY FILES "E:/git/third/libevent/build/bin/Debug/event_extra.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/libevent/bin/event_extra.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/bin" TYPE SHARED_LIBRARY FILES "E:/git/third/libevent/build/bin/Release/event_extra.dll")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files/libevent/include/event2/buffer.h;C:/Program Files/libevent/include/event2/bufferevent.h;C:/Program Files/libevent/include/event2/bufferevent_compat.h;C:/Program Files/libevent/include/event2/bufferevent_struct.h;C:/Program Files/libevent/include/event2/buffer_compat.h;C:/Program Files/libevent/include/event2/dns.h;C:/Program Files/libevent/include/event2/dns_compat.h;C:/Program Files/libevent/include/event2/dns_struct.h;C:/Program Files/libevent/include/event2/event.h;C:/Program Files/libevent/include/event2/event_compat.h;C:/Program Files/libevent/include/event2/event_struct.h;C:/Program Files/libevent/include/event2/http.h;C:/Program Files/libevent/include/event2/http_compat.h;C:/Program Files/libevent/include/event2/http_struct.h;C:/Program Files/libevent/include/event2/keyvalq_struct.h;C:/Program Files/libevent/include/event2/listener.h;C:/Program Files/libevent/include/event2/rpc.h;C:/Program Files/libevent/include/event2/rpc_compat.h;C:/Program Files/libevent/include/event2/rpc_struct.h;C:/Program Files/libevent/include/event2/tag.h;C:/Program Files/libevent/include/event2/tag_compat.h;C:/Program Files/libevent/include/event2/thread.h;C:/Program Files/libevent/include/event2/util.h;C:/Program Files/libevent/include/event2/visibility.h;C:/Program Files/libevent/include/event2/event-config.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/include/event2" TYPE FILE FILES
    "E:/git/third/libevent/include/event2/buffer.h"
    "E:/git/third/libevent/include/event2/bufferevent.h"
    "E:/git/third/libevent/include/event2/bufferevent_compat.h"
    "E:/git/third/libevent/include/event2/bufferevent_struct.h"
    "E:/git/third/libevent/include/event2/buffer_compat.h"
    "E:/git/third/libevent/include/event2/dns.h"
    "E:/git/third/libevent/include/event2/dns_compat.h"
    "E:/git/third/libevent/include/event2/dns_struct.h"
    "E:/git/third/libevent/include/event2/event.h"
    "E:/git/third/libevent/include/event2/event_compat.h"
    "E:/git/third/libevent/include/event2/event_struct.h"
    "E:/git/third/libevent/include/event2/http.h"
    "E:/git/third/libevent/include/event2/http_compat.h"
    "E:/git/third/libevent/include/event2/http_struct.h"
    "E:/git/third/libevent/include/event2/keyvalq_struct.h"
    "E:/git/third/libevent/include/event2/listener.h"
    "E:/git/third/libevent/include/event2/rpc.h"
    "E:/git/third/libevent/include/event2/rpc_compat.h"
    "E:/git/third/libevent/include/event2/rpc_struct.h"
    "E:/git/third/libevent/include/event2/tag.h"
    "E:/git/third/libevent/include/event2/tag_compat.h"
    "E:/git/third/libevent/include/event2/thread.h"
    "E:/git/third/libevent/include/event2/util.h"
    "E:/git/third/libevent/include/event2/visibility.h"
    "E:/git/third/libevent/build/include/event2/event-config.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files/libevent/include/evdns.h;C:/Program Files/libevent/include/evrpc.h;C:/Program Files/libevent/include/event.h;C:/Program Files/libevent/include/evhttp.h;C:/Program Files/libevent/include/evutil.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/include" TYPE FILE FILES
    "E:/git/third/libevent/include/evdns.h"
    "E:/git/third/libevent/include/evrpc.h"
    "E:/git/third/libevent/include/event.h"
    "E:/git/third/libevent/include/evhttp.h"
    "E:/git/third/libevent/include/evutil.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files/libevent/cmake/LibeventConfig.cmake;C:/Program Files/libevent/cmake/LibeventConfigVersion.cmake")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/cmake" TYPE FILE FILES
    "E:/git/third/libevent/build//CMakeFiles/LibeventConfig.cmake"
    "E:/git/third/libevent/build/LibeventConfigVersion.cmake"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}C:/Program Files/libevent/cmake/LibeventTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}C:/Program Files/libevent/cmake/LibeventTargets.cmake"
         "E:/git/third/libevent/build/CMakeFiles/Export/C_/Program_Files/libevent/cmake/LibeventTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}C:/Program Files/libevent/cmake/LibeventTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}C:/Program Files/libevent/cmake/LibeventTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files/libevent/cmake/LibeventTargets.cmake")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/cmake" TYPE FILE FILES "E:/git/third/libevent/build/CMakeFiles/Export/C_/Program_Files/libevent/cmake/LibeventTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/libevent/cmake/LibeventTargets-debug.cmake")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/cmake" TYPE FILE FILES "E:/git/third/libevent/build/CMakeFiles/Export/C_/Program_Files/libevent/cmake/LibeventTargets-debug.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/libevent/cmake/LibeventTargets-release.cmake")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "C:/Program Files/libevent/cmake" TYPE FILE FILES "E:/git/third/libevent/build/CMakeFiles/Export/C_/Program_Files/libevent/cmake/LibeventTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "E:/git/third/libevent/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
