#include "plugin_interface.h"
#include <iostream>

class MathPlugin : public PluginInterface {
public:
    std::string getName() const override {
        return "MathPlugin";
    }
    
    int processData(int input) const override {
        // This plugin doubles the input value
        std::cout << "MathPlugin: Processing data " << input << std::endl;
        return input * 2;
    }
};

// Export the factory functions with C linkage
EXPORT_PLUGIN_API PLUGIN_API PluginInterface* createPlugin() {
    std::cout << "Creating MathPlugin instance" << std::endl;
    return new MathPlugin();
}

EXPORT_PLUGIN_API PLUGIN_API void destroyPlugin(PluginInterface* plugin) {
    std::cout << "Destroying MathPlugin instance" << std::endl;
    delete plugin;
}
