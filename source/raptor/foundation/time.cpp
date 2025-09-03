#include "time.hpp"

#if defined(_MSC_VER)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <time.h>
#endif

namespace raptor {

    #if defined(_MSC_VER)
    // Cached frequency.
    // From Microsoft Docs: (https://docs.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency)
    // "The frequency of the performance counter is fixed at system boot and is consistent across all processors.
    // Therefore, the frequency need only be queried upon application initialization, and the result can be cached."
    static LARGE_INTEGER s_frequency;
    #endif
    
    void time_service_init() {
#if defined(_MSC_VER)
        // Cache this value - by Microsoft Docs it will not change during process lifetime.
        QueryPerformanceFrequency(&s_frequency);      
#endif
    }

    void time_service_shutdown()  {
        // Nothin to do
    }

    // Taken from the Rust code base: ttps://github.com/rust-lang/rust/blob/3809bbf47c8557bd149b3e52ceb47434ca8378d5/src/libstd/sys_common/mod.rs#L124
    static i64 int64_mul_div( i64 value, i64 numer, i64 denom ) {
        const i64 q = value / denom;
        const i64 r = value % denom;
        // Decompose value as (value/denom*denom + value%denom),
        // substitute into (value*numer)/denom and simplify.
        // r < denom, so (denom*numer) is the upper bound of (r*numer)
        return q * numer + r * numer / denom;
    }

    
}
