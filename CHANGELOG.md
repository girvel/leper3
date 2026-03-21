# CHANGELOG

## 3.0-alpha.2: playground

Leper OS as a playground for C or C++ development, basically a microkernel that gets you NULL, graphics, keyboard & direct access to memory.

- No "modern"
- Do the .c/.h split
- No allocators, no pages, rawdogging stack
- Maybe nob-based build system, or even better -- a CLI app
- Exported api folder

### Stage 1: remove "modern", split into .c and .h, document

- [x] idt.c
- [ ] string.c
- [x] io.c
- [x] clock.c
- [ ] kb.c
- [ ] power.c
- [ ] random.c
- [ ] kernel.c

### Stage 2: restructure the project

- [ ] Remove allocators & "modern"
- [ ] Separate the user application from the kernel
- [ ] Make the CLI

