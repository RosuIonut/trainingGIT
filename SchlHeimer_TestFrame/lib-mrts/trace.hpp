#ifndef TRACE_HPP
#define TRACE_HPP

#ifdef USE_TRACE

// Debug configuration
#include <cstdio>
#define TRACE(...)  printf(__VA_ARGS__)

#else

// Release configuration
#define TRACE(...)

#endif

#endif
