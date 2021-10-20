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

#if USE(LIBWEBRTC)

#include "ArgumentCoders.h"
#include "Connection.h"
#include "MessageNames.h"
#include "NetworkRTCSocketMessagesReplies.h"
#include "RTCNetwork.h"
#include <wtf/Forward.h>
#include <wtf/ThreadSafeRefCounted.h>

namespace IPC {
class DataReference;
}

namespace WebKit {
struct RTCPacketOptions;
}

namespace Messages {
namespace NetworkRTCSocket {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::NetworkRTCSocket;
}

class SendTo {
public:
    using Arguments = std::tuple<const IPC::DataReference&, const WebKit::RTCNetwork::SocketAddress&, const WebKit::RTCPacketOptions&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkRTCSocket_SendTo; }
    static const bool isSync = false;

    SendTo(const IPC::DataReference& data, const WebKit::RTCNetwork::SocketAddress& address, const WebKit::RTCPacketOptions& options)
        : m_arguments(data, address, options)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class Close {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkRTCSocket_Close; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetOption {
public:
    using Arguments = std::tuple<const int&, const int&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkRTCSocket_SetOption; }
    static const bool isSync = false;

    SetOption(const int& option, const int& value)
        : m_arguments(option, value)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

} // namespace NetworkRTCSocket
} // namespace Messages

#endif // USE(LIBWEBRTC)
