#include "common/utils.h"
#include "lab1/base.h"
#include "lab1/doublylinked.h"
#include "lab1/callocator.h"

void* dealignMemory(void* memory, uint8_t prefix) {
    for(auto ptr = reinterpret_cast<size_t>(memory);; ++ptr)
        if(ptr % sizeof(size_t) == sizeof(size_t) - prefix) return reinterpret_cast<void*>(ptr);
}

template<typename Allocator>
void test(void *memory, size_t size) {
    Allocator allocator(memory, size);
    allocator.mem_dump();
    void *ptr1 = allocator.mem_alloc(10);
    allocator.mem_dump();
    void *ptr2 = allocator.mem_alloc(50);
    allocator.mem_dump();
    allocator.mem_realloc(ptr1, 20);
    allocator.mem_dump();
    allocator.mem_free(ptr1);
    allocator.mem_dump();
    allocator.mem_free(ptr2);
    allocator.mem_dump();
}
int main() {
    void* memory = malloc(1ull << 31);
    std::cout << "Однонаправлений хедер для керування шматками пам'яті до "
              << (1ull << 7) - 1 << " байт "
              << "(розмір хедера - " << sizeof(Base::Header<120>) << " байт)" << std::endl;
    test<CAllocator<Base::Header<120>>>(
        dealignMemory(memory, sizeof(Base::Header<120>)), 120);
    std::cout << "Двонапрямлений хедер для керування шматками пам'яті до "
              << (1ull << 7) - 1 << " байт "
              << "(розмір хедера - " << sizeof(DoublyLinked::Header<120>) << " байт)" << std::endl;
    test<CAllocator<DoublyLinked::Header<120>>>(
        dealignMemory(memory, sizeof(DoublyLinked::Header<120>)), 120);
    std::cout << "Однонаправлений хедер для керування шматками пам'яті до "
              << (1ull << 15) - 1 << " байт "
              << "(розмір хедера - " << sizeof(Base::Header<32000>) << " байт)" << std::endl;
    test<CAllocator<Base::Header<32000>>>(
        dealignMemory(memory, sizeof(Base::Header<32000>)), 32000);
    std::cout << "Двонапрямлений хедер для керування шматками пам'яті до "
              << (1ull << 15) - 1 << " байт "
              << "(розмір хедера - " << sizeof(DoublyLinked::Header<32000>) << " байт)" << std::endl;
    test<CAllocator<DoublyLinked::Header<32000>>>(
        dealignMemory(memory, sizeof(DoublyLinked::Header<32000>)), 32000);
    std::cout << "Однонаправлений хедер для керування шматками пам'яті до "
              << (1ull << 31) - 1 << " байт "
              << "(розмір хедера - " << sizeof(Base::Header<2000000000>) << " байт)" << std::endl;
    test<CAllocator<Base::Header<2000000000>>>(
        dealignMemory(memory, sizeof(Base::Header<2000000000>)), 2000000000);
    std::cout << "Двонапрямлений хедер для керування шматками пам'яті до "
              << (1ull << 31) - 1 << " байт "
              << "(розмір хедера - " << sizeof(DoublyLinked::Header<2000000000>) << " байт)" << std::endl;
    test<CAllocator<DoublyLinked::Header<2000000000>>>(
        dealignMemory(memory, sizeof(DoublyLinked::Header<2000000000>)), 2000000000);
    return 0;
}
