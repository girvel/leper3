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

--- @param filename string
--- @return string
local read_file = function(filename)
  local f = assert(io.open(filename, "rb"))
  local result = f:read("*a")
  f:close()
  return result
end

local cc_flags = read_file("compile_flags.txt"):gsub("\n", " ")

--- @param source string
--- @return string destination
local cc = function(source)
  assert(source:sub(-2) == ".c")
  local destination = source:sub(1, -2) .. "o"
  cmd("gcc %s -c src/%s -o .build/%s", cc_flags, source, destination)
  return destination
end

cmd("mkdir -p .build")
cmd("nasm -f bin boot.asm -o .build/boot.bin")
cmd("nasm -f elf32 src/isr.asm -o .build/isr.o")
cc("kernel.c")
cc("string.c")
cc("io.c")
cmd("ld -o .build/kernel.bin -Ttext 0x1000 -e main --oformat binary -m elf_i386 .build/kernel.o .build/string.o .build/io.o .build/isr.o")

do
  local SECTOR_SIZE = 512
  local KERNEL_MAX_SIZE = 50 * SECTOR_SIZE
  local NORMAL_SIZE = 10 * 1024 * 1024

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

    leper3_bin:write(("\0"):rep(NORMAL_SIZE - KERNEL_MAX_SIZE))
  leper3_bin:close()
end

cmd("qemu-system-x86_64 -drive format=raw,file=.build/leper3.bin")
