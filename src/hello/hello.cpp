#include "hello.h"
#include <utility>

namespace hello {

    Greeter::Greeter(std::string name) : name_(std::move(name)) {}

    std::string Greeter::greet(std::string_view prefix) const {
        // Uses std::string + std::string_view (modern C++)
        return std::string(prefix) + ", " + name_ + "!";
    }

} // namespace hello
