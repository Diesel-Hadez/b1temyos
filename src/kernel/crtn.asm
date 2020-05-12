[section .init]

;GCC will put crtend.o's .init section here

pop ebp
ret

[section .fini]

;GCC will put crtend.o's .fini section here

pop ebp
ret
