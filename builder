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

-- cmd("cat .build/boot.bin .build/kernel.bin > .build/leper3.bin")
do
  local SECTOR_SIZE = 512
  local KERNEL_MAX_SIZE = 50 * SECTOR_SIZE

  local leper3_bin = assert(io.open(".build/leper3.bin", "wb"))
    local boot_bin = assert(io.open(".build/boot.bin", "rb"))
      local content = boot_bin:read("*a")
    boot_bin:close()

    assert(#content == 512)
    leper3_bin:write(content)

    local kernel_bin = assert(io.open(".build/kernel.bin", "rb"))
      content = kernel_bin:read("*a")
    kernel_bin:close()

    local is_below_limit = #content < KERNEL_MAX_SIZE
    print(("[%s] Kernel size %.2f / %.2f KB"):format(is_below_limit and "INFO" or "ERROR", #content / 1024, KERNEL_MAX_SIZE / 1024))
    if not is_below_limit then os.exit(1) end

    leper3_bin:write(content)
    leper3_bin:write(("\0"):rep(KERNEL_MAX_SIZE - #content))
  leper3_bin:close()
end

cmd("qemu-system-x86_64 -drive format=raw,file=.build/leper3.bin")
