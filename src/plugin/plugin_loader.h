#pragma once
#include "plugin_interface.h"
#include <string>
#include <memory>
#include <stdexcept>

// Platform-specific includes for dynamic loading
#ifdef _WIN32
    #include <windows.h>
    #define LIBRARY_HANDLE HMODULE
    #define LOAD_LIBRARY(name) LoadLibraryA(name)
    #define GET_PROC_ADDRESS(handle, name) GetProcAddress(handle, name)
    #define CLOSE_LIBRARY(handle) FreeLibrary(handle)
    #define LIBRARY_PREFIX ""
    #define LIBRARY_EXTENSION ".dll"
#else
    #include <dlfcn.h>
    #define LIBRARY_HANDLE void*
    #define LOAD_LIBRARY(name) dlopen(name, RTLD_LAZY)
    #define GET_PROC_ADDRESS(handle, name) dlsym(handle, name)
    #define CLOSE_LIBRARY(handle) dlclose(handle)
    #ifdef __APPLE__
        #define LIBRARY_PREFIX "lib"
        #define LIBRARY_EXTENSION ".dylib"
    #else
        #define LIBRARY_PREFIX "lib"
        #define LIBRARY_EXTENSION ".so"
    #endif
#endif

class PluginLoadError : public std::runtime_error {
public:
    explicit PluginLoadError(const std::string& message)
        : std::runtime_error(message) {}
};

class PluginLoader {
public:
    PluginLoader(const std::string& pluginPath);
    ~PluginLoader();

    // Non-copyable
    PluginLoader(const PluginLoader&) = delete;
    PluginLoader& operator=(const PluginLoader&) = delete;

    // Get a plugin instance
    std::unique_ptr<PluginInterface, void(*)(PluginInterface*)> createInstance();

    // Get the last error message from the dynamic loader
    static std::string getLastErrorMessage();

private:
    LIBRARY_HANDLE libraryHandle;
    CreatePluginFunc createFunc;
    DestroyPluginFunc destroyFunc;

    // Custom deleter for the plugin
    static void pluginDeleter(PluginInterface* plugin);
};
