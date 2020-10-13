#pragma once
#include <iostream>
#include "../common/utils.h"
#include "pages.h"
#include "containers.h"
#include "pagedescriptor.h"
class FastAllocator {
    PageDescriptor _pages[Pages::PAGE_COUNT];
    size_t _pageSize = Pages::translator.pageSize();
    Containers::unordered_multimap<uint8_t, Pages::IndexType> _classified;
    Pages::IndexType classifyPage(uint8_t c) {
        Pages::IndexType i = Pages::manager.lockSingle();
        if(i) {
            _pages[i - 1].setClass(c);
            return i - 1;
        } else throw std::bad_alloc();
    }
  public:
    void* mem_alloc(std::size_t size) {
        if(size <= _pageSize / 2) {
            size--;
            uint8_t c = 0;
            while(size) {
                size >>= 1;
                c++;
            }
            c = 11 - std::max(c, uint8_t(8));
            if(!_classified.contains(c)) _classified.insert(std::make_pair(c, classifyPage(c)));
            auto pagePair = _classified.find(c);
            Pages::IndexType page = pagePair->second;
            uint8_t block = _pages[page].lock();
            if(!_pages[page].getFreeCount()) _classified.erase(pagePair);
            return Pages::translator.indexToPtr(page, block, c);
        } else {
            std::size_t pages = Pages::translator.sizeToPages(size);
            Pages::IndexType i = pages == 1 ? Pages::manager.lockSingle() : Pages::manager.lockMultiple(pages);
            if(i) return Pages::translator.indexToPtr(i - 1);
            else throw std::bad_alloc();
        };
    }
    void mem_free(void* ptr) {
        Pages::IndexType index = Pages::translator.ptrToIndex(ptr);
        if(_pages[index].isDivided()) {
            uint8_t c = _pages[index].getClass();
            uint8_t block = Pages::translator.ptrToPageBlock(ptr, c);
            _pages[index].free(block);
            uint8_t freeCount = _pages[index].getFreeCount();
            if(freeCount == 1) {
                _classified.insert(std::make_pair(c, index));
            } else if(freeCount == (2u << c)) {
                std::erase_if(_classified, [c, index](const auto& pagePair) {
                    auto const& [Class, Index] = pagePair;
                    return Class == c && Index == index;
                });
                _pages[index].declassify();
                Pages::manager.free(index + 1);
            }
        } else {
            Pages::manager.free(index + 1);
        }
    }
    void mem_dump() {
        bool inMultiple = false;
        Pages::IndexType currStart = 0;
        Pages::IndexType byContainers = 0;
        for(Pages::IndexType i = 0; i < Pages::PAGE_COUNT; ++i) {
            const auto [isUsed, isConsecutive] = Pages::manager.pageFlags(i);
            if(isUsed) {
                if(isConsecutive) {
                    if(!inMultiple) {
                        inMultiple = true;
                        currStart = i;
                    }
                } else if(inMultiple) {
                    inMultiple = false;
                    std::cout << "Page " << i << " at " << Pages::translator.indexToPtr(i)
                              << " Multipage allocation(" << i - currStart + 1 << " pages) " << std::endl;
                } else if (_pages[i].isDivided()) {
                    std::cout << "Page " << i << " at " << Pages::translator.indexToPtr(i)
                              << " Divided page(" << (size_t)_pages[i].getFreeCount() << " free out of " << (2u << _pages[i].getClass())
                              << " blocks " << Pages::translator.pageSize() / (2u << _pages[i].getClass()) << " bytes each)"
                              << std::endl;
                } else {
                    bool isUsedByContainers = Pages::usedByContainers.get(i);
                    if(isUsedByContainers) ++byContainers;
                    std::cout << "Page " << i << " at " << Pages::translator.indexToPtr(i)
                              << " Singlepage allocation" << (isUsedByContainers ? "(used by containers)" : "") << std::endl;
                }
            }
        }
        std::cout << "Hash table has " << _classified.bucket_count()
                  << " buckets using " << byContainers << " pages" << std::endl;
    }
};
