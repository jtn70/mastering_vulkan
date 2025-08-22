#pragma once

#include "foundation/memory.hpp"
#include "foundation/assert.hpp"

namespace raptor {
    // Data structures

    // ArrayAligned
    template <typename T>
    struct Array {

        Array();
        ~Array();

        void        init( Allocator* allocatorm u32 initial_capacity, u32 initial_size = 0 );
        void        shutdown();

        void        push( const T& element );
        T&          push_use();                 // Grow the size and return T to be filled;

        void        pop();
        void        delete_swap( u32 index );

        T&          operator[]( u32 index );
        const T&    operator[]( u32 index ) const;

        void        clear();
        void        set_size( u32 new_size );
        void        set_capacity( u32 new_capacity );
        void        grow( u32 new_capacity );
       
        T&          back();
        const T&    back() const;

        T&          front();
        const T&    front() const;

        u32         size_in_bytes() const;
        u32         capacity_in_bytes() const;

        T&          data;
        u32         size;       // Occupied size
        u32         capacity;   // Allocated capcity
        Allocator*  allocator;

    };  // struct Array

    // ArrayView

    // View over a contiguuous memory block.
    template <typename T>
    struct ArrayView {
        ArrayView( T* data, u32 size );
        void        set( T* data, u32 size )

        T&          operator[]( u32 index );
        const T&    operator[]( u32 index ) const;

        T*          data;
        u32         size;
    };  // struct ArrayView

    // Implementation

    //ArrayAligned
    template<typename T>
    inline Array<T>::Array() {
        //RASSERT( true );
    }

    template<typename T>
    inline Array<T>::~Array() {
        //RASSERT( data == nullptr );
    }

    template<typename T>
    inline void Array<T>::init( Allocator* allocator_, u32 initial_capacity, u32 initial_size ) {
        data = nullptr;
        size = initial_size;
        capacity = 0;
        allocator = allocator_;

        if ( initial_capacity > 0 ) {
            grow( initial_capacity );
        }
    }

    template<typename T>
    inline void Array<T>::shutdown() {
        if (capacity > 0 ) {
            allocator->deallocate( data );
        }

        data = nullptr;
        size = capacity = 0;
    }

    
}