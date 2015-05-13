// Copyright (C) 2015 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include "new_allocator.hpp"

#include "debugging.hpp"

using namespace foonathan::memory;

void* new_allocator::allocate_node(std::size_t size, std::size_t)
{
    auto mem = static_cast<char*>(::operator new(size + 2 * detail::debug_fence_size));
    detail::debug_fill(mem, detail::debug_fence_size, debug_magic::fence_memory);
    mem += detail::debug_fence_size;
    detail::debug_fill(mem, size, debug_magic::new_memory);
    detail::debug_fill(mem + size, detail::debug_fence_size, debug_magic::fence_memory);
    return mem;
}

void new_allocator::deallocate_node(void* node, std::size_t size, std::size_t) FOONATHAN_NOEXCEPT
{
    detail::debug_fill(node, size, debug_magic::freed_memory);
    auto memory = static_cast<char*>(node) - detail::debug_fence_size;
    ::operator delete(memory);
}
