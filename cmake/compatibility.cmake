# Copyright (C) 2015 Jonathan Müller <jonathanmueller.dev@gmail.com>
# This file is subject to the license terms in the LICENSE file
# found in the top-level directory of this distribution.

# compatibility.cmake - various necessary compatibility checks
# note: only include it in memory's top-level CMakeLists.txt

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/comp/comp_base.cmake)

# download in source for convenience
set(_foonathan_comp_dest_dir ${CMAKE_CURRENT_SOURCE_DIR}/cmake/comp)

function(_foonathan_use_comp target)
    comp_target_features(${target} PUBLIC
        cpp11_lang/alignof cpp11_lang/constexpr cpp11_lang/noexcept cpp11_lang/thread_local
        cpp11_lib/max_align_t cpp11_lib/get_new_handler
        env/exception_support env/hosted_implementation
        PREFIX "FOONATHAN_" NAMESPACE "foonathan_memory_comp"
        CMAKE_PATH "${_foonathan_comp_dest_dir}")
endfunction()
