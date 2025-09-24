#!/bin/bash
set -e  # Exit on first error

echo "=== AWS SDK Compatibility Fix Script ==="
echo "This script will create compatibility patches for the AWS SDK headers."
echo ""

# Check if the problematic files exist
ENDPOINT_PROVIDER_PATH="/usr/local/include/aws/bedrock-runtime/BedrockRuntimeEndpointProvider.h"
CLIENT_MODEL_PATH="/usr/local/include/aws/bedrock-runtime/BedrockRuntimeServiceClientModel.h"

if [ ! -f "$ENDPOINT_PROVIDER_PATH" ] || [ ! -f "$CLIENT_MODEL_PATH" ]; then
    echo "Error: AWS SDK header files not found in expected locations."
    echo "Please make sure the AWS SDK is installed correctly."
    exit 1
fi

# Create backup of the original files
echo "Creating backups of original header files..."
if [ ! -f "${ENDPOINT_PROVIDER_PATH}.orig" ]; then
    sudo cp "$ENDPOINT_PROVIDER_PATH" "${ENDPOINT_PROVIDER_PATH}.orig"
fi

if [ ! -f "${CLIENT_MODEL_PATH}.orig" ]; then
    sudo cp "$CLIENT_MODEL_PATH" "${CLIENT_MODEL_PATH}.orig"
fi

# Add compatibility patch to the header files
echo "Applying compatibility patches..."

# Create temporary files with our modifications
TMP_ENDPOINT=$(mktemp)
cat > "$TMP_ENDPOINT" << 'EOF'
/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/bedrock-runtime/BedrockRuntimeEndpointRules.h>
#include <aws/bedrock-runtime/BedrockRuntime_EXPORTS.h>
// Fix for SDK version mismatch
#ifndef GenericClientConfiguration
#define GenericClientConfiguration ClientConfiguration
#endif
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/endpoint/DefaultEndpointProvider.h>
#include <aws/core/endpoint/EndpointParameter.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>

namespace Aws {
namespace BedrockRuntime {
namespace Endpoint {
using EndpointParameters = Aws::Endpoint::EndpointParameters;
using Aws::Endpoint::DefaultEndpointProvider;
using Aws::Endpoint::EndpointProviderBase;

using BedrockRuntimeClientContextParameters = Aws::Endpoint::ClientContextParameters;

using BedrockRuntimeClientConfiguration = Aws::Client::ClientConfiguration;
using BedrockRuntimeBuiltInParameters = Aws::Endpoint::BuiltInParameters;

/**
 * The type for the BedrockRuntime Client Endpoint Provider.
 * Inherit from this Base class / "Interface" should you want to provide a custom endpoint provider.
 * The SDK must use service-specific type for each service per specification.
 */
EOF

TMP_CLIENT_MODEL=$(mktemp)
cat > "$TMP_CLIENT_MODEL" << 'EOF'
/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

/* Generic header includes */
#include <aws/bedrock-runtime/BedrockRuntimeEndpointProvider.h>
#include <aws/bedrock-runtime/BedrockRuntimeErrors.h>
#include <aws/core/client/AWSError.h>
#include <aws/core/client/AsyncCallerContext.h>
// Fix for SDK version mismatch
#ifndef GenericClientConfiguration
#define GenericClientConfiguration ClientConfiguration
#endif
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/http/HttpTypes.h>
#include <aws/core/utils/memory/stl/AWSString.h>

#include <functional>
#include <future>
/* End of generic header includes */

/* Service model headers required in BedrockRuntimeClient header */
#include <aws/bedrock-runtime/model/ApplyGuardrailResult.h>
#include <aws/bedrock-runtime/model/ConverseResult.h>
#include <aws/bedrock-runtime/model/CountTokensResult.h>
#include <aws/bedrock-runtime/model/GetAsyncInvokeResult.h>
#include <aws/bedrock-runtime/model/InvokeModelResult.h>
#include <aws/bedrock-runtime/model/ListAsyncInvokesRequest.h>
#include <aws/bedrock-runtime/model/ListAsyncInvokesResult.h>
#include <aws/bedrock-runtime/model/StartAsyncInvokeResult.h>
#include <aws/core/NoResult.h>
/* End of service model headers required in BedrockRuntimeClient header */

namespace Aws {
namespace Http {
class HttpClient;
class HttpClientFactory;
}  // namespace Http

namespace Utils {
template <typename R, typename E>
class Outcome;

namespace Event {
class EventEncoderStream;
}  // namespace Event

namespace Threading {
class Executor;
}  // namespace Threading
}  // namespace Utils

namespace Auth {
class AWSCredentials;
class AWSCredentialsProvider;
}  // namespace Auth

namespace Client {
class RetryStrategy;
}  // namespace Client

namespace BedrockRuntime {
using BedrockRuntimeClientConfiguration = Aws::Client::ClientConfiguration;
using BedrockRuntimeEndpointProviderBase = Aws::BedrockRuntime::Endpoint::BedrockRuntimeEndpointProviderBase;
using BedrockRuntimeEndpointProvider = Aws::BedrockRuntime::Endpoint::BedrockRuntimeEndpointProvider;

namespace Model {
/* Service model forward declarations required in BedrockRuntimeClient header */
class ApplyGuardrailRequest;
class ConverseRequest;
class ConverseStreamRequest;
EOF

# Apply patches with sudo
echo "Applying patches to AWS SDK headers (may require password)..."
sudo cp "$TMP_ENDPOINT" "$ENDPOINT_PROVIDER_PATH"
sudo cp "$TMP_CLIENT_MODEL" "$CLIENT_MODEL_PATH"

# Clean up
rm "$TMP_ENDPOINT" "$TMP_CLIENT_MODEL"

echo ""
echo "Patching complete! Try building your project again."
echo "If you need to restore the original files, you can use the .orig backups."
echo ""
