#include "lab2/allocator.h"
int main() {
    FastAllocator test;
    void* one = test.mem_alloc(10000);
    test.mem_alloc(4000);
    void* three = test.mem_alloc(3000);
    void* two = test.mem_alloc(2000);
    test.mem_alloc(1750);
    test.mem_alloc(1500);
    test.mem_alloc(1250);
    test.mem_alloc(1000);
    void* five = test.mem_alloc(500);
    void* four = test.mem_alloc(400);
    test.mem_alloc(300);
    test.mem_alloc(200);
    test.mem_alloc(100);
    test.mem_free(one);
    test.mem_free(two);
    test.mem_free(three);
    test.mem_free(four);
    test.mem_free(five);
    test.mem_alloc(100);
    test.mem_alloc(200);
    test.mem_alloc(500);
    test.mem_alloc(1000);
    test.mem_alloc(2000);
    test.mem_alloc(5000);
    test.mem_dump();
}