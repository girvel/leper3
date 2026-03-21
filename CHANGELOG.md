# CHANGELOG

## 3.0-alpha.2: playground

Leper OS as a playground for C or C++ development, basically a microkernel that gets you NULL, graphics, keyboard & direct access to memory.

- No "modern"
- No allocators, no pages, rawdogging stack
- Maybe nob-based build system, or even better -- a CLI app
- Do the .c/.h split
- Exported api folder

### Stage 1: remove "modern" from all the modules

- [x] idt.c
- [ ] string.c
- [ ] io.c
- [ ] clock.c
- [ ] kb.c
- [ ] power.c
- [ ] random.c
- [ ] kernel.c

### Stage 2: restructure the project

- [ ] Do the .c/.h split
- [ ] Separate the user application from the kernel
- [ ] Make the CLI

