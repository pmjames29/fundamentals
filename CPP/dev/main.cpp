#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <sys/mman.h>
#include <utility>
#include <cstring>

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

/**
0 1 2 3 4 5 6 7 
  x
*/

template<typename T>
T align(T data, s64 align) {
    s64 a = (s64) data;
    if (a % align != 0) {
        a += align - a % align;
    }
    return (T) a;
} 




struct Arena {
    void* memory;
    s64 align = 8;
    u8* end;
    u8* cursor;
};

struct Does_Things {
    Does_Things() {
        printf("I'm doing things\n");
    }
    
    ~Does_Things() {
        printf("I'm done do things\n");
    }
};

static const size_t TWO_GIGS = (size_t) (1024) * 1024 * 1024 * 2;

void init(Arena* arena, size_t count = TWO_GIGS) {
    auto& a = *arena;
    
    auto memory = mmap(NULL, count, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    a.memory = memory;
    a.end = (u8*) memory + count;
    a.cursor = (u8*) memory;
}

#define RESET_AT_POINT(arena) \
auto current_point = arena.cursor; \
defer { arena.cursor = current_point; }

void* alloc(Arena* arena, size_t count) {
    auto& a = *arena;
    
    a.cursor = align(a.cursor, a.align);
    auto new_cursor = a.cursor + count;
    if (new_cursor > a.end) {
        printf("Out of memory\n");
        return NULL;
    }
    auto ret = a.cursor;
    a.cursor = new_cursor;
    return ret;
}

void* realloc(Arena* arena, void* prev, size_t old_count, size_t new_count) {
    if (new_count <= old_count) return prev;
    auto& a = *arena;
    if (prev == a.cursor - old_count) {
        auto diff = new_count - old_count;
        auto new_cursor = a.cursor + diff;
        if (new_cursor > a.end) {
            printf("Out of memory\n");
            return NULL;
        }
        a.cursor = new_cursor;
        return prev;
    }
    auto ret = alloc(arena, new_count);
    memcpy(ret, prev, old_count);
    return ret;
}

template<typename Code>
struct Defer {
    Code code;
    Defer(Code block) : code(block) {}
    ~Defer() { code(); }
};
struct Defer_Generator { template<typename Code> Defer<Code> operator +(Code code) { return Defer<Code>{code}; } };
#define GEN_DEFER_NAME_HACK(name, counter) name ## counter
#define GEN_DEFER_NAME(name, counter) GEN_DEFER_NAME_HACK(name, counter)
#define defer auto GEN_DEFER_NAME(_defer_, __COUNTER__) = Defer_Generator{} + [&]()

template<typename T, size_t Count>
struct Array_Fix {
    T data[Count];
    
    T& operator [] (s64 idx) {
        return data[idx];
    }
};

struct Array_Dyn {

};

struct Array_View {

};


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
    
    Arena arena;
    init(&arena);
    arena.align = 1;
    printf("%p\n", arena.memory);
    a_bool = (bool*) alloc(&arena, sizeof(bool));
    printf("%p\n", arena.cursor);
    a_bool = (bool*) realloc(&arena, a_bool, sizeof(bool), sizeof(bool) * 3);
    printf("%p\n", arena.cursor);
    
    Does_Things dt;
    auto lamb = [&] () { alloc(&arena, 2); printf("Hello lambda world!\n"); };
    lamb();
    
    // Calling defer here will reference the macro defined above
    defer { printf("Hello defer world 1!\n"); };
    defer { printf("Hello defer world 2!\n"); };
    
    // for (int i = 0; i < 10; i++) {
    //     RESET_AT_POINT(arena);
    //     printf("%p\n", arena.cursor);
    //     alloc(&arena, sizeof(int));
    // }
    
    Array_Fix<int, 2> arr;
    arr[0] = 2;
    printf("%d\n", arr[0]);
    
}