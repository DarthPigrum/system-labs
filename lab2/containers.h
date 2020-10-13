#pragma once
#include "pages.h"
#include <unordered_map>
namespace Containers {
template<
    class Key,
    class T,
    class Hash = std::hash<Key>,
    class KeyEqual = std::equal_to<Key>,
    class Allocator = Pages::allocator< std::pair<const Key, T> >
    > using unordered_multimap = std::unordered_map<Key, T, Hash, KeyEqual, Allocator>;
}