#include <stdint.h>
#ifndef VIDEO_MEMORY
// Might need to change this when paging is enabled
// and we move the kernel to the higher half
// and map b8000 somewhere else
//#define VIDEO_MEMORY 0xb8000
#define VIDEO_MEMORY 0xC03FF000
#endif

