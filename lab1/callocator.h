#pragma once
#include <iostream>
#include <cstring>
#include "interfaces.h"

template<IHeader Header>
class CAllocator {
  protected:
    void* _memory;
    Header* _first;
    Header* _last;
  public:
    CAllocator(void* memory, size_t size) :
        _memory(memory),
        _first(new(_memory) Header(size - 2 * sizeof(Header), true)),
        _last(new(reinterpret_cast<uint8_t*>(memory) + size - sizeof(Header)) Header(0, false)) {}
    void* mem_alloc(size_t size) {
        if((size + sizeof(Header)) % sizeof(size_t))
            size = size + sizeof(size_t) - (size + sizeof(Header)) % sizeof(size_t);
        for(auto header = _first; header != _last; header=header->next()) {
            auto currentSize = header->size();
            if(header->isFree()) {
                if(currentSize >= size && currentSize <= size + sizeof(Header)) {
                    header->lock();
                    return header->data();
                } else if(currentSize > size + sizeof(Header)) {
                    header->lock();
                    header->setSize(size);
                    header->createNext(currentSize - size - sizeof(Header), true);
                    return header->data();
                }
            }
        }
        return nullptr;
    }
    void* mem_realloc(void* addr, size_t size) {
        if(addr < _first || addr >= _last) return nullptr;
        if((size + sizeof(Header)) % sizeof(size_t))
            size = size + sizeof(size_t) - (size + sizeof(Header)) % sizeof(size_t);
        auto header = Header::fromData(addr);
        if(header->next()->isFree())
            header->setSize(header->size() + header->next()->size() + sizeof(Header));
        auto currentSize = header->size();
        if(currentSize > size + sizeof(Header)) {
            header->setSize(size);
            header->createNext(currentSize - size - sizeof(Header), false);
            return header->data();
        } else if(currentSize >= size) {
            return header->data();
        } else {
            void* block = mem_alloc(size);
            if(!block) return nullptr;
            std::memcpy(block, header->data(), header->size());
            mem_free(header->data());
            return block;
        }
    }
    void mem_free(void* addr) {
        if(addr < _first || addr >= _last) return;
        auto header = Header::fromData(addr);
        header->free();
        header->defragment(_first, _last);
    }
    void mem_dump() {
        for(auto header = _first; header != _last; header = header->next())
            std::cout
                    << "Pointer: " << header->data() << " "
                    << "Size: " << +header->size() << " "
                    << "Free: " << (header->isFree() ? "yes" : "no") << std::endl;
        std::cout << std::endl;
    }
};