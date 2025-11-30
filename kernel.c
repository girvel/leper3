void run();

// NOTICE: should come first!
int main() {
    run();
    while(1);
    return 0;
}

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef u32 size_t;

typedef struct {
    u8 *base;
    size_t length;
} String;

typedef struct {
    u8 character;
    u8 color;
} __attribute__((packed)) VideoMemory;

#define VIDEO_MEMORY_ADDRESS 0xb8000
#define VIDEO_MEMORY_W 80
#define VIDEO_MEMORY_H 25

#define FG_BLUE 0x1
#define FG_WHITE 0xF

int x = 0;
int y = 0;

void write(String str) {
    VideoMemory *video_memory = (VideoMemory *)VIDEO_MEMORY_ADDRESS;

    for (size_t i = 0; i < str.length; i++) {
        u8 character = str.base[i];

        if (character == '\n') {
            x = 0;
            y++;
        } else {
            VideoMemory *cell = &video_memory[y * VIDEO_MEMORY_W + x];
            cell->character = character;
            cell->color = FG_WHITE;
            x++;
        }

        if (x == VIDEO_MEMORY_W) {
            x = 0;
            if (y == VIDEO_MEMORY_H) {
                y = 0;
            } else {
                y++;
            }
        }
    }
}

void run() {
    write((String) {.base = (u8 *)"Hello, world!\nHello from Leper OS!", .length = 34});
}
