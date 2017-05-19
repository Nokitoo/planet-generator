# detect os
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(OS_WINDOWS 1)
else()
    message(FATAL_ERROR "Unsupported operating system or environment")
    return()
endif()

# define the architecture
if (CMAKE_CL_64)
  set(ARCH_DIR "x64")
else()
  set(ARCH_DIR "x86")
endif()
