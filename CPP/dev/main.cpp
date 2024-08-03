#include <cstdio>
#include <cstdlib>
#include <cstdint>

// Aliases:
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using s8  = int8_t;
using s16 = int16_t;
using s32 = int32_t; // int
using s64 = int64_t;

using f32 = float;
using f64 = double;


void print_byte(u8 data) {
    u8 mask = 0b1;
    for (int i = 7; i >= 0; --i) {
        printf("%d", (data >> i) & mask);
    }
    printf(" ");
}

void print_binary(void* data, std::size_t size) {
    u8* viewer = (u8*) data;
    for (std::size_t i = size; i > 0; --i) {
        print_byte(viewer[i - 1]);
    }
    printf("\n");
}

int main() {
    bool* a_bool = (bool*) malloc(sizeof(bool));
    *a_bool = true;
    u8* viewer = (u8*) a_bool;
    int x = -1;
    char c = '\0';
    char* str = "hello";
    u32 u = (1 << 31);
    f32 f = -1.1f;
    f64 d = -1.1;
    print_binary(&x, sizeof(int));
    print_binary(&c, sizeof(char));
    print_binary(str, sizeof("hello"));
    print_binary(&u, sizeof(u32));
    print_binary(&f, sizeof(f32));
    print_binary(&d, sizeof(f64));
    print_binary(a_bool, sizeof(bool));
}