#pragma once
#include "../common/utils.h"
template<std::size_t pageCount>
class PageManager {
    using _IndexType = Utils::uintFitting_t<pageCount>;
    _IndexType indexBuffer = 0;
    _IndexType freePages = pageCount;
    Utils::Flags<pageCount> isUsed;
    Utils::Flags<pageCount> isConsecutive;
    bool findSingle() {
        for(std::size_t i = 0; i < pageCount; ++i) {
            if(!isUsed.get((indexBuffer + i) % pageCount)) {
                indexBuffer = (indexBuffer + i) % pageCount;
                return true;
            }
        }
        return false;
    }
    bool findConsecutive(_IndexType count) {
        for(_IndexType i = 0; i < pageCount; ++i) {
            if(!isUsed.get(i)) {
                std::size_t free = 1;
                while(free < count && !isUsed.get(i)) {
                    free++;
                    i++;
                }
                if(free == count) {
                    indexBuffer = i + 1 - count;
                    return true;
                }
            }
        }
        return false;
    }
  public:
    using IndexType = _IndexType;
    PageManager() : isUsed(), isConsecutive() {}
    IndexType lockSingle() {
        if(!freePages) return 0;
        if(findSingle()) {
            isUsed.set(indexBuffer, true);
            freePages--;
            return indexBuffer + 1;
        }
        return 0;
    }
    IndexType lockMultiple(IndexType pages) {
        if(pages > freePages) return 0;
        if(findConsecutive(pages)) {
            for(std::size_t i = 0; i < pages - 1; ++i) {
                isUsed.set(indexBuffer + i, true);
                isConsecutive.set(indexBuffer + i, true);
            }
            isUsed.set(indexBuffer + pages - 1, true);
            IndexType res = indexBuffer + 1;
            freePages -= pages;
            findSingle();
            return res;
        }
        return 0;
    };
    void free(IndexType index) {
        if(index <= 0 || index > pageCount) return;
        IndexType i = index - 1;
        isUsed.set(i, false);
        freePages++;
        while(isConsecutive.get(i)) {
            isUsed.set(i + 1, false);
            isConsecutive.set(i, false);
            freePages++;
            i++;
        }
    }
    std::pair<bool, bool> pageFlags(IndexType index) {
        return std::make_pair(isUsed.get(index), isConsecutive.get(index));
    }
};
