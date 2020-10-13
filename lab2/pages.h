#pragma once
#include "../common/utils.h"
#include "pagemanager.h"
#include "addresstranslator.h"
namespace Pages {
static constexpr std::size_t PAGE_COUNT = 1024;
static PageManager<PAGE_COUNT> manager;
static AddressTranslator<PAGE_COUNT> translator;
static Utils::Flags<PAGE_COUNT> usedByContainers;
using IndexType = PageManager<PAGE_COUNT>::IndexType;
template <class T>
struct allocator {
    using value_type = T;
    allocator() noexcept {}
    template <class U> allocator (const allocator<U>&) noexcept {}
    T* allocate (std::size_t n) {
        std::size_t pages = translator.sizeToPages(sizeof(T) * n);
        IndexType i = pages == 1 ? manager.lockSingle() : manager.lockMultiple(pages);
        if(i) {
            usedByContainers.set(i - 1, true);
            return reinterpret_cast<T*>(translator.indexToPtr(i - 1));
        } else throw std::bad_alloc();
    }
    void deallocate (T* p, std::size_t n) {
        IndexType i = translator.ptrToIndex(p);
        usedByContainers.set(i, false);
        manager.free(i + 1);
    }
};
template <class T, class U>
constexpr bool operator== (const allocator<T>&, const allocator<U>&) noexcept;
template <class T, class U>
constexpr bool operator!= (const allocator<T>&, const allocator<U>&) noexcept;
}