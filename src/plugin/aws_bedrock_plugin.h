#pragma once
#include "plugin_interface.h"
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <algorithm>

// Check if AWS SDK might be available
#if __has_include(<aws/core/Aws.h>)
    #define MIGHT_HAVE_AWS_SDK 1
#else
    #define MIGHT_HAVE_AWS_SDK 0
#endif

// Forward declarations of AWS SDK components
#if MIGHT_HAVE_AWS_SDK
namespace Aws {
    namespace Client {
        class ClientConfiguration;
    }
    namespace Utils {
        namespace Json {
            class JsonValue;
        }
    }
    
    // BedrockRuntime namespace (newer SDKs)
    namespace BedrockRuntime {
        class BedrockRuntimeClient;
    }
    
    // Bedrock namespace (older SDKs)
    namespace Bedrock {
        class BedrockClient;
    }
}
#endif

class BedrockPlugin : public PluginInterface {
public:
    BedrockPlugin(const std::string& region = "us-east-1", 
                  const std::string& model = "anthropic.claude-3-sonnet-20240229-v1:0");
    ~BedrockPlugin();
    
    std::string getName() const override;
    int processData(int input) const override;
    
    // Specialized methods for the Bedrock plugin
    std::string converse(const std::string& prompt);
    void setModel(const std::string& model);
    void setSystemPrompt(const std::string& systemPrompt);

private:
#if MIGHT_HAVE_AWS_SDK
    // AWS SDK components (conditionally compiled)
    std::unique_ptr<Aws::Client::ClientConfiguration> clientConfig;
    
    // We use void* here because we don't know which client type we'll have
    // The implementation file will cast to the appropriate type based on availability
    std::unique_ptr<void, void(*)(void*)> bedrockClient;
    
    // Helper methods
    void initializeAWS();
#else
    // Mock implementation data
    std::vector<std::pair<std::string, std::string>> conversationHistory;
#endif
    
    // Plugin configuration (used by both real and mock implementations)
    std::string region;
    std::string modelId;
    std::string systemPrompt;
};
