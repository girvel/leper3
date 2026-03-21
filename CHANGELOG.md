# CHANGELOG

## 3.0-alpha.2: playground

Leper OS as a playground for C or C++ development, basically a microkernel that gets you NULL, graphics, keyboard & direct access to memory.

- No "modern"
- Do the .c/.h split
- No allocators, no pages, rawdogging stack
- Maybe nob-based build system, or even better -- a CLI app
- Exported api folder

- [x] idt.c
- [x] io.c
- [x] clock.c
- [x] kb.c
- [x] power.c
- [x] random.c
- [ ] allow bigger kernel
- [ ] vmm.c
- [ ] remove: arena, fs, heap, page, tar, tty;
- [ ] vga.c: redo, split & document
- [ ] a simple userspace app to link with kernel
- [ ] string.c

- [ ] Remove allocators & "modern"
- [ ] Make the CLI

