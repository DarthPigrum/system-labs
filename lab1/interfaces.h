#pragma once
#include <concepts>

template<typename T>
concept ISizeable = requires(T t, std::size_t s) {
    {
        t.size()
    }
    -> std::convertible_to<std::size_t>;
    {
        t.setSize(s)
    };
};
template<typename T>
concept ILockable = requires(T t) {
    {
        t.lock()
    };
    {
        t.free()
    };
    {
        t.isFree()
    }
    -> std::convertible_to<bool>;
};
template<typename T>
concept IForwardIterable = requires(T t) {
    {
        t.next()
    }
    -> std::convertible_to<T*>;
};
template<typename T>
concept IForwardBuildable = requires(T t, std::size_t s, bool f) {
    {
        t.createNext(s, f)
    };
};
template<typename T>
concept IBackwardIterable = requires(T t) {
    {
        t.prev()
    }
    -> std::convertible_to<T*>;
};
template<typename T>
concept IBackwardLinkable = requires(T t, T* ptr) {
    {
        t.setPrev(ptr)
    };
};
template<typename T>
concept IData = requires (T t, void* data) {
    {
        t.data()
    }
    -> std::convertible_to<void*>;
    {
        T::fromData(data)
    }
    -> std::convertible_to<T*>;
};
template<typename T>
concept Defragmentable = requires(T t, T* start, T* end) {
    {
        t.defragment(start, end)
    };
};
template<typename T>
concept IHeader = ISizeable<T> && ILockable<T> && IForwardIterable<T> && IForwardBuildable<T> && IData<T> && Defragmentable<T>;
