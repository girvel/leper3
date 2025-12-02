// #pragma once
// 
// #include "modern/string.h"
// 
// #define FS_FILES_N 128
// 
// typedef struct {
//     address offset;
//     u8 filename[124];
// } __attribute__ ((packed)) fs_Header;
// 
// typedef struct {
//     fs_Header base[FS_FILES_N];
//     address length;
//     address capacity;
// } fs_HeaderBlock;
// 
// typedef struct {
//     Fat location;
//     fs_HeaderBlock *header_block;
// } fs;
// 
// void fs_init(fs *filesystem, Fat location) {
//     filesystem->location = location;
//     filesystem->header_block = location.base;
//     filesystem->header_block->length = FS_FILES_N;
//     filesystem->header_block->capacity = 0;
// }
// 
// void fs_list_files(String path, Strings *result) {
//     
// }
// 
// void fs_read_file(String path, DynamicString *result) {
//     
// }
