#pragma once
#include <string>

// Define a common interface for plugins
class PluginInterface {
public:
    virtual ~PluginInterface() = default;
    virtual std::string getName() const = 0;
    virtual int processData(int input) const = 0;
};

// Function signature for creating plugin instances
// This will be exported from the plugin library
typedef PluginInterface* (*CreatePluginFunc)();
typedef void (*DestroyPluginFunc)(PluginInterface*);

// These are the names we'll use to find the exported functions
#define CREATE_PLUGIN_FUNC_NAME "createPlugin"
#define DESTROY_PLUGIN_FUNC_NAME "destroyPlugin"

// Macros to simplify plugin implementation
#define EXPORT_PLUGIN_API extern "C"

#ifdef _WIN32
    #define PLUGIN_API __declspec(dllexport)
#else
    #define PLUGIN_API __attribute__((visibility("default")))
#endif
