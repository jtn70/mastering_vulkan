#pragma once

#include "foundation/platform.hpp"

namespace raptor {

    struct Allocator;

    // Common methods
    u32         leading_zeroes_u32( u32 x );
#if defined(_MSC_VER)
    u32         leading_zeroes_u32_msvc( u32 x );
#endif
    u32         trailing_zeros_u32( u32 x );
    u64         trailing_zeros_u64( u64 x );

    u32         round_up_to_power_of_2( u32 v );

    void        print_binary( u64 n );
    void        print_binary( u32 n );

    // class BitMask

    // An abstraction over a bitmask. It provides an easy way to iterate through the
    // indexes of the set bits of a bitmask. When SHift=0 (platforms with SSE),
    // this is a true bitmask. On non-SSE platforms, the arithmetic used to 
    // emulate the SSE behaviour works in bytes (Shift=3) and leaves each bytes as 
    // either 0x00 og 0x80.
    //
    // For example:
    //  for (int i : Bitmask<uint32_t, 16>(0x5)) -> yields 0, 2
    //  for (int i : Bitmask<uint64_t, 8, 3>(0x0000000080800000)) -> yields 2, 3
    template <class T, int SignificantBits, int Shift = 0>
    class BitMask {
        //static_assert( std::is_unsigned<T>::value, "" );
        //static_assert( Shift == 0 || Shift == 3, "")
    
    public:
        // These are useful for unit tests (gunit)
        using value_type = int;
        using iterator = BitMask;
        using const_iterator = BitMask;

        explicit BitMask( T mask ) : mask_( mask ) {
        }
        BitMask& operator++() {
            mask_ &= ( mask_ -1 );
            return *this;
        }
    }
}