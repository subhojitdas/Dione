#include "plugin_loader.h"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

// Get the path to the plugin library based on the current platform
std::string getPluginPath(const std::string& pluginName) {
    std::string basePath = fs::current_path().string() + "/build";
    std::string filename = std::string(LIBRARY_PREFIX) + pluginName + LIBRARY_EXTENSION;
    return basePath + "/" + filename;
}

int main(int argc, char* argv[]) {
    try {
        std::cout << "Dynamic Plugin Loader Demo\n" 
                  << "===========================\n" << std::endl;
        
        // Determine the plugin path
        std::string pluginPath = getPluginPath("math_plugin");
        std::cout << "Looking for plugin at: " << pluginPath << std::endl;
        
        // Load the plugin library
        PluginLoader loader(pluginPath);
        
        // Create a plugin instance
        auto plugin = loader.createInstance();
        std::cout << "Successfully loaded plugin: " << plugin->getName() << std::endl;
        
        // Use the plugin functionality
        std::vector<int> testValues = {5, 10, 15};
        for (int value : testValues) {
            int result = plugin->processData(value);
            std::cout << "Input: " << value << ", Output: " << result << std::endl;
        }
        
        std::cout << "\nPlugin will be destroyed when we exit scope" << std::endl;
        // plugin automatically gets destroyed here when it goes out of scope
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "Program completed successfully" << std::endl;
    return 0;
}
