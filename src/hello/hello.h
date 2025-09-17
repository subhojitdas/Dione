#pragma once
#include <string>
#include <string_view>

namespace hello {

    // A tiny RAII-style greeter that owns a std::string
    class Greeter {
    public:
        explicit Greeter(std::string name);
        std::string greet(std::string_view prefix = "Hello") const;

    private:
        std::string name_;
    };

} // namespace hello
