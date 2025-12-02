#!/usr/bin/env luajit

--- @param cmd string
--- @param ... any
local cmd = function(cmd, ...)
  cmd = cmd:format(...)

  print("[CMD]", cmd)
  local code = os.execute(cmd)

  if code ~= 0 then
    print("[ERROR] Exited with code", code)
    os.exit(1)
  end
end

cmd("mkdir -p .build")
cmd("nasm -f bin boot.asm -o .build/boot.bin")
cmd("gcc -ffreestanding -m32 -fno-pie -c src/kernel.c -o .build/kernel.o")
cmd("ld -o .build/kernel.bin -Ttext 0x1000 -e main --oformat binary -m elf_i386 .build/kernel.o")
cmd("cat .build/boot.bin .build/kernel.bin > .build/leper3.bin")
cmd("dd if=/dev/zero bs=512 count=50 >> .build/leper3.bin 2>/dev/null")
cmd("qemu-system-x86_64 -drive format=raw,file=.build/leper3.bin")
