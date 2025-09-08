#pragma once

#include "foundation/memory.hpp"
#include "foundation/assert.hpp"
#include "foundation/bit.hpp"

#include "../external/wyhash/wyhash.h"

namespace raptor {

    // Hash Map

    static const u64 k_iterator_end = u64_max;

    struct FindInfo {
        u64 offset;
        u64 probe_length;
    }; // struct FindInfo

    struct FindResult {
        u64 index;
        bool free_index; // States if the index is free or used.
    }; // struct FindResult

    // Iterator that stores the index of the entry.
    struct FlasHashMapIterator {
        u64 index;
        bool is_valid() const { return index != k_iterator_end; }
        bool is_invalid() const { return index == k_iterator_end; }
    }; // struct FlatHashMapIterator

    // A single block of empty control bytes for tables without any slots allocated.
    // This enables removing a branch in the hot path of find().
    i8* group_init_empty();

    // Probing
    struct ProbeSequence {

        static const u64 k_width = 16;  // TODO: this should be selectable.
        static const sizet k_engine_hash = 0x31d3a36013e;

        ProbeSequence( u64 hash, u64 mask );

        u64 get_offset() const;
        u64 get_offset( u64 i ) const;

        // 0-based probe index. The i-th probe in the probe sequence.
        u64 get_index() const;

        void next();

        u64 mask;
        u64 offset;
        u64 index = 0;
    }; // struct ProbeSequence

    template <typename K, typename V>
    struct FlatHashMap {
        
    }
}
