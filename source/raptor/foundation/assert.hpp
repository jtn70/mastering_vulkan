#pragma once

#include "foundation/log.hpp"

namespace raptor {
    #define RASSERT( condition )    if(!(condition)) { rprint(RAPTOR_FILELINE("FALSE\n")); RAPTOR_DEBUG_BREAK }
    
}
