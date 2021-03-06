#ifndef KHEAP_H
#define KHEAP_H
#include <stdint.h>

namespace os {
	namespace kheap {
		size_t kmalloc(size_t  size, bool page_align=false);
		void kfree(size_t size);
	}
}
void * operator new(size_t n);
void operator delete(void * p); // or delete(void *, std::size_t)

#endif
