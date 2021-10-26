/*
 * Copyright (C) 2010-2020 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "ArgumentCoders.h"
#include "Connection.h"
#include "MessageNames.h"
#include "ShareableResource.h"
#include "WebResourceLoaderMessagesReplies.h"
#include <wtf/Forward.h>
#include <wtf/ThreadSafeRefCounted.h>

namespace IPC {
class DataReference;
class FormDataReference;
class SharedBufferDataReference;
}

namespace WebCore {
class NetworkLoadMetrics;
class ResourceError;
class ResourceRequest;
class ResourceResponse;
}

namespace Messages {
namespace WebResourceLoader {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::WebResourceLoader;
}

class WillSendRequest {
public:
    using Arguments = std::tuple<const WebCore::ResourceRequest&, const IPC::FormDataReference&, const WebCore::ResourceResponse&>;

    static IPC::MessageName name() { return IPC::MessageName::WebResourceLoader_WillSendRequest; }
    static const bool isSync = false;

    WillSendRequest(const WebCore::ResourceRequest& request, const IPC::FormDataReference& requestBody, const WebCore::ResourceResponse& redirectResponse)
        : m_arguments(request, requestBody, redirectResponse)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidSendData {
public:
    using Arguments = std::tuple<uint64_t, uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::WebResourceLoader_DidSendData; }
    static const bool isSync = false;

    DidSendData(uint64_t bytesSent, uint64_t totalBytesToBeSent)
        : m_arguments(bytesSent, totalBytesToBeSent)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidReceiveResponse {
public:
    using Arguments = std::tuple<const WebCore::ResourceResponse&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebResourceLoader_DidReceiveResponse; }
    static const bool isSync = false;

    DidReceiveResponse(const WebCore::ResourceResponse& response, bool needsContinueDidReceiveResponseMessage)
        : m_arguments(response, needsContinueDidReceiveResponseMessage)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidReceiveData {
public:
    using Arguments = std::tuple<const IPC::DataReference&, int64_t>;

    static IPC::MessageName name() { return IPC::MessageName::WebResourceLoader_DidReceiveData; }
    static const bool isSync = false;

    DidReceiveData(const IPC::DataReference& data, int64_t encodedDataLength)
        : m_arguments(data, encodedDataLength)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidReceiveSharedBuffer {
public:
    using Arguments = std::tuple<const IPC::SharedBufferDataReference&, int64_t>;

    static IPC::MessageName name() { return IPC::MessageName::WebResourceLoader_DidReceiveSharedBuffer; }
    static const bool isSync = false;

    DidReceiveSharedBuffer(const IPC::SharedBufferDataReference& data, int64_t encodedDataLength)
        : m_arguments(data, encodedDataLength)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidFinishResourceLoad {
public:
    using Arguments = std::tuple<const WebCore::NetworkLoadMetrics&>;

    static IPC::MessageName name() { return IPC::MessageName::WebResourceLoader_DidFinishResourceLoad; }
    static const bool isSync = false;

    explicit DidFinishResourceLoad(const WebCore::NetworkLoadMetrics& networkLoadMetrics)
        : m_arguments(networkLoadMetrics)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidFailResourceLoad {
public:
    using Arguments = std::tuple<const WebCore::ResourceError&>;

    static IPC::MessageName name() { return IPC::MessageName::WebResourceLoader_DidFailResourceLoad; }
    static const bool isSync = false;

    explicit DidFailResourceLoad(const WebCore::ResourceError& error)
        : m_arguments(error)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidFailServiceWorkerLoad {
public:
    using Arguments = std::tuple<const WebCore::ResourceError&>;

    static IPC::MessageName name() { return IPC::MessageName::WebResourceLoader_DidFailServiceWorkerLoad; }
    static const bool isSync = false;

    explicit DidFailServiceWorkerLoad(const WebCore::ResourceError& error)
        : m_arguments(error)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ServiceWorkerDidNotHandle {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebResourceLoader_ServiceWorkerDidNotHandle; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidBlockAuthenticationChallenge {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebResourceLoader_DidBlockAuthenticationChallenge; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class StopLoadingAfterXFrameOptionsOrContentSecurityPolicyDenied {
public:
    using Arguments = std::tuple<const WebCore::ResourceResponse&>;

    static IPC::MessageName name() { return IPC::MessageName::WebResourceLoader_StopLoadingAfterXFrameOptionsOrContentSecurityPolicyDenied; }
    static const bool isSync = false;

    explicit StopLoadingAfterXFrameOptionsOrContentSecurityPolicyDenied(const WebCore::ResourceResponse& response)
        : m_arguments(response)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if ENABLE(SHAREABLE_RESOURCE)
class DidReceiveResource {
public:
    using Arguments = std::tuple<const WebKit::ShareableResource::Handle&>;

    static IPC::MessageName name() { return IPC::MessageName::WebResourceLoader_DidReceiveResource; }
    static const bool isSync = false;

    explicit DidReceiveResource(const WebKit::ShareableResource::Handle& resource)
        : m_arguments(resource)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

} // namespace WebResourceLoader
} // namespace Messages
