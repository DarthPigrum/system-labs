#include <array>
#include <chrono>
#include <iostream>

auto original() {
    std::array<std::array<int, 100>, 100> a;
    auto start = std::chrono::high_resolution_clock::now();
    for(int n = 0; n < 1000; ++n) {
        for (int i = 0; i < 100; ++i)
            for (int j = 0; j < 100; ++j)
                a[j][i] = a[j-1][i-1] + 1;
        for (int i = 0; i < 100; ++i)
            for (int j = 0; j < 100; ++j)
                a[j][i]++;
    }
    return duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
}

auto optimized() {
    std::array<int, 10000> a;
    auto start = std::chrono::high_resolution_clock::now();
    for(int n = 0; n < 1000; ++n) {
        for (int i = 0; i < 100; ++i)
            for (int j = 0; j < 100; ++j)
                a[j + 100*i] = a[(j-1) + 100*(i-1)] + 1;
        for (int i = 0; i < 100; ++i)
            for (int j = 0; j < 100; ++j)
                a[j + 100*i]++;
    }
    return duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
}
 
int main() {
    std::cout << "Original: " << original() << " ms" << std::endl;
    std::cout << "Optimized: " << optimized() << " ms" << std::endl;
}
