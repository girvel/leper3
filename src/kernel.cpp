void run();

extern "C" int main() {
    run();
    return 0;
}

struct __attribute__((packed)) vga_Cell {
    char character;
    char color;
};

#define VGA_VIDEO_MEMORY_ADDRESS 0xb8000

void run() {
    ((vga_Cell *)0xb8000)->character = '+';
}
