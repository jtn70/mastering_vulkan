#include "file.hpp"

#include "foundation/memory.hpp"
#include "foundation/assert.hpp"
#include "foundation/string.hpp"

#if defined(_WIN64)
#include <windows.h>
#else
#define MAX_PATH 65536
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include <string.h>

namespace raptor {

    void file_open( cstring filename, cstring mode, FileHandle* file ) {
        #if defined(_WIN64)
            fopen_s( file, filename, mode );
        #else
            *file = fopen( filename, mode );
        #endif
    }

    void file_close( FileHandle file ) {
        if ( file )
            fclose( file );
    }

    sizet file_write( uint8_t* memory, u32 element_size, u32 count, FileHandle file ) {      
        return fwrite( memory, element_size, count, file );
    }

    static long file_get_size( FileHandle f ) {
        long fileSizeSigned;

        fseek( f, 0, SEEK_END );
        fileSizeSigned = ftell( f );
        fseek( f, 0, SEEK_SET );

        return fileSizeSigned;
    }

#if defined(_WIN64)
    FileTime file_last_write_time( cstring filename ) {
        FILETIME lastWriteTime = {};

        WIN32_FILE_ATTRIBUTE_DATA data;
        if ( GetFileAttributesExA( filename, GetFileExInfoStandard, &data ) ) {
            lastWriteTime.dwHighDataTime = data.ftLastWriteTime.dwHighDateTime;
            lastWriteTime.dwLowDataTime = data.ftLastWriteTime.dwLowDateTime;
        }

        return lastWriteTime;
    }
#endif // _WIN64

    u32 file_resolve_to_full_path( cstring path, char* out_full_path, u32 max_size ) {
#if defined(_WIN64)
        return GetFullPathNameA( path, max_size, out_full_path, max_size );
#else
        return readlink( path, out_full_path, max_size );
#endif // _WIN64
    }

    void file_directory_from_path( char* path ) {
        char*
    }
}