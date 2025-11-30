int main() {
    char *video_memory = (char *)0xb8000;
    *video_memory = 'C';
    *(video_memory + 1) = 0x1f;
    return 0;
}
