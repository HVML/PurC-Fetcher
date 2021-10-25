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
#include "DownloadProxyMessagesReplies.h"
#include "MessageNames.h"
#include <wtf/Forward.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/text/WTFString.h>

namespace IPC {
class DataReference;
}

namespace WebCore {
class AuthenticationChallenge;
class ResourceError;
class ResourceRequest;
class ResourceResponse;
}

namespace WebKit {
class DownloadID;
}

namespace Messages {
namespace DownloadProxy {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::DownloadProxy;
}

class DidStart {
public:
    using Arguments = std::tuple<const WebCore::ResourceRequest&, const AtomString&>;

    static IPC::MessageName name() { return IPC::MessageName::DownloadProxy_DidStart; }
    static const bool isSync = false;

    DidStart(const WebCore::ResourceRequest& request, const AtomString& suggestedFilename)
        : m_arguments(request, suggestedFilename)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidReceiveAuthenticationChallenge {
public:
    using Arguments = std::tuple<const WebCore::AuthenticationChallenge&, uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::DownloadProxy_DidReceiveAuthenticationChallenge; }
    static const bool isSync = false;

    DidReceiveAuthenticationChallenge(const WebCore::AuthenticationChallenge& challenge, uint64_t challengeID)
        : m_arguments(challenge, challengeID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class WillSendRequest {
public:
    using Arguments = std::tuple<const WebCore::ResourceRequest&, const WebCore::ResourceResponse&>;

    static IPC::MessageName name() { return IPC::MessageName::DownloadProxy_WillSendRequest; }
    static const bool isSync = false;

    WillSendRequest(const WebCore::ResourceRequest& redirectRequest, const WebCore::ResourceResponse& redirectResponse)
        : m_arguments(redirectRequest, redirectResponse)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DecideDestinationWithSuggestedFilenameAsync {
public:
    using Arguments = std::tuple<const WebKit::DownloadID&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::DownloadProxy_DecideDestinationWithSuggestedFilenameAsync; }
    static const bool isSync = false;

    DecideDestinationWithSuggestedFilenameAsync(const WebKit::DownloadID& downloadID, const String& suggestedFilename)
        : m_arguments(downloadID, suggestedFilename)
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
    using Arguments = std::tuple<const WebCore::ResourceResponse&>;

    static IPC::MessageName name() { return IPC::MessageName::DownloadProxy_DidReceiveResponse; }
    static const bool isSync = false;

    explicit DidReceiveResponse(const WebCore::ResourceResponse& response)
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

class DidReceiveData {
public:
    using Arguments = std::tuple<uint64_t, uint64_t, uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::DownloadProxy_DidReceiveData; }
    static const bool isSync = false;

    DidReceiveData(uint64_t bytesWritten, uint64_t totalBytesWritten, uint64_t totalBytesExpectedToWrite)
        : m_arguments(bytesWritten, totalBytesWritten, totalBytesExpectedToWrite)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidCreateDestination {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::DownloadProxy_DidCreateDestination; }
    static const bool isSync = false;

    explicit DidCreateDestination(const String& path)
        : m_arguments(path)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidFinish {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::DownloadProxy_DidFinish; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidFail {
public:
    using Arguments = std::tuple<const WebCore::ResourceError&, const IPC::DataReference&>;

    static IPC::MessageName name() { return IPC::MessageName::DownloadProxy_DidFail; }
    static const bool isSync = false;

    DidFail(const WebCore::ResourceError& error, const IPC::DataReference& resumeData)
        : m_arguments(error, resumeData)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidCancel {
public:
    using Arguments = std::tuple<const IPC::DataReference&>;

    static IPC::MessageName name() { return IPC::MessageName::DownloadProxy_DidCancel; }
    static const bool isSync = false;

    explicit DidCancel(const IPC::DataReference& resumeData)
        : m_arguments(resumeData)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

} // namespace DownloadProxy
} // namespace Messages
