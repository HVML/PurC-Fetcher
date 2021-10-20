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
#include "LibWebRTCResolverIdentifier.h"
#include "MessageNames.h"
#include "NetworkRTCProviderMessagesReplies.h"
#include "RTCNetwork.h"
#include <WebCore/LibWebRTCSocketIdentifier.h>
#include <wtf/Forward.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/text/WTFString.h>


namespace Messages {
namespace NetworkRTCProvider {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::NetworkRTCProvider;
}

class CreateUDPSocket {
public:
    using Arguments = std::tuple<const WebCore::LibWebRTCSocketIdentifier&, const WebKit::RTCNetwork::SocketAddress&, uint16_t, uint16_t>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkRTCProvider_CreateUDPSocket; }
    static const bool isSync = false;

    CreateUDPSocket(const WebCore::LibWebRTCSocketIdentifier& identifier, const WebKit::RTCNetwork::SocketAddress& localAddress, uint16_t minPort, uint16_t maxPort)
        : m_arguments(identifier, localAddress, minPort, maxPort)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class CreateServerTCPSocket {
public:
    using Arguments = std::tuple<const WebCore::LibWebRTCSocketIdentifier&, const WebKit::RTCNetwork::SocketAddress&, uint16_t, uint16_t, const int&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkRTCProvider_CreateServerTCPSocket; }
    static const bool isSync = false;

    CreateServerTCPSocket(const WebCore::LibWebRTCSocketIdentifier& identifier, const WebKit::RTCNetwork::SocketAddress& localAddress, uint16_t minPort, uint16_t maxPort, const int& options)
        : m_arguments(identifier, localAddress, minPort, maxPort, options)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class CreateClientTCPSocket {
public:
    using Arguments = std::tuple<const WebCore::LibWebRTCSocketIdentifier&, const WebKit::RTCNetwork::SocketAddress&, const WebKit::RTCNetwork::SocketAddress&, const String&, const int&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkRTCProvider_CreateClientTCPSocket; }
    static const bool isSync = false;

    CreateClientTCPSocket(const WebCore::LibWebRTCSocketIdentifier& identifier, const WebKit::RTCNetwork::SocketAddress& localAddress, const WebKit::RTCNetwork::SocketAddress& remoteAddress, const String& userAgent, const int& options)
        : m_arguments(identifier, localAddress, remoteAddress, userAgent, options)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class WrapNewTCPConnection {
public:
    using Arguments = std::tuple<const WebCore::LibWebRTCSocketIdentifier&, const WebCore::LibWebRTCSocketIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkRTCProvider_WrapNewTCPConnection; }
    static const bool isSync = false;

    WrapNewTCPConnection(const WebCore::LibWebRTCSocketIdentifier& identifier, const WebCore::LibWebRTCSocketIdentifier& newConnectionSocketIdentifier)
        : m_arguments(identifier, newConnectionSocketIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class CreateResolver {
public:
    using Arguments = std::tuple<const WebKit::LibWebRTCResolverIdentifier&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkRTCProvider_CreateResolver; }
    static const bool isSync = false;

    CreateResolver(const WebKit::LibWebRTCResolverIdentifier& identifier, const String& address)
        : m_arguments(identifier, address)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class StopResolver {
public:
    using Arguments = std::tuple<const WebKit::LibWebRTCResolverIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkRTCProvider_StopResolver; }
    static const bool isSync = false;

    explicit StopResolver(const WebKit::LibWebRTCResolverIdentifier& identifier)
        : m_arguments(identifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

} // namespace NetworkRTCProvider
} // namespace Messages

#endif // USE(LIBWEBRTC)
