#pragma once

#include "foundation/platform.hpp"

namespace raptor {

    // Forward declarations
    struct Allocator;

    template <typename K, typename V>
    struct FlatHashMap;

    struct FlatHashMapIterator;

    // String view that refernces an already existing stream of chars.
    struct StringView {
        char* text;
        sizet length;

        //static stringview
        static bool equals( const StringView& a, const StringView& b );
        static void copy_to( c)
    }
    
    

} // namespace raptor
