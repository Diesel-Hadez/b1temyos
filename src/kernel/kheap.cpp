#include "utils.h"
#include "common.h"
#include "terminal.h"
#include "kheap.h"

extern "C" void* _heap_start;
namespace {
	uint32_t placement_address{0};
}

// Need to be very careful when using this.
// A kfree assumes that you are kfreeing the last 
// kmalloced item and that you know it's size
// 
size_t os::kheap::kmalloc(size_t size, bool page_align) {
	if (placement_address == 0) {
		placement_address =reinterpret_cast<unsigned int>(&_heap_start);
		os::Term::kprintf("_heap_start: %x\n", reinterpret_cast<unsigned int>(&_heap_start));
	}
	if (page_align && (placement_address & 0x00000FFF != 0)) {
		placement_address += 0x1000;
		placement_address &= 0xFFFFF000;
	}
	size_t ret = placement_address;
	placement_address += size;
	return ret;
}

// Doesn't nullify data
void os::kheap::kfree(size_t size) {
	placement_address -= size;
}

// Virtual Base Class I use for MemoryAddress requires an implementation for new and delete
// I don't actually have a working heap yet, so it's definitely leaking memory but eh.
void *operator new(size_t size)
{
    return reinterpret_cast<void*>(os::kheap::kmalloc(size));
}
 
void *operator new[](size_t size)
{
    return reinterpret_cast<void*>(os::kheap::kmalloc(size));
}
 
// Uhhh
void operator delete(void *p)// or delete(void *, std::size_t)
{
}
 
void operator delete[](void *p)
{
}

// I need this particular argument list for some reason
void operator delete(void *p, size_t temp)
{
}
