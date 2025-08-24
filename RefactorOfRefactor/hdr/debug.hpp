#ifdef assert
#undef assert
#endif

// Small hack to eliminate name collision
// between my assert function and assert macro from C

#include "debug2.hpp"