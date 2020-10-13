#pragma once
#include <unistd.h>
template<std::size_t pageCount>
class AddressTranslator {
    void* _memory;
    static constexpr size_t _pageSize = 4096;
  public:
    using IndexType = Utils::uintFitting_t<pageCount>;
    size_t pageSize() {
        return  _pageSize;
    }
    explicit AddressTranslator() : _memory(std::malloc(_pageSize * pageCount)) {}
    void* indexToPtr(IndexType index) {
        return reinterpret_cast<uint8_t*>(_memory) + _pageSize * index;
    }
    void* indexToPtr(IndexType index, uint8_t block, uint8_t c) {
        return reinterpret_cast<uint8_t*>(_memory) + _pageSize * index + (block * (_pageSize >> (c + 1)));
    }
    IndexType ptrToIndex(void* ptr) {
        return (reinterpret_cast<size_t>(ptr) - reinterpret_cast<size_t>(_memory)) / _pageSize;
    }
    uint8_t ptrToPageBlock(void* ptr, uint8_t c) {
        return ((reinterpret_cast<size_t>(ptr) - reinterpret_cast<size_t>(_memory)) % _pageSize) / (_pageSize / (2u << c));
    }
    size_t sizeToPages(std::size_t size) {
        return size / _pageSize + (size % _pageSize ? 1 : 0);
    }
};