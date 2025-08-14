#include "memory.hpp"
#include "memory_utils.hpp"
#include "assert.hpp"

#include "../../external/tlsf/tlsf.h"

#include <stdlib.h>
#include <memory.h>

#if defined RAPTOR_IMGUI
#include "../../external/imgui/imgui.h"
#endif //RAPTOR_IMGUI

// Define this and add StackWalker to heavy memory profile
//#define RAPTOR_MEMORY_STACK

#define HEAP_ALLOCATOR_STATS

#if defined (RAPTOR_MEMORY_STACK)
#include "ExternalAccessory/StackWalker.h"
#endif // RAPTOR_MEMORY_STACK

namespace raptor {

#if defined (RAPTOR_MEMORY_DEBUG)
    #define hy_mem_assert(cond) hy_assert(cond)
#else
    #define hy_mem_assert(cond)
#endif  // RAPTOR_MEMORY_DEBUG

    // Memory Service
    static MemoryService    s_memory_service;

    // Locals
    static size_t s_size = rmega(32) + tlsf_size() + 8;

    // Walker methods
    static void exit_walker( void* ptr, size_t size, int used, void* user);
    static void imgui_walker( void* ptr, size_t size, int used, void* user);
    
    MemoryService* MemoryService::instance() {
        return &s_memory_service;
    }

    void MemoryService::init( void* configuration ) {

        rprint( "Memory Service Init\n ");
        MemoryServiceConfiguration* memory_configuration = static_cast< MemoryServiceConfiguration* >( configuration );
        system_allocator.init( memory_configuration ? memory_configuration->maximum_dynamic_size : s_size );
    }

    void MemoryService::shutdown() {

        system_allocator.shutdown();

        rprint ( "Memory Service Shutdown" );
    }

    void exit_walker( void* ptr, size_t size, int used, void* user ) {
        MemoryStatistics* stats = ( MemoryStatistics* )user;
        stats->add( used ? size : 0 );

        if ( used )
            rprint( "Found active allocation %p, %llu\n", ptr, size);
    }

#if defined RAPTOR_IMGUI
    void imgui_walker( void* ptr, size_t size, int used, void* user) {

        u32 memory_size = (u32) lsize;
        cstring memory_unit = "b";

        if ( memory_size > 1024 * 1024 ) {
            memory_size /= 1024;
            memory_unit = "Mb";
        }
        else if ( memory_size > 1024 ) {
            memory_size /= 1024;
            memory_unit = "kb";
        }
        ImGui::Text ( "\t%p $s size: $4llu %s\n",ptr, used ? "used" : "free", memory_size, memory_unit );

        MemoryStatistics* stats = ( MemoryStatistics* )user;
        stats->add( used ? size : 0 );
    }
#endif // RAPTOR_IMGUI

    void MemoryService::test() {
        /*
        static u8 mem[ 1024 ];
        LinearAllocator la;
        la.init( mem, 1024 );

        // Allocate 3 times
        void* a1 = ralloca( 16, &la );
        void* a2 = ralloca( 20, &la );
        void* a4 = ralloca( 10, &la );

        // Free based on size
        la.free(10);
        void* a3 = ralloca( 10, &la );
        RASSERT( a3 == a4 );

        // Free based on pointer
        rfree( a2, &la );
        void* a32 = ralloca( 10, &la );
        RASSERT( a32 == a2 );

        // Test out of bonds
        u8* out_bounds = ( u8* )a1 + 10000;
        rfree( out_bounds, &la );
        */
    }

    // Memory Structs

    // Heap allocator
    HeapAllocator::~HeapAllocator() {}

    void HeapAllocator::init( sizet size ) {

        // Allocate
        memory = malloc( size );
        max_size = size;
        allocated_size = 0;

        tlsf_handle = tlsf_create_with_pool( memory, size );

        rprint( "HeapAllocator of size %llu created.\n", size );
    }

    void HeapAllocator::shutdown() {

        // Check memory at the application exit.
        MemoryStatistics stats( 0, max_size );
        pool_t pool = tlsf_get_pool( tlsf_handle );
        tlsf_walk_pool( pool, exit_walker, (void* )&stats );

        if ( stats.allocated_bytes ) {
            rprint ( "ERROR on HeapAllocator shutdown. Allocated memory detected. Allocated %llu, total %llu.", stats.allocated_bytes, stats.total_bytes );
        } else {
            rprint( "HeapAllocator Shutdown - all memory freed!\n" );
        }

        RASSERTM( stats.allocatedbytes = 0, "Allocations still present. Check your code!");

        tlsf_destroy( tlsf_handle );

        free( memory );
    }
}
