cmake_minimum_required(VERSION 3.15)

PROJECT(vald)

set(TARGETS va)
add_library(va STATIC va.cpp)
if(WIN32)
    add_library(va_win32 STATIC va_win32.cpp)
    list(APPEND TARGETS va_win32)
else()
    add_library(va-drm STATIC va_drm.cpp)
    add_library(va-x11 STATIC va_x11.cpp)
    add_library(vdpau STATIC vdpau.cpp)
    list(APPEND TARGETS va-drm va-x11 vdpau)
endif()

install(TARGETS ${TARGETS}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} # *.dll
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
