# SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
# SPDX-License-Identifier: GPL-3.0-only

# Add runner
add_executable(AOC "Runner.cpp" "Utils.cpp")
target_compile_features(AOC PRIVATE cxx_std_17)
set_target_properties(AOC PROPERTIES CXX_EXTENSIONS OFF)

# Runner dependencies
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY "https://github.com/libcpr/cpr.git"
                         GIT_TAG "1.9.3")
FetchContent_MakeAvailable(cpr)
target_link_libraries(AOC PRIVATE cpr::cpr)

# Add solutions
file(GLOB sources CONFIGURE_DEPENDS "*.cpp")
foreach (file ${sources})
    target_sources(AOC PRIVATE ${file})
endforeach()

# Add session token
if (NOT AOC_SESSION_TOKEN)
    message(FATAL_ERROR "Please pass your AOC session token to cmake (-DAOC_SESSION_TOKEN=xyz)")
else()
    string(LENGTH ${AOC_SESSION_TOKEN} SESSION_TOKEN_LENGTH)
    if (NOT ${SESSION_TOKEN_LENGTH} EQUAL 128)
        message(FATAL_ERROR "Please make sure your AOC session token is correct")
    endif()
endif()
configure_file(Session.hpp.in ${CMAKE_BINARY_DIR}/generated/Session.hpp)
target_include_directories(AOC PRIVATE ${CMAKE_BINARY_DIR}/generated/)
