# C++ Modules, Preprocessing, Linking, and Runtime Loading Guide

This project demonstrates key concepts in C++ development, focusing on module separation, preprocessing, linking, and runtime loading. Below is a comprehensive explanation of these concepts and how they're implemented in this project.

## 1. Preprocessing in C++

The preprocessing stage occurs before actual compilation and is handled by the C++ preprocessor. In our project, we use several preprocessing directives:

### Header Guards

```cpp
#pragma once
```

This prevents multiple inclusions of the same header file, avoiding redefinition errors.

### Macros

```cpp
#define CREATE_PLUGIN_FUNC_NAME "createPlugin"
#define DESTROY_PLUGIN_FUNC_NAME "destroyPlugin"

#define EXPORT_PLUGIN_API extern "C"
```

Macros substitute text in the code before compilation, making code more readable and maintainable.

### Conditional Compilation

```cpp
#ifdef _WIN32
    #define LIBRARY_HANDLE HMODULE
    // ...
#else
    #define LIBRARY_HANDLE void*
    // ...
#endif
```

This allows platform-specific code to be conditionally included based on defined symbols.

## 2. C++ Compilation and Linking Process

The overall process goes through these stages:

1. **Preprocessing**: Handles directives like `#include`, `#define`, `#ifdef`
2. **Compilation**: Converts preprocessed C++ code into object files
3. **Linking**: Combines object files and libraries into executables or libraries

### Static Linking

In our CMakeLists.txt, we create a static library:

```cmake
add_library(hello STATIC
        src/hello/hello.cpp
)
```

Static linking means the library code is copied into the final executable during the linking phase. The executable doesn't need the library file at runtime.

Benefits:
- No runtime dependencies on the library
- Potentially better optimization
- Simpler deployment (single executable)

Drawbacks:
- Larger executable size
- Can't update the library without recompiling the executable

### Dynamic Linking

We also create a shared library:

```cmake
add_library(math_plugin SHARED
        src/plugin/math_plugin.cpp
)
```

Dynamic linking means the library is loaded at program startup or runtime. The executable contains references to the library functions but not the code itself.

Benefits:
- Smaller executable size
- Library can be updated without recompiling the executable
- Library can be shared between multiple programs

Drawbacks:
- Runtime dependency on the library
- Potential for "DLL Hell" (version compatibility issues)

## 3. Runtime Loading (Dynamic Loading)

Our project demonstrates runtime loading through a plugin system:

```cpp
// Load the dynamic library
libraryHandle = LOAD_LIBRARY(pluginPath.c_str());

// Get the creation function
createFunc = reinterpret_cast<CreatePluginFunc>(
    GET_PROC_ADDRESS(libraryHandle, CREATE_PLUGIN_FUNC_NAME)
);
```

Runtime loading allows you to:
- Load code modules on-demand
- Implement plugin systems
- Support hot-swapping of code
- Create modular applications that can be extended without recompilation

Key functions used:
- Windows: `LoadLibrary`, `GetProcAddress`, `FreeLibrary`
- UNIX/Linux/macOS: `dlopen`, `dlsym`, `dlclose`

## 4. Symbol Visibility and Exporting Functions

To make functions available for dynamic loading:

```cpp
EXPORT_PLUGIN_API PLUGIN_API PluginInterface* createPlugin() {
    // ...
}
```

This uses platform-specific attributes to export the function symbol:
- Windows: `__declspec(dllexport)`
- UNIX/Linux/macOS: `__attribute__((visibility("default")))`

## 5. C++ Interface Design for Plugins

Our plugin system uses abstract interfaces:

```cpp
class PluginInterface {
public:
    virtual ~PluginInterface() = default;
    virtual std::string getName() const = 0;
    virtual int processData(int input) const = 0;
};
```

This allows for:
- Type safety across module boundaries
- Polymorphism (different plugin implementations)
- Clear API contracts between modules

## 6. Smart Pointers for Resource Management

The plugin system uses smart pointers to manage dynamic memory:

```cpp
std::unique_ptr<PluginInterface, void(*)(PluginInterface*)> createInstance();
```

This ensures proper cleanup of resources even when plugins are dynamically loaded/unloaded.

## 7. Running the Demo

1. Build the project: `./build_plugin_demo.sh`
2. Run the plugin demo: `./build/plugin_loader`

The demo shows:
- Loading a shared library at runtime
- Creating a plugin instance through exported functions
- Using the plugin's functionality through an abstract interface
- Proper cleanup when the program exits

## 8. Real-world Applications

This pattern is used in many software systems:
- Web browsers with plugin architectures
- Graphics applications with filter plugins
- Game engines with modding support
- Audio workstations with effect plugins
- Text editors with extension systems

## 9. Advanced Concepts

For further exploration:
- Symbol versioning
- Plugin dependency management
- Hot-reloading of plugins
- Cross-platform plugin systems
- Security considerations for dynamically loaded code
