#pragma once

#include "modern/allocator.h"
#include "modern/integer.h"
#include "modern/memory.h"
#include "modern/string.h"

typedef struct {
    u8 filename[100];
    u8 mode[8];
    u8 uid[8];
    u8 gid[8];
    u8 size[12];
    u8 mtime[12];
    u8 chksum[8];
    u8 typeflag[1];
} tar_Header;

typedef struct {
    tar_Header *header;
    String content;
} tar_File;

typedef array(tar_File) tar_FileArray;
typedef list(tar_File) tar_FileList;

address _tar_interpret_size(u8 raw_size[12]) {
    address result = 0;
    address count = 1;
    for (address i = 11; i >= 0; i--, count *= 8) {
        result += (raw_size[i] - '0') * count;
    }
    return result;
}

void tar_read(void *base, tar_FileList *result, Allocator *allocator) {
    while (*(u8 *)base) {
        tar_Header *header = base;
        address size = _tar_interpret_size(header->size);
        tar_File file = {
            .header = header,
            .content = (String) {
                .base = base + sizeof(tar_Header),
                .length = size,
            },
        };
        append(result, allocator, file);
        base += sizeof(tar_Header) + size;
    }
}
