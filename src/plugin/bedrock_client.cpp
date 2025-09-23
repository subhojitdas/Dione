#include "plugin_loader.h"
#include "aws_bedrock_plugin.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

// Get the path to the plugin library based on the current platform
std::string getPluginPath(const std::string& pluginName) {
    std::string basePath = fs::current_path().string() + "/build";
    std::string filename = std::string(LIBRARY_PREFIX) + pluginName + LIBRARY_EXTENSION;
    return basePath + "/" + filename;
}

int main() {
    try {
        std::cout << "AWS Bedrock Plugin Demo\n" 
                  << "=====================\n" << std::endl;
        
        // Determine the plugin path
        std::string pluginPath = getPluginPath("bedrock_plugin");
        std::cout << "Loading AWS Bedrock plugin from: " << pluginPath << std::endl;
        
        // Load the plugin library
        PluginLoader loader(pluginPath);
        
        // Create a plugin instance
        auto basePlugin = loader.createInstance();
        std::cout << "Successfully loaded plugin: " << basePlugin->getName() << std::endl;
        
        // Cast to BedrockPlugin to access specialized methods
        // Note: In a production system, we might use a safer downcast mechanism
        BedrockPlugin* bedrockPlugin = dynamic_cast<BedrockPlugin*>(basePlugin.get());
        if (!bedrockPlugin) {
            throw std::runtime_error("Failed to cast to BedrockPlugin");
        }
        
        // Set custom system prompt
        bedrockPlugin->setSystemPrompt(
            "You are an AI expert who provides brief, technical responses about software engineering."
        );
        
        // Conversation loop with the plugin
        std::string prompt;
        std::string response;
        
        while (true) {
            std::cout << "\nEnter a prompt for AWS Bedrock (or 'quit' to exit): ";
            std::getline(std::cin, prompt);
            
            if (prompt == "quit" || prompt == "exit") {
                break;
            }
            
            // Send the prompt to Bedrock
            response = bedrockPlugin->converse(prompt);
            
            // Display the response
            std::cout << "\n--- Response from AWS Bedrock ---\n";
            std::cout << response << std::endl;
            std::cout << "-------------------------------\n";
        }
        
        std::cout << "\nExiting AWS Bedrock demo" << std::endl;
        
        // Plugin will be destroyed when we exit scope
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
