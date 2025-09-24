#include "aws_bedrock_plugin.h"
#include <iostream>
#include <sstream>
#include <thread>

// Check if the AWS SDK is available
#if __has_include(<aws/core/Aws.h>) && \
    __has_include(<aws/core/client/ClientConfiguration.h>)

// AWS SDK includes - only include what we need
#include <aws/core/Aws.h>
#include <aws/core/client/ClientConfiguration.h>

// Forward declare types we need
namespace Aws {
namespace BedrockRuntime {
class BedrockRuntimeClient;
namespace Model {
class InvokeModelRequest;
class InvokeModelResult;
}
}
namespace Bedrock {
class BedrockClient;
namespace Model {
class InvokeModelRequest;
class InvokeModelResult;
}
}
}

// Define BEDROCK_RUNTIME_CLIENT_TYPE macro to help us choose which client to use at compile time
#if __has_include(<aws/bedrock-runtime/BedrockRuntimeClient.h>)
  #define BEDROCK_RUNTIME_CLIENT_TYPE Aws::BedrockRuntime::BedrockRuntimeClient
  #define BEDROCK_REQUEST_TYPE Aws::BedrockRuntime::Model::InvokeModelRequest
  #define BEDROCK_RESULT_TYPE Aws::BedrockRuntime::Model::InvokeModelResult
  #include <aws/bedrock-runtime/BedrockRuntimeClient.h>
  #include <aws/bedrock-runtime/model/InvokeModelRequest.h>
  #include <aws/bedrock-runtime/model/InvokeModelResult.h>
  #define AWS_BEDROCK_AVAILABLE 1
#elif __has_include(<aws/bedrock/BedrockClient.h>)
  #define BEDROCK_RUNTIME_CLIENT_TYPE Aws::Bedrock::BedrockClient
  #define BEDROCK_REQUEST_TYPE Aws::Bedrock::Model::InvokeModelRequest
  #define BEDROCK_RESULT_TYPE Aws::Bedrock::Model::InvokeModelResult
  #include <aws/bedrock/BedrockClient.h>
  #include <aws/bedrock/model/InvokeModelRequest.h>
  #include <aws/bedrock/model/InvokeModelResult.h>
  #define AWS_BEDROCK_AVAILABLE 1
#else
  #define AWS_BEDROCK_AVAILABLE 0
#endif

#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/core/utils/memory/AWSMemory.h>
#include <aws/core/utils/stream/ResponseStream.h>
#include <aws/core/utils/StringUtils.h>

#else
  #define AWS_BEDROCK_AVAILABLE 0
#endif

#if AWS_BEDROCK_AVAILABLE
  // Initialize the AWS SDK - will be called once when the plugin is loaded
  static Aws::SDKOptions sdkOptions;
  static bool awsInitialized = false;
  
  // This is needed to handle our static deleter function properly
  struct BedrockClientDeleter {
    static void DestroyClient(void* ptr) {
      #if defined(BEDROCK_RUNTIME_CLIENT_TYPE)
        delete static_cast<BEDROCK_RUNTIME_CLIENT_TYPE*>(ptr);
      #endif
    }
  };
#endif

BedrockPlugin::BedrockPlugin(const std::string& region, const std::string& model)
    // Initialize member variables in declaration order to avoid warnings
    : bedrockClient(nullptr, [](void*){}),
      region(region),
      modelId(model) {
    
    std::cout << "Creating BedrockPlugin instance with model: " << model << std::endl;
    
#if AWS_BEDROCK_AVAILABLE
    // Initialize AWS SDK if not already done
    initializeAWS();
    
    // Create client configuration
    clientConfig = std::make_unique<Aws::Client::ClientConfiguration>();
    clientConfig->region = region;
    
    // Use our static deleter function for the client
    auto clientDeleter = &BedrockClientDeleter::DestroyClient;
    
    // Create Bedrock Runtime client using our type macro
    void* rawClient = nullptr;
    #if defined(BEDROCK_RUNTIME_CLIENT_TYPE)
        Aws::Client::ClientConfiguration config;
        config.region = region;
        rawClient = new BEDROCK_RUNTIME_CLIENT_TYPE(config);
    #endif
    
    // Assign the client to our unique_ptr with custom deleter
    bedrockClient = std::unique_ptr<void, void(*)(void*)>(rawClient, clientDeleter);
#else
    std::cout << "WARNING: AWS SDK not available or Bedrock headers not found." << std::endl;
    std::cout << "         Using mock implementation instead." << std::endl;
#endif
    
    // Default system prompt
    systemPrompt = "You are a helpful AI assistant that provides concise and accurate information.";
}

BedrockPlugin::~BedrockPlugin() {
    std::cout << "Destroying BedrockPlugin instance" << std::endl;
    
#if AWS_BEDROCK_AVAILABLE
    // Release resources in reverse order
    bedrockClient.reset();
    clientConfig.reset();
#endif
}

#if AWS_BEDROCK_AVAILABLE
void BedrockPlugin::initializeAWS() {
    if (!awsInitialized) {
        std::cout << "Initializing AWS SDK" << std::endl;
        Aws::InitAPI(sdkOptions);
        awsInitialized = true;
    }
}
#endif

std::string BedrockPlugin::getName() const {
    return "BedrockPlugin";
}

int BedrockPlugin::processData(int input) const {
    // This is required by the interface but not our primary function
    std::cout << "BedrockPlugin: processData called with " << input << std::endl;
    return input; // Just pass through the value
}

