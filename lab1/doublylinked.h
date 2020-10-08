#pragma once
#include <cstddef>
#include "../common/utils.h"
#include "interfaces.h"

namespace DoublyLinked {
template<size_t maxSize>
struct DataSizeLimit {
    template<typename Derived>
    class SerialBackwardIterator {
        using uintT = Utils::uintFitting_t<maxSize>;
        uintT _prevSize;
      public:
        Derived *prev() {
            return reinterpret_cast<Derived *>(reinterpret_cast<uint8_t *>(static_cast<Derived *>(this) - 1) - _prevSize);
        }
        void setPrev(Derived *prev) requires ISizeable<Derived> {
            _prevSize = prev->size();
        }
    };
};
template<typename Derived>
class NextBuilder {
  public:
    void createNext(size_t size, bool isFree) requires IForwardIterable<Derived> && IBackwardLinkable<Derived> {
        (new(reinterpret_cast<Derived*>(this)->next()) Derived(size, isFree))
        ->setPrev(reinterpret_cast<Derived*>(this));
    }
};
template<typename Derived>
class DefragmentationStrategy {
  public:
    void defragment(Derived* start, Derived* end)
    requires ILockable<Derived> && ISizeable<Derived> && IForwardIterable<Derived>
    && IBackwardIterable<Derived> && IBackwardLinkable<Derived> {
        auto result = static_cast<Derived*>(this);
        for(auto block = result->prev();
                block >= start && block->isFree();
                block = block->prev()) {
            block->setSize(result->size() + block->size() + sizeof(Derived));
            result->next()->setPrev(result);
            result = block;
        }
        for(auto block = result->next();
                block <= end && block->isFree();
                block = block->next()) {
            result->setSize(result->size() + block->size() + sizeof(Derived));
            result->next()->setPrev(result);
        }
    }
};
template<size_t pageSize>
using Header = Utils::Composed<Base::HeaderData<pageSize>,
      Base::DataPlacementStrategy,
      Base::SerialForwardIterator,
      DataSizeLimit<pageSize>::template SerialBackwardIterator,
      NextBuilder,
      DefragmentationStrategy>;

}
