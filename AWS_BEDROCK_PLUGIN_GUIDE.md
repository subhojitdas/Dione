# AWS Bedrock Plugin Integration Guide

This guide explains the AWS Bedrock plugin that we've created to integrate with the plugin system. This plugin allows you to interact with the AWS Bedrock service for accessing large language models (LLMs) like Claude from Anthropic.

> **Note:** This project includes both a real AWS Bedrock implementation and a mock implementation for development purposes. The mock implementation does not require AWS SDK installation or credentials.

## Overview

The AWS Bedrock plugin is designed as a dynamically loadable module that extends our plugin system to interact with AWS Bedrock's Converse API (or to simulate these interactions in mock mode). This demonstrates how a C++ application can make use of AWS services through dynamically loaded libraries.

## Components

1. **AWS Bedrock Plugin Interface** (`aws_bedrock_plugin.h`):
   - Extends the base PluginInterface
   - Adds specialized methods for working with AWS Bedrock
   - Handles AWS SDK initialization and cleanup

2. **AWS Bedrock Plugin Implementation** (`aws_bedrock_plugin.cpp`):
   - Implements the BedrockPlugin class
   - Manages AWS SDK resources
   - Formats and sends requests to AWS Bedrock

3. **Bedrock Client Application** (`bedrock_client.cpp`):
   - Demonstrates loading and using the plugin
   - Provides an interactive command-line interface for conversations

4. **CMake Integration** (updated `CMakeLists.txt`):
   - Conditional building of AWS components
   - Links against AWS SDK libraries
   - Creates the plugin library and client executable

5. **Build Script** (`build_bedrock_plugin.sh`):
   - Checks for AWS SDK installation
   - Builds the project with AWS modules enabled
   - Provides instructions for installation if needed

## Prerequisites

### For Mock Implementation (Default)

The mock implementation has no special requirements:
- C++20 compatible compiler
- CMake 3.16 or higher
- Standard C++ libraries

### For Real AWS Bedrock Integration

> **IMPORTANT NOTE:** AWS Bedrock is a relatively new service, and the "bedrock-runtime" component may not be available in all versions of the AWS SDK for C++. The mock implementation is recommended for most users.

If you still want to use the real AWS Bedrock service, there are several options:

#### Option 1: Use a recent AWS SDK version (if available)

1. **AWS SDK for C++** (with latest version that supports Bedrock):
   ```bash
   git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp.git
   cd aws-sdk-cpp
   # Make sure you're on the latest version
   git checkout main
   git pull
   
   # Build with Bedrock components
   mkdir -p build
   cd build
   
   # Try with bedrock component name
   cmake .. -DBUILD_ONLY="bedrock;bedrock-runtime;core" -DCMAKE_BUILD_TYPE=Release
   
   # If the above fails, you might need to build all components
   # cmake .. -DBUILD_ONLY="ALL" -DCMAKE_BUILD_TYPE=Release
   
   make -j
   sudo make install
   ```

#### Option 2: Use AWS CLI or other tools

As an alternative, you could use the AWS Command Line Interface (CLI) or other tools to interact with AWS Bedrock instead of the C++ SDK. The AWS CLI is more likely to have up-to-date support for newer services.

#### Option 3: Continue using the mock implementation

The mock implementation provides a good simulation of the AWS Bedrock service for development and testing purposes, without requiring the actual AWS SDK or credentials.

2. **AWS Credentials**:
   - Configure credentials in `~/.aws/credentials` or environment variables
   - Ensure you have permissions to access AWS Bedrock

3. **AWS Bedrock Access**:
   - Make sure your AWS account has access to AWS Bedrock and the models you want to use

4. **Configuration Changes**:
   - Edit CMakeLists.txt to set `USE_MOCK_BEDROCK` to OFF

## Building the AWS Bedrock Plugin

### Mock Implementation (Default)

Run the build script:

```bash
./build_bedrock_plugin.sh
```

This will:
1. Clean the build directory
2. Configure CMake with the mock implementation
3. Build the plugin library and client application

### Real AWS Implementation

To build with the real AWS integration:

1. Install AWS SDK for C++ (see prerequisites)
2. Modify CMakeLists.txt to set `USE_MOCK_BEDROCK` to OFF
3. Run the build script

## Running the AWS Bedrock Client

Execute:

```bash
./build/bedrock_client
```

This will:
1. Load the AWS Bedrock plugin
2. Start an interactive session where you can send prompts to AWS Bedrock
3. Display responses from the selected model (default is Claude)

## Plugin Architecture

### Dynamic Loading

The plugin is loaded at runtime using the same plugin loader mechanism as our previous example:

```cpp
PluginLoader loader(pluginPath);
auto basePlugin = loader.createInstance();
```

### Accessing Specialized Methods

Since the BedrockPlugin extends the base interface with specialized methods, we need to downcast:

```cpp
BedrockPlugin* bedrockPlugin = dynamic_cast<BedrockPlugin*>(basePlugin.get());
```

This allows access to methods like `converse()` and `setSystemPrompt()`.

### AWS SDK Integration

The plugin handles AWS SDK initialization and cleanup:

```cpp
void BedrockPlugin::initializeAWS() {
    if (!awsInitialized) {
        Aws::InitAPI(sdkOptions);
        awsInitialized = true;
    }
}
```

## Customization

You can customize the plugin behavior:

1. **Change the model**:
   ```cpp
   bedrockPlugin->setModel("anthropic.claude-3-haiku-20240307-v1:0");
   ```

2. **Set a custom system prompt**:
   ```cpp
   bedrockPlugin->setSystemPrompt("You are an AI expert who specializes in...");
   ```

3. **Modify request parameters**:
   - Edit `configureConverseRequest()` to change parameters like `max_tokens`

## Error Handling

The plugin includes comprehensive error handling:

- AWS SDK initialization errors
- Plugin loading errors
- API call failures
- Response parsing issues

## Further Development

Possible enhancements:

1. Add support for streaming responses
2. Include image input capabilities
3. Add more AWS Bedrock models
4. Implement a message history mechanism
5. Create a GUI frontend

## Mock Implementation Details

The mock implementation:

1. **Simulates AI Responses**:
   - Provides pre-defined responses for common queries
   - Uses keyword detection to generate contextual responses
   - Adds a random delay to simulate network latency

2. **Maintains Conversation History**:
   - Tracks previous exchanges for basic context awareness
   - Uses system prompts to influence response style

3. **Benefits**:
   - No need for AWS SDK installation
   - No credentials or AWS account required
   - Instant setup for development purposes
   - Predictable responses for testing

4. **Limitations**:
   - Limited response variety
   - No actual AI/ML capabilities
   - No streaming responses

## Troubleshooting

### For Mock Implementation

1. **Build errors**:
   - Make sure you have a C++20 compatible compiler
   - Check that CMake is version 3.16 or higher
   
2. **Runtime errors**:
   - Verify that the mock plugin library is built correctly
   - Make sure file paths are correct

### For Real AWS Implementation

1. **AWS SDK not found**:
   - Ensure the SDK is installed correctly
   - Check pkg-config path includes AWS SDK

2. **Authentication failures**:
   - Verify AWS credentials are properly configured
   - Check permissions for AWS Bedrock

3. **Model access issues**:
   - Confirm your AWS account has access to the model specified
   - Check for any model-specific requirements

4. **Build errors**:
   - Make sure all AWS SDK dependencies are installed
   - Check that you're building with C++17 or higher
