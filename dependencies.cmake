include("${CMAKE_SOURCE_DIR}/cmake/CPM.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/system_link.cmake")

function(setup_dependencies)
    CPMAddPackage(
            NAME SPDLOG
            GITHUB_REPOSITORY gabime/spdlog
            VERSION 1.12.0
            OPTIONS
            "SPDLOG_BUILD_EXAMPLE OFF"
            "SPDLOG_BUILD_TESTS OFF"
    )
    CPMAddPackage(
            NAME GSL
            GITHUB_REPOSITORY microsoft/GSL
            VERSION 4.0.0
            OPTIONS
            "GSL_TEST OFF"
    )
    CPMAddPackage(
            NAME GLFW
            GITHUB_REPOSITORY glfw/glfw
            GIT_TAG 3.3.9
            OPTIONS
            "GLFW_BUILD_DOCS OFF"
            "GLFW_INSTALL OFF"
    )
    CPMAddPackage(
            NAME range-v3
            GITHUB_REPOSITORY ericniebler/range-v3
            GIT_TAG 0.12.0
    )
    CPMAddPackage(
            NAME glm
            GITHUB_REPOSITORY g-truc/glm
            GIT_TAG 1.0.0
            OPTIONS
            "GLM_ENABLE_CXX_20 ON"
    )
    CPMAddPackage(
            NAME tl-expected
            GITHUB_REPOSITORY TartanLlama/expected
            VERSION 1.1.0
    )
endfunction()
