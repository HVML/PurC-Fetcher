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
#include "WebSocketChannelMessagesReplies.h"
#include <wtf/Forward.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/text/WTFString.h>

namespace IPC {
class DataReference;
}

namespace WebCore {
class ResourceRequest;
class ResourceResponse;
}

namespace Messages {
namespace WebSocketChannel {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::WebSocketChannel;
}

class DidConnect {
public:
    using Arguments = std::tuple<const String&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSocketChannel_DidConnect; }
    static const bool isSync = false;

    DidConnect(const String& subprotocol, const String& extensions)
        : m_arguments(subprotocol, extensions)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidClose {
public:
    using Arguments = std::tuple<const unsigned short&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSocketChannel_DidClose; }
    static const bool isSync = false;

    DidClose(const unsigned short& code, const String& reason)
        : m_arguments(code, reason)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidReceiveText {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSocketChannel_DidReceiveText; }
    static const bool isSync = false;

    explicit DidReceiveText(const String& message)
        : m_arguments(message)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidReceiveBinaryData {
public:
    using Arguments = std::tuple<const IPC::DataReference&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSocketChannel_DidReceiveBinaryData; }
    static const bool isSync = false;

    explicit DidReceiveBinaryData(const IPC::DataReference& data)
        : m_arguments(data)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidReceiveMessageError {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSocketChannel_DidReceiveMessageError; }
    static const bool isSync = false;

    explicit DidReceiveMessageError(const String& errorMessage)
        : m_arguments(errorMessage)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidSendHandshakeRequest {
public:
    using Arguments = std::tuple<const WebCore::ResourceRequest&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSocketChannel_DidSendHandshakeRequest; }
    static const bool isSync = false;

    explicit DidSendHandshakeRequest(const WebCore::ResourceRequest& request)
        : m_arguments(request)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidReceiveHandshakeResponse {
public:
    using Arguments = std::tuple<const WebCore::ResourceResponse&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSocketChannel_DidReceiveHandshakeResponse; }
    static const bool isSync = false;

    explicit DidReceiveHandshakeResponse(const WebCore::ResourceResponse& response)
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

} // namespace WebSocketChannel
} // namespace Messages
