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
#include "WebSocketStreamMessagesReplies.h"
#include <wtf/Forward.h>
#include <wtf/ThreadSafeRefCounted.h>

namespace IPC {
class DataReference;
}

namespace WebCore {
class SocketStreamError;
}

namespace Messages {
namespace WebSocketStream {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::WebSocketStream;
}

class DidOpenSocketStream {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebSocketStream_DidOpenSocketStream; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidCloseSocketStream {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebSocketStream_DidCloseSocketStream; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidReceiveSocketStreamData {
public:
    using Arguments = std::tuple<const IPC::DataReference&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSocketStream_DidReceiveSocketStreamData; }
    static const bool isSync = false;

    explicit DidReceiveSocketStreamData(const IPC::DataReference& data)
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

class DidFailToReceiveSocketStreamData {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebSocketStream_DidFailToReceiveSocketStreamData; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidUpdateBufferedAmount {
public:
    using Arguments = std::tuple<uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::WebSocketStream_DidUpdateBufferedAmount; }
    static const bool isSync = false;

    explicit DidUpdateBufferedAmount(uint64_t bufferedAmount)
        : m_arguments(bufferedAmount)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidFailSocketStream {
public:
    using Arguments = std::tuple<const WebCore::SocketStreamError&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSocketStream_DidFailSocketStream; }
    static const bool isSync = false;

    explicit DidFailSocketStream(const WebCore::SocketStreamError& error)
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

class DidSendData {
public:
    using Arguments = std::tuple<uint64_t, bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebSocketStream_DidSendData; }
    static const bool isSync = false;

    DidSendData(uint64_t identifier, bool success)
        : m_arguments(identifier, success)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidSendHandshake {
public:
    using Arguments = std::tuple<uint64_t, bool, bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebSocketStream_DidSendHandshake; }
    static const bool isSync = false;

    DidSendHandshake(uint64_t identifier, bool success, bool didAccessSecureCookies)
        : m_arguments(identifier, success, didAccessSecureCookies)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

} // namespace WebSocketStream
} // namespace Messages
