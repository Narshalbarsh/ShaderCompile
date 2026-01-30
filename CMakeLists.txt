cmake_minimum_required(VERSION 3.7...3.19)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(ShaderCompile CXX)

option(RE2_BUILD_TESTING "" OFF)

add_subdirectory(shared/re2)
add_subdirectory(shared/gsl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC
    ShaderCompile/cfgprocessor.cpp
    ShaderCompile/d3dxfxc.cpp
    ShaderCompile/ShaderCompile.cpp
    ShaderCompile/shaderparser.cpp
    ShaderCompile/utlbuffer.cpp
)

add_executable(ShaderCompile ${SRC})
target_link_libraries(ShaderCompile PRIVATE re2::re2 Microsoft.GSL::GSL)

if (WIN32)
  target_link_libraries(ShaderCompile PRIVATE dbghelp d3dcompiler)
endif()

target_include_directories(ShaderCompile PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ShaderCompile/include
    ${CMAKE_CURRENT_SOURCE_DIR}/shared/re2
)

if(MSVC)
    target_compile_options(ShaderCompile PRIVATE /Zc:__cplusplus)
    set_property(TARGET ShaderCompile PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set_property(TARGET re2          PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    target_compile_definitions(ShaderCompile PRIVATE _ITERATOR_DEBUG_LEVEL=0)
    target_compile_definitions(re2          PRIVATE _ITERATOR_DEBUG_LEVEL=0)
endif()

if(MINGW)
  # keep libstdc++/libgcc static
  target_link_options(ShaderCompile PRIVATE
    -static-libgcc
    -static-libstdc++
  )

  # Make the toolchain's implicit "-lpthread" resolve to a *static* archive
  execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libwinpthread.a
    OUTPUT_VARIABLE WINPTHREAD_A
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  if(NOT WINPTHREAD_A OR WINPTHREAD_A STREQUAL "libwinpthread.a")
    message(FATAL_ERROR "Could not locate libwinpthread.a via ${CMAKE_CXX_COMPILER}")
  endif()

  set(PTHREAD_ALIAS_DIR "${CMAKE_BINARY_DIR}/pthread_alias")
  execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${PTHREAD_ALIAS_DIR}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
                  "${WINPTHREAD_A}"
                  "${PTHREAD_ALIAS_DIR}/libpthread.a")

  # Ensure our alias directory is searched before the toolchain lib dirs
  target_link_directories(ShaderCompile PRIVATE "${PTHREAD_ALIAS_DIR}")
endif()
