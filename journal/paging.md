# Paging
## 2020-09-11T09:15:43+00:00
A few problems I ran into on the way
1) page directory needs to be page aligned. Apparently C++'s alignas specifier doesn't work for me for some reason.
2) A silly mistake of not setting placement_address to _heap_start for my temporary heap
3) Sometimes I forgot to convert to virtual_to_physical for the page table
4) Casting to a kmalloc is really a pain. Perhaps I should add a helper class for that.
5) Constructor can't send a return variable, needs to throw an exception... which isn't supported yet.
6) A bunch of things I have repressed from memory.
7) Sometimes I was using a pointer to a pointer instead of just the pointer itself (&ptr instead of ptr?)
8) I forgot to map the VGA text buffer, so it was actually working
except for that at one point, and crashed only when the terminal was being written to in my Tick interrupt. It took me a while to realise that since in the debugger it worked fine, not realising when my interrupt was fired it broke.
9) A bunch of other things I have repressed from my memory

Maybe I'll clean up the paging code later. Also A20 is now broken.
Also maybe look into liballoc before implementing a self-made heap. I believe liballoc_lock() and liballoc_unlock() is not needed
until multitasking/multithreading is done (as long as it isn't used in the timer interrupt anyway I think)

