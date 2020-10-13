#pragma once
#include "../common/utils.h"
class PageDescriptor {
    Utils::UintContainer<uint8_t> _metadata;
    uint8_t _free;
    Utils::BytePair _blocks[16];
  public:
    explicit PageDescriptor() : _metadata() {}
    bool isDivided() {
        return _metadata.get<0>();
    }
    void declassify() {
        _metadata.set<0>(false);
    }
    uint8_t getFreeCount() {
        return _free;
    }
    uint8_t getClass() {
        return _metadata.get<1,3>();
    }
    void setClass(uint8_t c) {
        _metadata.set<0>(true);
        _metadata.set<1,3>(c);
        _free = 2u << c;
        _metadata.set<4,7>(0);
        for(uint8_t i = 0; i < (2u << c); ++i) {
            _blocks[i].set<0>(i);
            _blocks[i].set<1>(i + 1);
        }
    }
    uint8_t lock() {
        if(getFreeCount()) {
            uint8_t res = _metadata.get<4,7>();
            uint8_t prev = _blocks[res].get<0>();
            uint8_t next = _blocks[res].get<1>();
            _blocks[prev].set<1>(next);
            _blocks[next].set<0>(prev);
            _free--;
            return res;
        } else throw std::bad_alloc();
    }
    void free(uint8_t block) {
        uint8_t currFree = getFreeCount();
        if(currFree) {
            uint8_t currHead = _metadata.get<4,7>();
            _blocks[block].set<1>(currHead);
            _blocks[currHead].set<0>(block);
        }
        _metadata.set<4,7>(block);
        _free++;
    }
};