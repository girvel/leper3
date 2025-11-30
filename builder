#!/usr/bin/env luajit

--- @param cmd string
--- @param ... any
local cmd = function(cmd, ...)
  os.execute(cmd:format(...))
end

cmd("mkdir -p .build")
cmd("nasm -f bin boot.asm -o .build/boot.bin")
do
  local f = assert(io.open(".build/kernel.bin", "wb"))
  f:write("X")
  for _ = 1, 511 do
    f:write("\0")
  end
  f:close()
end
cmd("cat .build/boot.bin .build/kernel.bin > .build/leper3.bin")
cmd("qemu-system-x86_64 -drive format=raw,file=.build/leper3.bin")
