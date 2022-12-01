# SPDX-FileCopyrightText: 2022 metaquarx <metaquarx@protonmail.com>
# SPDX-License-Identifier: GPL-3.0-only

file(GLOB sources CONFIGURE_DEPENDS "*.cpp")
foreach (file ${sources})
    string(REGEX MATCH "(Day[0-9]+)" target_name ${file})
    add_executable("${target_name}" "${file}")
    target_compile_features("${target_name}" PUBLIC cxx_std_17)
    set_target_properties("${target_name}" PROPERTIES CXX_EXTENSIONS OFF)
endforeach()
