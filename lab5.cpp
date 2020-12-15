#include <array>
#include <chrono>
#include <iostream>

auto original() {
    std::array<std::array<int, 100>, 100> b;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 1000; ++i)
        for (int j = 0; j < 100; ++j)
            for (int k = 0; k < 100; ++k)
                ++b[k][j];
    return duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
}

auto optimized() {
    std::array<int, 10000> b;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 1000; ++i)
        for (int j = 0; j < 100; ++j)
            for (int k = 0; k < 100; ++k)
                ++b[k + 100*j];
    return duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
}
 
int main() {
    std::cout << "Original: " << original() << " ms" << std::endl;
    std::cout << "Optimized: " << optimized() << " ms" << std::endl;
}
