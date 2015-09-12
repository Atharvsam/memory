// Copyright (C) 2015 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef FOONATHAN_MEMORY_TEST_TEST_ALLOCATOR_HPP
#define FOONATHAN_MEMORY_TEST_TEST_ALLOCATOR_HPP

#include <unordered_map>

struct memory_info
{
    void *memory;
    std::size_t size, alignment;
};

// RawAllocator with various security checks
class test_allocator
{
public:
    using is_stateful = std::true_type;

    void* allocate_node(std::size_t size, std::size_t alignment)
    {
        auto mem = ::operator new(size);
        last_allocated_ = {mem, size, alignment};
        allocated_[mem] = last_allocated_;
        return mem;
    }

    void deallocate_node(void* node, std::size_t size, std::size_t alignment) FOONATHAN_NOEXCEPT
    {
        ++dealloc_count_;
        auto iter = allocated_.find(node);
        if (iter == allocated_.end() ||
            iter->second.size != size || iter->second.alignment != alignment)
        {
            last_valid_ = false;
            return;
        }
        else
            allocated_.erase(iter);
        ::operator delete(node);
    }

    std::size_t max_node_size() const FOONATHAN_NOEXCEPT
    {
        return std::size_t(-1);
    }

    bool last_deallocation_valid() FOONATHAN_NOEXCEPT
    {
        return last_valid_;
    }

    void reset_last_deallocation_valid() FOONATHAN_NOEXCEPT
    {
        last_valid_ = true;
    }

    memory_info last_allocated() const FOONATHAN_NOEXCEPT
    {
        return last_allocated_;
    }

    std::size_t no_allocated() const FOONATHAN_NOEXCEPT
    {
        return allocated_.size();
    }

    std::size_t no_deallocated() const FOONATHAN_NOEXCEPT
    {
        return dealloc_count_;
    }

    void reset_deallocation_count() FOONATHAN_NOEXCEPT
    {
        dealloc_count_ = 0u;
    }

private:
    std::unordered_map<void*, memory_info> allocated_;
    memory_info last_allocated_;
    std::size_t dealloc_count_ = 0u;
    bool last_valid_ = true;
};

#endif //FOONATHAN_MEMORY_TEST_TEST_ALLOCATOR_HPP
