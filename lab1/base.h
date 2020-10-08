#pragma once
#include <cstddef>
#include <iostream>
#include "../common/utils.h"
#include "interfaces.h"

namespace Base {
template<size_t maxSize>
class HeaderData {
    using uintT = Utils::uintFitting_t<maxSize << 1>;
    Utils::UintContainer<uintT> bits;
    static constexpr uint8_t lastBit = sizeof(uintT) * 8 - 1;
  public:
    explicit HeaderData(uintT size, bool isFree) {
        bits.template set<lastBit, lastBit>(isFree);
        bits.template set<0, lastBit - 1>(size);
    }
    uintT size() const {
        return bits.template get<0, lastBit - 1>();
    }
    void setSize(uintT size) {
        bits.template set<0, lastBit - 1>(size);
    }
    bool isFree() const {
        return bits.template get<lastBit, lastBit>();
    }
    void lock() {
        bits.template set<lastBit, lastBit>(false);
    }
    void free() {
        bits.template set<lastBit, lastBit>(true);
    }
};
template<typename Derived>
class SerialForwardIterator {
  public:
    Derived* next() requires ISizeable<Derived> {
        auto header = reinterpret_cast<Derived*>(this);
        return reinterpret_cast<Derived*>(reinterpret_cast<uint8_t*>(header + 1) + header->size());
    }
};
template<typename Derived>
class DataPlacementStrategy {
  public:
    static Derived* fromData(void* addr) {
        return reinterpret_cast<Derived*>(reinterpret_cast<Derived*>(addr) - 1);
    }
    void* data() {
        return reinterpret_cast<Derived*>(this) + 1;
    }
};
template<typename Derived>
class NextBuilder {
  public:
    void createNext(size_t size, bool isFree) requires IForwardIterable<Derived> {
        new(reinterpret_cast<Derived*>(this)->next()) Derived(size, isFree);
    }
};
template<typename Derived>
class DefragmentationStrategy {
  public:
    void defragment(Derived* start, Derived* end)
    requires ILockable<Derived> && ISizeable<Derived> && IForwardIterable<Derived> {
        Derived* extendable = nullptr;
        for(auto block = start; block != end; block = block->next()) {
            if(block->isFree()) {
                if(extendable)
                    extendable->setSize(extendable->size() + block->size() + sizeof(Derived));
                else
                    extendable = block;
            } else
                extendable = nullptr;
        }
    }
};
template<size_t pageSize>
using Header = Utils::Composed<HeaderData<pageSize>,
      DataPlacementStrategy,
      SerialForwardIterator,
      NextBuilder,
      DefragmentationStrategy>;

}
