#!/usr/bin/env luajit

local parameters = {
  source_folder = "src",
  bootloader = "src/boot.asm",
  kernel = "src/kernel.cpp",
  flags = "compile_flags.txt",
}

----------------------------------------------------------------------------------------------------
-- [SECTION] Tools
----------------------------------------------------------------------------------------------------

--- @param str string
--- @param pat string
--- @param plain boolean?
--- @return string[]
local split = function(str, pat, plain)
  local t = {}

  while true do
    local pos1, pos2 = str:find(pat, 1, plain or false)

    if not pos1 or pos1 > pos2 then
      t[#t + 1] = str
      return t
    end

    t[#t + 1] = str:sub(1, pos1 - 1)
    str = str:sub(pos2 + 1)
  end
end

--- @param str string
--- @param postfix string
--- @return boolean
local ends_with = function(str, postfix)
  return str:sub(-#postfix, -1) == postfix
end

--- @alias log_f fun(fmt: string, ...: any)
--- @class log
--- @field cmd log_f
--- @field info log_f
--- @field error log_f

--- @type log
local log = setmetatable({}, {
  __index = function(self, level)
    return function(fmt, ...)
      print("[" .. level:upper() .. "]", fmt:format(...))
    end
  end
})

--- @param command string
--- @param ... any
local cmd = function(command, ...)
  command = command:format(...)

  log.cmd(command)
  local code = os.execute(command)

  if code ~= 0 then
    log.error("Exited with code %s", code)
    os.exit(1)
  end
end

--- @param command string
--- @param ... any
--- @return string
local cmd_read = function(command, ...)
  local f = assert(io.popen("zsh -c '" .. command:format(...) .. "'"))
  local result = f:read("*a")
  f:close()

  if result:sub(-1, -1) == "\n" then
    result = result:sub(1, -2)
  end
  return result
end

--- @param path string
--- @return string
local read_file = function(path)
  local f = assert(io.open(path, "r"))
  local result = f:read("*a"):gsub("\n", " ")
  f:close()
  return result
end

--- @param source string
--- @param flags string
--- @return string
local gcc = function(source, flags)
  local object_file = ".build/" .. source:sub(1, -5) .. ".o"
  cmd("g++ %s -c %s -o %s", flags, source, object_file)
  return object_file
end

--- @param source string
--- @param format "bin"|"elf32"
--- @return string
local nasm = function(source, format)
  local object_file = ".build/" .. source:sub(1, -3) .. (format == "bin" and ".bin" or ".o")
  cmd("nasm -f %s %s -o %s", format, source, object_file)
  return object_file
end

----------------------------------------------------------------------------------------------------
-- [SECTION] Steps
----------------------------------------------------------------------------------------------------

--- @param folder string
--- @return string[]
local discover_sources = function(folder)
  log.info("Discovering source files...")
  return split(cmd_read("ls %s/**/*.{cpp,asm}", folder), "\n", true)
end

--- @param sources string[]
local create_build_directories = function(sources)
  log.info("Creating build directories...")
  local seen = {}
  for _, path in ipairs(sources) do
    local base = path:match("^(.*)/[^/]+$")
    if not seen[base] then
      cmd("mkdir -p .build/%s", base)
      seen[base] = true
    end
  end
end

--- @param sources string[]
--- @param flags string
--- @param kernel string
--- @param bootloader string
--- @return string, string[]
local compile_sources = function(sources, flags, kernel, bootloader)
  log.info("Compiling sources...")

  local kernel_out
  local object_files = {}
  for _, source in ipairs(sources) do
    if ends_with(source, ".cpp") then
      local file = gcc(source, flags)
      if source == kernel then
        kernel_out = file
      else
        table.insert(object_files, file)
      end
    elseif source ~= bootloader then
      table.insert(object_files, nasm(source, "elf32"))
    else
      nasm(source, "bin")
    end
  end
  return kernel_out, object_files
end

--- @param kernel string
--- @param object_files string[]
local link = function(kernel, object_files)
  log.info("Linking...")
  cmd(
    "ld -o .build/kernel.bin -Ttext 0x1000 -e main --oformat binary -m elf_i386 %s %s",
    kernel,
    table.concat(object_files, " ")
  )
end

local write_image = function()
  log.info("Writing the OS image...")

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

local run_image = function()
  cmd("qemu-system-x86_64 -drive format=raw,file=.build/leper3.bin")
end

----------------------------------------------------------------------------------------------------
-- [SECTION] Main
----------------------------------------------------------------------------------------------------

local sources = discover_sources(parameters.source_folder)
create_build_directories(sources)
local kernel, objects = compile_sources(sources, read_file(parameters.flags), parameters.kernel, parameters.bootloader)
link(kernel, objects)
write_image()
run_image()
