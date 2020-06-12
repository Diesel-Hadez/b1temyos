# B1TE-MY-OS
[Homepage](https://b1te.my/os/index.html)

## About
A hobby operating system cobbled together from various tutorials from around the web (See: [references](#references)). 
Written by [Me](https://b1te.my).

## Dependencies
Be sure to install each of the following and make sure it is available in your `$PATH` environment variable.
- `i686-elf-g++`
- `i686-elf-gcc`
- `i686-elf-ld`
- `qemu-system-i386`
- `nasm`

(Note that qemu isn't technically required if you do not wish to run the OS, or have some alternative way of running it, but using qemu
is the recommended way to run it)

Additionally, GNU Make is required as well to run the Makefile.
You may need to `mkdir obj` and `mkdir rel` first, as my `.gitignore` removed the directory for being empty.

Instructions on how to get the first three programs are available on the OSDev wiki [here](https://wiki.osdev.org/GCC_Cross-Compiler)
In short, you need to compile GNU's binutils and GCC (which includes g++ and not just gcc by the way, GCC stands for GNU Compiler Collection) 
and target it for the i686 architecture. Although you might be able to find pre-built binaries of it on the Internet, it might not be wise 
to use those as they may contain malicious code, or more likely, be of an older version which might cause you some trouble.

As for qemu, I found this [article](https://en.wikibooks.org/wiki/QEMU/QEMU_Hello_World:_Installing_QEMU_and_getting_it_up_and_running) which
shows how to compile qemu, just be sure to use `./configure --target-list=i686`, and for other settings you may want to configure, run
`./configure --help` for help. 

However, assuming you are running on Linux, it is likely that there exists safe pre-built binaries from whatever package
manager your distro is using. Just be aware that it is possible that some may not contain the specific i686 target needed, and may instead 
have additional packages with different names for specific targets of qemu. For example, on debian the `qemu` package is not the i686 target, 
and one must instead install the `qemu-system-x86` package. The reason the cross-compilers are less likely to be in these package managers
is because they are not used by many people, and so maintainers may find it a waste of time regularly updating those builds.

For Arch Linux and Arch-based distros, you can simply install these packages from the AUR (i686-elf-gcc and i686-elf-binutils) and 'extra' repositories (qemu and nasm) :-
- `qemu-arch-extra`
- `i686-elf-gcc`
- `i686-elf-binutils`
- `nasm`

## Installation
Once you have satisfied the [Dependencies](#dependencies), git clone this repo, cd into the root directory, and run `make all`, or
`make run` to run with qemu. The resulting raw image of the OS will be in the `rel/` directory.

## FAQ
- [Various Makefile Questions](#makefile-questions)
- [Misc. Questions](#misc-questions)

### Makefile Questions
#### Why don't you use wildcards or pattern rules in your Makefile?
I find them more trouble than they're worth. For example, what if you need to specify another directory? Your wildcard would
break. There are hacky ways to get around this, but they are extremely janky and rely on shell commands, which I want to try
to avoid as much as possible. Moreover, there are certains case where one file would rely on another, for example file `A.cpp`
including files `B.h` (which forward-declares functions/classes from `B.cpp`). Writing it out we could get this.
```makefile
#...
A.out : A.cpp B.cpp
	$(CC) ...
#...
``` 
whereas with a pattern rule would not be able to link dependencies like this. In short- I prefer to be completely explicit in
what is being compiled, even at the cost of convenience.
UPDATE: I have since started using pattern rules, and to solve the aforementioned problem have used the `-MMD` option to emit
dependency files. Wildcards however, I have not yet started to use.

#### Why don't you use CMake/SCons/<Insert Make alternative>?
This is somewhat planned.
I completely recognise how much cleaner CMake is than Make, such as by being a cleaner language, easier to do certain things,
makes easier cross-platform compatability, et cetera; but nonetheless I have doubts solely because I think that if I ever manage to create 
a self-sustainable OS (i.e: I can compile and further develop my OS _inside_ of on my own OS), it might be more difficult to 
compile CMake/SCons (the program itself) compared to GNU-Make? I'm not entirely sure about this, and I have my doubts I would ever 
even get that far, so I may change my mind in the future.

#### Compiling on non-Linux machines?
I recognise some things like the `dd` and `rm` command in the Makefile are **not** compatible with all platforms. But hard-coding
alternatives for _every_ platform can be tedious and clutter the Makefile up a bit. I am considering alternatives of using a 
cross-platform scripting language (eg.: Python) and writing my own scripts for doing these various cross-platform tasks, or using
CMake, which has some functionality for some actions which is automatically converted for each platform. Both solutions however
bring in yet another dependency to compile the OS, though it is arguably worth it.

#### Warnings for missing programs?
It would be nice for the Makefile to spit out an error if it cannot find the correct compilers/assemblers/etc and then terminate.
This is a planned feature, although it may be worth noting it would further exacerbate the problem of cross-compatability since
its use would likely rely on linux-specific shell commands.

### Misc. Questions

#### What is the `dd` for in the Makefile?
Apparently qemu (the virtual machine) requires at least thirty two 512-byte sectors in order to run (at least for the interface of an
IDE hard drive), as such this dd command seeks to the (512 * 32 - 1) th byte and inserts a single byte of zero there. This automatically
fills in everything preceeding this with zeroes as well, except what is already there. As a result, the file is "padded" with zeroes
to fit this file size. This trick was shamelessly stolen from [this stackexchange answer](https://unix.stackexchange.com/a/196727)
Another way around this would be to configure the Makefile to run qemu with `if=floppy` instead of `if=ide`, but I want to simulate a
hard drive as if I ever get around to running this on real hardware, I would more likely be using a hard drive (or even more likely a flash
 drive) instead of a floppy disk. 

#### Why not use clang instead of the gcc cross-compiler?
I'm planning on experimenting with that. The downside I see is that I haven't found a tutorial using clang instead of gcc 
for OSdev, with the best I could find being [this OSdev article](https://wiki.osdev.org/LLVM_Cross-Compiler), which itself states
is incomplete. This also means if I ever run into any trouble, it may prove difficult to find a solution online as less people use it 
for OSdev.

#### Why not use Rust then?
Way too little documentation (save for one excellent book linked below). Additionally I have not done too many projects with Rust yet, 
and am not as fluent in it as C++ anyway.

#### What are your goals?
In many ways, my goals are similar to that of a [Lino Commmando](https://wiki.osdev.org/Lino_Commando). In fact, besides wanting to
write my own bootloader instead of utilising GRUB, my goals are exactly that of Lino Commando. Getting this far is when I would consider
this project "complete", and I would consider any additional commits thereafter more of an experiment rather than further development.

#### Why not use `i686-elf-as` instead of `nasm`?
I find the NASM syntax to be a bit clearer, even with AS's `.intel_syntax` option. Regardless, since they (AS and G++)are both GCC,
there might be better integration, so if the situation arises where it would be easier to use AS I might switch to it in the future.

#### Why not use C instead of C++?
I'm more comfortable with C++ than C and enjoy its object-oriented capabilities (though I do not overuse them to the point of using
questionable and/or controversial design practices) I still see some problems with using C++ specifically for OSdev though, 
with it being (what I consider) a slightly higher-level language than C, some things need to be done to be able for C++ compilers 
to work properly: See my TO-DO on C++ for more information. This decision, though I am somewhat comfortable with now, may come back 
to haunt me in the future (It may..._bite my_ back).

#### Graphics Mode?
I'm actually comfortable with the current VGA Text Mode. I might play around with making a VGA driver in the future for 320x200 256 colors 
mode or- since I am using a custom bootloader- play around by specifying a different graphics mode using a BIOS function as detailed [here](https://wiki.osdev.org/User:Omarrx024/VESA_Tutorial). This would be possible but more difficult in Protected Mode. What I will definitely **not** and will **never** do is write an [accelerated graphics card driver](https://wiki.osdev.org/Accelerated_Graphic_Cards), which is probably a lot smoother and what any modern OS uses.

#### Filesystem Access?
Planned, but first I need a way to access the hard drive and read sectors from it, which is currently on my TO-DO. Most likely I would
end up with an old way of reading data from a disk, which would be slow, but I'm fine with that. 

#### License?
I currently have no license attached, which means I hold the copyright. Nonetheless, I am planning to add a permissive open-source license
in the future which just needs to credit me in some form. In the mean time, try go through some of the excellent [resources](#references) below,
which is a lot more useful than this project in its current form.

#### Why the name?
Not actually my first choice of a name for an OS, but this is the name of my website [https://b1te.my](https://b1te.my) with the word "OS" tacked on to it. Note that if you are reading this in the future, the domain may have expired by now. Apparently cool domain names are expensive.
Anyway, B1TEMYOS is actually a codename, and is subject to a rename in the future when I think up a better name.
And for the curious, I used that domain name as a reference to part of a catchphrase of a TV show, as well as using the number 1 instead of
the letter 'i' as a reference to the binary system, though unfortunately I had nowhere to put a 0 (Hmm...B1teMy0S anyone?)

## TO-DO
In no particular order:-
-  **Enable A20 line** - See [OSDev article](https://wiki.osdev.org/A20_Line) or [this forum thread](https://forum.osdev.org/viewtopic.php?f=8&t=32115&start=0). 
As far as I can tell most tutorials don't mention it because they use GRUB, which enables the A20 line for us (source: A20 Line 
article on the osdev wiki). Additionally, according to [this](https://stackoverflow.com/a/33827632) stackoverflow post, jumping to 
protected mode does *not* automatically enable the A20 line. According to the wiki, enabling of the A20 line can be done in either 
Real or Protected Mode. Also of note is that qemu [automatically enables the A20 line](https://www.reddit.com/r/osdev/comments/a20q4j/change_status_of_a20_line_when_qemu_starts/) and the trick mentioned of reading from IO location 0xEE does not seem to do the trick for me. As such,
I would not be able to test any A20 line setting code without recompiling SeaBIOS for Qemu, or perhaps [using VirtualBox instead](https://forum.osdev.org/viewtopic.php?f=1&t=33169) or _some_ real hardware.

- **Terminal printing** - Things like kprintf(), etc. It might be a good idea however to make a minimal interface first, then
refactor once a working heap is implemented, since , for example, I cannot do some things such as returning a pointer in a function
since I would need it to be allocated on the heap.

- Done ~~**Re-initialise GDT**~~ - See any one of the tutorials linked in the [references](#references) section. Despite being initialised in
the bootloader, as far as I can tell the GDT _might_ still need to be reloaded. If instead GRUB was used instead of our own bootloader,
the memory address of the GDT is unknown (and even if it was, I supposed it might not be reliable and could be changed in different GRUB
versions), and thus the GDT would **have** to be re-initialised with a new known memory address. Since here our own custom bootloader
is being used, this may very well not be needed, though I am unsure about that. In any case, you might want to set two more additional
segments for User Mode Code and User Mode Data as per James M's tutorials, and perhaps the Task Segment Selector in the future.

- **IDT** - Again just follow a tutorial, and cross-reference with other tutorials/Known Bugs as well.
- **IRQs and PIT** - Again just follow a tutorial, and cross-reference with other tutorials/Known Bugs as well.
- **Keyboard** - Actually this is technically part of IRQ.
- **Paging** - Again just follow a tutorial, and cross-reference with other tutorials/Known Bugs as well.

- **ATA Driver** - This is the difficult one. Try looking either at the skelix tutorial or youtube tutorial mentioned in the [references](#references). Also of interest is [OSDev's article on ATA PIO Mode](https://wiki.osdev.org/ATA_PIO_Mode) and [this forum thread](https://forum.osdev.org/viewtopic.php?t=12268) linked in the aforementioned article, which uses 28 bit PIO but 48 bit PIO using the article as reference shouldn't
be too hard to adapt to. This is, by the way, the answer to the second-last reply of the thread. I would be satisfied with just being
able to read/write sectors. A filesystem like ext4/ext2/FAT32/FAT16/etc could be implemented later on. 
I would also be satisfied even if reading speeds are slow, which I suspect may be the case, as I suspect PIO mode to be a legacy feature.

- **FAT12 Driver** - See: [Wikipedia](https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system), and search elsewhere as well. 
Interestingly, it may seem that I would have to slightly rewrite my bootloader- though I still have space so that should hopefully not be a problem, else I could split my bootloader into two separate stages.

- **Port IO Assembly Code** - Currently I use inline assembly from the Developing an Operating System From Scratch book. I wonder if it would
be a better idea to code it in assembly, and link it to the C++ code. One potential problem with this is that I want inb/outb to be part
of a namespace for organisation purposes, which I think is possible but a bit of research would need to be done first.

- ~~**C++ Fixing**~~ - UPDATE: I have since implemented calling global constructors/destructors, but I think there is still much to be done. For example, I stumbled upon a problem of the C++ Compiler needing the atexit() function for Singletons, so be aware of this. ~~See [OSDev's article on C++](https://wiki.osdev.org/C%2B%2B) and [OSDev's article on Calling Global Constructors](https://wiki.osdev.org/Calling_Global_Constructors) as well as [OSDev's article on C optimisations](https://wiki.osdev.org/C#Things_you_should_know_about_optimizations), the latter which also applies to C++. Another forum thread regarding C++ can be found [here](https://forum.osdev.org/viewtopic.php?f=1&t=29216&start=0). Additionally, the youtube series in the [references](#references) may be of help; looking at his `linker.ld` 
and `kernel.cpp`, he seems to have `.init_array` as a list of constructors, and has `start_ctors` and `end_ctors` as the index of the 
beginning and end of this list respectively. These are then called in a function in `kernel.cpp`, which itself is called by `loader.s` 
manually. There does _not_ seem to exist any similar functionality for destructors, and furthermore, I _suspect_ that the constructors to 
be called are locked in to being a type where it does not accept any parameters because of the typedef of the function pointer in 
`kernel.cpp` being of a type `void()`. Try follow the OSDev's wiki article on calling Global Objects and do some tests with classes 
and constructors/destructors to see if that works first.~~

- **Debugging interface** - See [here](https://github.com/cfenollosa/os-tutorial/blob/master/22-malloc/Makefile)

- **Cleaner Code** - General ideas would be in the obj/ directory having subdirectories inside, allowing for multiple source code  files with the same name but different directories.

- **Better Makefile** - I think there are issues with object files all being in the same folder, potentially a problem if there are multiple
files with the same name but different directories. Also, for different directories in the kernel there is repeated code. I might have to use
`$(find X)` to avoid repetition.

- KDevelop4 files - Probably nicer to use an IDE when/if this gets bigger and then add the KDevelop files to gitignore.

- **Homepage** - It doesn't exist.

## Future
Some features I consider too optimistic or do not nearly know enough about, but may be able to handle say, 10 years down the line.
- DMA - Use for access for moving large amounts of memory. I also suspect this may be key in faster hard drive reading/writing?
- GUI/Graphics - Unlike others in this list, I actually think I am currently capable of doing this if I get Graphics done, but simply choose not to
- Networking - Hah I seriously doubt I'll get to this. Nonetheless, if an ambitious reader (or an ambitious future-me) would like to attempt 
this, perhaps using the Ethernet port would be a good start, and besides the osdev wiki there is Viktor's Youtube series linked in [references](#references)
- ELF file execution - I would need to get paging and filesystems working first. Perhaps a precursor to this could be a readelf type program.
To do this, maybe implement a custom syscall for executing a file.
- A good Heap implementation - Although James M's tutorial covers this, the wiki states it is not a terribly good solution and suggests
implementing one yourself, which I currently find somewhat difficult.
- Usermode - Although James M's tutorial covers this, I have slight doubts in my ability to implement this as well- although hopefully
if I get a working Heap this may be easier. Little OS Book does cover this though, so I might be able to do so, I do not want a monolithic
kernel and would rather a microkernel after all.
- Graphics Driver - Hah I'll never get to this, not even in 10 years. To an ambitious reader, all I have to say is: Good luck. Maybe PDoane's OS Source code would be on interest to you.
- Multitasking - Although James M's tutorial covers this, the wiki compares the approach used to "insanity", See perhaps these OSDev articles
[Brendan's Multitasking Tutorial](https://wiki.osdev.org/Brendan%27s_Multi-tasking_Tutorial) and [Multitasking Systems](https://wiki.osdev.org/Multitasking_Systems)
- USB Driver - Haven't the foggiest as to where to start on this.
- libcpp - Will never get to this. A c++ standard library. Best I'll do is my own small library or use a port something.
- ncurses port?
- PCI Communication
- "Message Passing" - ala IPC communication
- compiler/assembler - If I manage to get ELF file execution, perhaps one of these written/ported so that the OS could be self-hosted.
- Audio - Haven't the foggiest as to where to start on this.
- Loading on real hardware / a flash drive - I guess I hypothetically might be able to achieve this after a while, but the problem is 
that I do not want to risk messing up my hardware.
- CMake - I expanded more on this in the FAQ above.
- A [PKGBUILD](https://wiki.archlinux.org/index.php/PKGBUILD) could be made. Also see [Creating packages](https://wiki.archlinux.org/index.php/Creating_packages)
- There is a "Serial Port Printing" referenced in the osdev wiki ([here](https://wiki.osdev.org/What_order_should_I_make_things_in)),
which I assume is a process somewhat similar to an Arduino sending data over a Serial Port, but for the life of me I can not find out 
how to implement this. I know the little os book (linked in the references) has a chapter on serial communication, but without knowing 
what serial port it is, I cannot send data to it. One thing i have found is that you probably need to use the option `-serial stdio` when
running with qemu, see: [OSDev QEMU article](https://wiki.osdev.org/QEMU). Also see `man qemu` or try sending data to the COM1/SerialPortA port.Also see [this commit](https://github.com/klange/toaruos/commit/52a5b9e6cfda90108fafb3e2eba161d8b8d8e320) of toaruos

## References
Note: At the time of writing, I have only used Dr Nicholas Blundell's and James M's tutorials, and have
not yet incorporated all these tutorials into the OS. They do, however, represent what I plan to use as reference.

- [OSDev Wiki](https://wiki.osdev.org/) - Extremely good with lots of information, but sometimes not very well-written 
(written like a personal journal), sometimes incomplete, and sometimes has a less-than-forgiving community. Be sure to 
go to the linked references at the bottom of the wiki pages for further info.
- Various external tutorials the osdev wiki links to [here](https://wiki.osdev.org/Tutorials#Third_Party_Tutorials), particularly the 
following, also note that some of these tutorials have pages in the OSdev wiki documenting known bugs in them and ways to fix it.
	- [Writing an Operating System - From Scratch by Dr Nicholas Blundell](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf) - Incomplete but extremely well-written. An excellent start to writing a bootloader.
	- [James A Molloy's Kernel Tutorials](http://jamesmolloy.co.uk/tutorial_html/) | [Known Bugs](https://wiki.osdev.org/James_Molloy%27s_Tutorial_Known_Bugs)
	- [BrokenThorn Operating System Development Series](http://www.brokenthorn.com/Resources/OSDevIndex.html) | [Known Bugs](https://wiki.osdev.org/Brokenthorn%27s_Known_Bugs)
	- [Bran's Kernel Tutorial](http://www.osdever.net/bkerndev/Docs/title.htm) | [Known Bugs](https://wiki.osdev.org/Bran%27s_Known_Bugs)
- [Writing an OS in Rust](https://os.phil-opp.com/) despite being in Rust and being incomplete as of time of writing, this still is an invaluable resource, and also is probably the most up-to-date tutorial on Operating Systems there is available for free online.
- [Skelix OS](http://skelix.net/skelixos/index_en.html) In particular tutorial 7 regarding hard drive access seems useful. Found this 
through early commits of toaruos linking this as a reference tutorial.
- [Little OS Book](https://littleosbook.github.io/) Pretty nice all round, could use as a replacement for James M's tutorial and it has
neat ways to go into usermode and has a chapter on serial communication, which I have not found elsewhere.
- [Writing your own OS - By Viktor \[YT\]](https://www.youtube.com/watch?v=1rnA6wpF0o4&list=PLHh55M_Kq4OApWScZyPl5HhgsTJS9MZ6M) Another 
completely invaluable, very comprehensive, and _clear_ (_cough_ osdev wiki _cough_) resource, with the only downside is of it 
being in a video form instead of in a written medium (although I admit this is a subjective preference of the learning process, it still
takes up a lot more time than reading an article/manual/book of the same content. As a compromise I watch it in 2X speed to slightly hasten
the process, or else just use his operating system's [source code](https://github.com/AlgorithMan-de/wyoos) as reference). I'm not entirely
sure it follows best practices all the time (I have, for example, found a potential problem with constructors/destructors), but still a 
nice resource, and perhaps this was a one-off mistake, or I am mistaken entirely and this was correct.
- [Reddit OSDev Community](https://www.reddit.com/r/osdev/) - More of a community forum than a large resource of information, but
there are sometimes good explanations in comments, if you can find them.

### Others
- Additionally, although I personally have not read it due to its high cost, I have heard good things about this book and it is cited as the natural next-step after reading the book by Dr Nicholas Blundell above. The book's title is `Operating Systems Design and Implementation (3rd Edition) - Andrew S Tanenbaum, Albert S Woodhull`
- [Operating Systems: From 0 to 1](https://github.com/tuhdo/os01) is incomplete, but the explanations of the low-level way the actual
circuitry in a computer works is interesting and may help in understanding certain OSDev concepts. If you are more of a hardware person
by the way, check out [Ben Eater](https://www.youtube.com/user/eaterbc) on youtube if you haven't already, or the [Lessons in Electric Circuits](https://www.allaboutcircuits.com/textbook/) which the book suggests to read if you don't know much about electric circuits.
- [cfenollosa's os-tutorial](https://github.com/cfenollosa/os-tutorial) Incomplete, but could be useful. For example, this is where I found
out about qemu's debugging interface (in the Makefile) and it was very thoughtful of the author to include "keywords" of concepts 
in each chapter so that the reader would know what to search for for research. ("for for" is intentional and I _think_ grammatically correct)
- [samypesse how-to-create-an-os](https://samypesse.gitbook.io/how-to-create-an-operating-system/chapter-6) could be useful.
- [Independent Software](http://www.independent-software.com/operating-system-development.html) - Haven't looked too much into this, it sticks
out to me particularly because it seems to have a chapter on the A20 line.
- [Early versions of ToaruOS](https://github.com/klange/toaruos/commits/master?after=571cda2928ef335af3418c77280dbf8c91f1f573+4568) Fun fact: I
first found out about this OS through its off-spring PonyOS. I am not entirely sure, but I think PonyOS was the first OS I found out
about years ago which wasn't mainstream, and what got me interested in OS Development. Note that you shouldn't copy code from there, just see how things are implemented, because its license requires an acknowledgement, which I would rather not have; Besides, copying code is not fun.
- [Pdoane's OS Source Code](https://github.com/pdoane/osdev/) - It's meant to target a 64 bit machine rather than my 32 bit (i686), but it's a
really easy to understand and read OS.
- [Raspberry Pi OS](https://github.com/s-matyukevich/raspberry-pi-os) - Actually seems like a really good guide to OSDev, although it is
incomplete and doesn't have a tutorial on reading sectors for the SD Card or a flash drive.
- [VGA Hardware](http://www.osdever.net/FreeVGA/vga/vgareg.htm) Some info about the VGA Hardware
- [SkiftOS](https://github.com/maker-dev/skift) (Also: [Reddit link](https://www.reddit.com/r/osdev/comments/aazvhd/after_5_months_of_working_on_my_operating_system/)) The great thing about this is that it is MIT licensed!
- [K-OS](https://github.com/khoek/k-os)
