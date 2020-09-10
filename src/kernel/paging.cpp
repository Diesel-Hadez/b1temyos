#include "paging.h"
#include "kheap.h"
#include "utils.h"
#include "terminal.h"

namespace {
	os::paging::PageDirectory current_directory;
}

void os::paging::Init() {

}

size_t os::paging::MemoryAddress::GetAddress() const {
	return m_Address;
}


os::paging::MemoryAddress::~MemoryAddress(){};
void os::paging::SwitchPageDirectory(const PageDirectory &pd) {
// From JamesM tutorial

// I hope it makes a deep copy *shrug*
// Otherwise I could probably overload the copy assignment operator.
// And then probably follow rule of three
   current_directory = pd;
   asm volatile("mov %0, %%cr3":: "r"(&pd.m_PhysicalTables));
   uint32_t cr0;
   asm volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80010000; // Enable paging and write protect!
   asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void os::paging::Reload() {
   uint32_t cr3;
   asm ("mov %%cr3, %0": "=r"(cr3));
   asm ("mov %0, %%cr3":: "r"(cr3));
}

// We check if address is within the addressable range
// though I guess since it's in size_t it's already
// in that range. Huh. I did not think this through.
os::paging::MemoryAddress::MemoryAddress(size_t addr) {
	// We assume a 32 bit system
	/*
	if (addr > 0xFFFFFFFF || 
		addr < 0x0) {
		// Oh yeah, and I have no heap
		// so no string class
		throw "Oops";
	}
	*/
	// Actual assigning of m_Address to addr is left to derived
	// class constructor
}

os::paging::VirtualAddress::VirtualAddress(size_t addr) : MemoryAddress(addr) {
	m_Address = addr;
}

os::paging::PhysicalAddress::PhysicalAddress(size_t addr) : MemoryAddress(addr) {
	m_Address = addr;
}

using os::paging::PageStatus;

os::paging::PageAlignedAddress::PageAlignedAddress(size_t addr) : MemoryAddress(addr) {
	if (addr & 0x00000FFF != 0) {
		// Exceptions currently not available
	//	throw PageStatus::NOT_PAGE_ALIGNED;
	}
}

os::paging::PageDirectory::PageDirectory() {
	// I think this is done by the compiler already
	//tables = reinterpret_cast<void*>(os::kheap::kmalloc(1024*sizeof(os::paging::PageTable), true));
}

PageStatus os::paging::PageDirectory::MapPage(const PageAlignedAddress &physical_addr, const PageAlignedAddress &virtual_addr, size_t num_pages) {
	static constexpr int PRESENT_BIT 	= 0x01;
	static constexpr int READ_WRITE_BIT 	= 0x02;
	const size_t nVirtualAddress 	= virtual_addr.GetAddress();
	const size_t nPhysicalAddress 	= physical_addr.GetAddress();

	const size_t index_1 	= nVirtualAddress >> 22;
	const size_t index_2 	= ((nVirtualAddress >> 22) << 22) >> 12;

	os::Term::kprintf("sizeof(PageTable): %x\n", static_cast<unsigned int>(sizeof(PageTable)));
	// If my badly designed free was ever used, this may fail
	if (tables[index_1] == 0) {
		os::Term::kprintf("Mapping a page...\n");
		size_t allocated =os::kheap::kmalloc(sizeof(PageTable), true);
		os::Term::kprintf("allocated: %x\n", allocated);
		tables[index_1] = reinterpret_cast<os::paging::PageTable*>(allocated);
		os::Term::kprintf("tables[index_1] pointer: %x\n", reinterpret_cast<unsigned int>(tables[index_1]));
		//Doesn't work for some reason
		//tables[index_1] = new PageTable;
		os::Term::kprintf("sizeof(*tables[index_1]): %x\n", static_cast<unsigned int>(sizeof(*tables[index_1])));
	}
	tables[index_1]->pages[index_2].frame = 0;

	tables[index_1]->pages[index_2].frame = nPhysicalAddress >> 12;
	tables[index_1]->pages[index_2].present = 1;
	tables[index_1]->pages[index_2].rw 	= 1;
	m_PhysicalTables[index_1] = os::util::virtual_to_physical(reinterpret_cast<size_t>(&tables[index_2])) | (PRESENT_BIT | READ_WRITE_BIT);
	return PageStatus::OK;
}

PageStatus os::paging::PageDirectory::UnmapPage(const PageAlignedAddress &physical_addr) {

	// Has since been moved to be the responsibility of PageAlignedAddress class
	/*
	if ((physical_addr & 0x00000FFF != 0) || 
		(virtual_addr & 0x00000FFF != 0)) {
		return PageStatus::NOT_PAGE_ALIGNED;
	}
	*/
	return PageStatus::OK;
}
