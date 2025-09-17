#include <iostream>
#include <vector>
#include <memory>
#include "hello.h"

int main() {
    // Smart pointer + move semantics demo
    auto greeter = std::make_unique<hello::Greeter>("Subhojit");

    // Small modern STL usage
    std::vector<const char*> prefixes = {"Hello", "Namaste", "Hola"};
    for (const auto* p : prefixes) {
        std::cout << greeter->greet(p) << "\n";
    }

    return 0;
}
