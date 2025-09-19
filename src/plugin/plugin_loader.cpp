#include "plugin_loader.h"
#include <iostream>

// Global function that will be used for deleting plugins
static DestroyPluginFunc globalDestroyFunc = nullptr;

PluginLoader::PluginLoader(const std::string& pluginPath) 
    : libraryHandle(nullptr), createFunc(nullptr), destroyFunc(nullptr) {
    
    std::cout << "Loading plugin from: " << pluginPath << std::endl;
    
    // Load the dynamic library
    libraryHandle = LOAD_LIBRARY(pluginPath.c_str());
    if (!libraryHandle) {
        throw PluginLoadError("Failed to load plugin library: " + getLastErrorMessage());
    }
    
    // Get the creation function
    createFunc = reinterpret_cast<CreatePluginFunc>(
        GET_PROC_ADDRESS(libraryHandle, CREATE_PLUGIN_FUNC_NAME)
    );
    
    if (!createFunc) {
        CLOSE_LIBRARY(libraryHandle);
        throw PluginLoadError("Failed to find createPlugin function: " + getLastErrorMessage());
    }
    
    // Get the destruction function
    destroyFunc = reinterpret_cast<DestroyPluginFunc>(
        GET_PROC_ADDRESS(libraryHandle, DESTROY_PLUGIN_FUNC_NAME)
    );
    
    if (!destroyFunc) {
        CLOSE_LIBRARY(libraryHandle);
        throw PluginLoadError("Failed to find destroyPlugin function: " + getLastErrorMessage());
    }
}

PluginLoader::~PluginLoader() {
    if (libraryHandle) {
        std::cout << "Unloading plugin library" << std::endl;
        CLOSE_LIBRARY(libraryHandle);
    }
}

// Static function to act as deleter for the unique_ptr
static void pluginDeleterFunction(PluginInterface* plugin) {
    if (globalDestroyFunc != nullptr) {
        globalDestroyFunc(plugin);
    }
}

std::unique_ptr<PluginInterface, void(*)(PluginInterface*)> PluginLoader::createInstance() {
    if (!createFunc || !destroyFunc) {
        throw PluginLoadError("Plugin not properly initialized");
    }
    
    // Set the global destroy function to our instance's destroy function
    globalDestroyFunc = destroyFunc;
    
    // Create a plugin instance and wrap it in a unique_ptr with our static deleter function
    return std::unique_ptr<PluginInterface, void(*)(PluginInterface*)>(
        createFunc(),
        &pluginDeleterFunction
    );
}

std::string PluginLoader::getLastErrorMessage() {
#ifdef _WIN32
    DWORD errorCode = GetLastError();
    if (errorCode == 0) {
        return "No error";
    }
    
    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    
    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);
    return message;
#else
    const char* error = dlerror();
    return error ? error : "No error";
#endif
}

void PluginLoader::pluginDeleter(PluginInterface* plugin) {
    // This is just a placeholder - we actually use the captured destroyFunc in createInstance
    delete plugin;
}