std::string BedrockPlugin::converse(const std::string& prompt) {
    std::cout << "BedrockPlugin: Processing prompt: " << prompt << std::endl;

#if AWS_BEDROCK_AVAILABLE
    try {
        // Create the InvokeModel request - using Anthropic's Claude model
        #if defined(BEDROCK_REQUEST_TYPE)
          BEDROCK_REQUEST_TYPE request;
        #endif

        // Set the model ID
        request.SetModelId(modelId.c_str());

        // Create the request body for Claude
        std::ostringstream jsonPayload;
        jsonPayload << R"({
            "anthropic_version": "bedrock-2023-05-31",
            "system": ")" << systemPrompt << R"(",
            "messages": [
                {
                    "role": "user",
                    "content": [
                        {
                            "type": "text",
                            "text": ")" << prompt << R"("
                        }
                    ]
                }
            ],
            "max_tokens": 1000
        })";

        // Convert the JSON payload to a shared iostream which the SDK expects
        const std::string payload = jsonPayload.str();
        auto payloadStream = Aws::MakeShared<Aws::StringStream>("Payload");
        *payloadStream << payload;
        request.SetBody(payloadStream);
        request.SetContentType("application/json");

        // Send the request to Bedrock using our type macro
        #if defined(BEDROCK_RUNTIME_CLIENT_TYPE)
            auto runtimeClient = static_cast<BEDROCK_RUNTIME_CLIENT_TYPE*>(bedrockClient.get());
            auto outcome = runtimeClient->InvokeModel(request);
        #endif

        if (outcome.IsSuccess()) {
            // Get the response body - read it from the stream into a string
            std::stringstream responseStream;
            auto& bodyStream = outcome.GetResult().GetBody();
            responseStream << bodyStream.rdbuf();
            std::string responseString = responseStream.str();

            // Parse the JSON response
            Aws::Utils::Json::JsonValue responseJson(responseString);
            if (responseJson.WasParseSuccessful()) {
                // Try to extract the content based on Claude's response format
                auto view = responseJson.View();
                
                // Check if we have a completion response
                if (view.KeyExists("completion")) {
                    return view.GetString("completion");
                }
                
                // Check for content array (newer format)
                if (view.KeyExists("content")) {
                    auto contentView = view.GetArray("content");
                    // Check array size
                    if (contentView.GetLength() > 0) {
                        for (size_t i = 0; i < contentView.GetLength(); i++) {
                            auto contentItem = contentView[i];
                            if (contentItem.KeyExists("type") && contentItem.GetString("type") == "text") {
                                return contentItem.GetString("text");
                            }
                        }
                    }
                }
                
                // Fallback: return the entire response as string
                return responseString;
            } else {
                return "Error parsing JSON response from Bedrock";
            }
        } else {
            std::ostringstream errorMsg;
            errorMsg << "Error calling Bedrock: " 
                    << outcome.GetError().GetExceptionName() << " - " 
                    << outcome.GetError().GetMessage();
            return errorMsg.str();
        }
    } catch (const std::exception& e) {
        std::ostringstream errorMsg;
        errorMsg << "Exception in converse: " << e.what();
        return errorMsg.str();
    }
#else
    // Mock implementation when AWS SDK is not available
    std::string lowerPrompt = prompt;
    std::transform(lowerPrompt.begin(), lowerPrompt.end(), lowerPrompt.begin(), 
                  [](unsigned char c){ return std::tolower(c); });
    
    // Add response delay to simulate network latency
    unsigned int delay = 200 + (std::rand() % 800); // 200-1000ms
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    
    // Generate different responses based on prompt content
    if (lowerPrompt.find("c++") != std::string::npos || 
        lowerPrompt.find("cpp") != std::string::npos) {
        return "C++ is a powerful systems programming language that offers performance, "
               "memory control, and hardware access while providing high-level abstractions. "
               "Modern C++ (C++11 and beyond) adds features like smart pointers, lambdas, and improved STL.";
    }
    
    if (lowerPrompt.find("aws") != std::string::npos || 
        lowerPrompt.find("bedrock") != std::string::npos) {
        return "AWS Bedrock provides access to foundation models from companies like "
               "Anthropic, AI21, and Amazon. The Bedrock API enables text generation, "
               "embeddings, and chat-based interactions.";
    }
    
    // Default response
    return "I understand your question about '" + prompt + "'. As an AI assistant, "
           "I'd recommend exploring this topic further with specific examples and use cases.";
#endif
}

void BedrockPlugin::setModel(const std::string& model) {
    modelId = model;
    std::cout << "BedrockPlugin: Model set to " << modelId << std::endl;
}

void BedrockPlugin::setSystemPrompt(const std::string& newSystemPrompt) {
    systemPrompt = newSystemPrompt;
    std::cout << "BedrockPlugin: System prompt updated" << std::endl;
}

// Export the factory functions with C linkage
EXPORT_PLUGIN_API PLUGIN_API PluginInterface* createPlugin() {
    std::cout << "Creating BedrockPlugin instance via factory function" << std::endl;
    return new BedrockPlugin();
}

EXPORT_PLUGIN_API PLUGIN_API void destroyPlugin(PluginInterface* plugin) {
    std::cout << "Destroying BedrockPlugin instance via factory function" << std::endl;
    delete plugin;
}
