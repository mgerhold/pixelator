include(${CMAKE_SOURCE_DIR}/cmake/warnings.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/sanitizers.cmake)

# the following function was taken from:
# https://github.com/cpp-best-practices/cmake_template/blob/main/ProjectOptions.cmake
macro(check_sanitizer_support)
    if ((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
        set(supports_ubsan ON)
    else ()
        set(supports_ubsan OFF)
    endif ()

    if ((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
        set(supports_asan OFF)
    else ()
        set(supports_asan ON)
    endif ()
endmacro()

if (PROJECT_IS_TOP_LEVEL)
    option(c2k_pixelator_warnings_as_errors "Treat warnings as errors" ON)
    option(c2k_pixelator_enable_undefined_behavior_sanitizer "Enable undefined behavior sanitizer" ${supports_ubsan})
    option(c2k_pixelator_enable_address_sanitizer "Enable address sanitizer" ${supports_asan})
else ()
    option(c2k_pixelator_warnings_as_errors "Treat warnings as errors" OFF)
    option(c2k_pixelator_enable_undefined_behavior_sanitizer "Enable undefined behavior sanitizer" OFF)
    option(c2k_pixelator_enable_address_sanitizer "Enable address sanitizer" OFF)
endif ()

add_library(c2k_pixelator_warnings INTERFACE)
set_warnings(c2k_pixelator_warnings ${c2k_pixelator_warnings_as_errors})

add_library(c2k_pixelator_sanitizers INTERFACE)
enable_sanitizers(c2k_pixelator_sanitizers ${c2k_pixelator_enable_address_sanitizer} ${c2k_pixelator_enable_undefined_behavior_sanitizer})

add_library(c2k_pixelator_project_options INTERFACE)
target_link_libraries(c2k_pixelator_project_options
        INTERFACE c2k_pixelator_warnings
        INTERFACE c2k_pixelator_sanitizers
)
