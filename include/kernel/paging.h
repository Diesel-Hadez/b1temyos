#ifndef PAGING_H
#define PAGING_H
#include <stdint.h>
#include <stddef.h>

namespace os {
	namespace paging {
		struct PageEntry {
			uint32_t present 	: 1;
			uint32_t rw		: 1;
			uint32_t user 		: 1;
			uint32_t access		: 1;
			uint32_t dirty		: 1;
			uint32_t unused 	: 7;
			uint32_t frame		: 20;
		};
		
		struct PageTable {
			PageEntry pages[1024];
		};

		class MemoryAddress {
			protected:
				size_t m_Address;
			public:
				size_t GetAddress() const;
				explicit MemoryAddress(size_t addr);
				MemoryAddress() = delete;
				virtual ~MemoryAddress()=0;
		};

		// I have a hunch something about Move Semantics
		// can improve this. Something like
		// `void Foobar(PhysicalAddress a);`
		// would be called by
		// `Foobar(PhysicalAddresss(0x10000))`
		// And the default copy constructor would be
		// used instead of "moving" PhysicalAddress
		// to Foobar
		class PhysicalAddress : public MemoryAddress {
			private:
			public:
				explicit PhysicalAddress(size_t addr);
				PhysicalAddress() = delete;
				~PhysicalAddress() = default;

		};

		class VirtualAddress: public MemoryAddress {
			private:
			public:
				explicit VirtualAddress(size_t addr);
				VirtualAddress() = delete;
				~VirtualAddress() = default;

		};

		class PageAlignedAddress: public MemoryAddress {
			private:
			public:
				explicit PageAlignedAddress(size_t addr);
				PageAlignedAddress() = delete;
				~PageAlignedAddress() = default;

		};
		class PageDirectory;
		void SwitchPageDirectory(const PageDirectory& pd);
		enum PageStatus {
			NOT_PAGE_ALIGNED,
			USED_PAGE,
			OK
		};
		class PageDirectory {
			friend void os::paging::SwitchPageDirectory(const PageDirectory&);
			private:
				// array of pointers to tables
				PageTable * tables[1024];
				// Used for cr0
				size_t m_PhysicalTables[1024];
			public:
				PageStatus MapPage(const PageAlignedAddress &physical_addr, const PageAlignedAddress &virtual_addr, size_t num_pages=1);
				PageStatus UnmapPage(const PageAlignedAddress &physical_addr);
				PageDirectory();
				~PageDirectory() = default;
		};
		void Init();
		// Reloads cr3
		void Reload();
	}
}

#endif
