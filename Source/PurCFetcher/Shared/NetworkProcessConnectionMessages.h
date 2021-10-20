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
#include "NetworkProcessConnectionMessagesReplies.h"
#include "ShareableResource.h"
#include <WebCore/Cookie.h>
#include <WebCore/MessagePortChannelProvider.h>
#include <wtf/Forward.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace JSC {
enum class MessageLevel : uint8_t;
enum class MessageSource : uint8_t;
}

namespace WebCore {
class ResourceError;
class ResourceRequest;
class ResourceResponse;
enum class HTTPCookieAcceptPolicy : uint8_t;
struct MessagePortIdentifier;
}

namespace Messages {
namespace NetworkProcessConnection {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::NetworkProcessConnection;
}

#if ENABLE(SHAREABLE_RESOURCE)
class DidCacheResource {
public:
    using Arguments = std::tuple<const WebCore::ResourceRequest&, const WebKit::ShareableResource::Handle&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessConnection_DidCacheResource; }
    static const bool isSync = false;

    DidCacheResource(const WebCore::ResourceRequest& request, const WebKit::ShareableResource::Handle& resource)
        : m_arguments(request, resource)
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

class DidFinishPingLoad {
public:
    using Arguments = std::tuple<uint64_t, const WebCore::ResourceError&, const WebCore::ResourceResponse&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessConnection_DidFinishPingLoad; }
    static const bool isSync = false;

    DidFinishPingLoad(uint64_t pingLoadIdentifier, const WebCore::ResourceError& error, const WebCore::ResourceResponse& response)
        : m_arguments(pingLoadIdentifier, error, response)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidFinishPreconnection {
public:
    using Arguments = std::tuple<uint64_t, const WebCore::ResourceError&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessConnection_DidFinishPreconnection; }
    static const bool isSync = false;

    DidFinishPreconnection(uint64_t preconnectionIdentifier, const WebCore::ResourceError& error)
        : m_arguments(preconnectionIdentifier, error)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetOnLineState {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessConnection_SetOnLineState; }
    static const bool isSync = false;

    explicit SetOnLineState(bool isOnLine)
        : m_arguments(isOnLine)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class CookieAcceptPolicyChanged {
public:
    using Arguments = std::tuple<WebCore::HTTPCookieAcceptPolicy>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessConnection_CookieAcceptPolicyChanged; }
    static const bool isSync = false;

    explicit CookieAcceptPolicyChanged(WebCore::HTTPCookieAcceptPolicy policy)
        : m_arguments(policy)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if HAVE(COOKIE_CHANGE_LISTENER_API)
class CookiesAdded {
public:
    using Arguments = std::tuple<const String&, const Vector<struct WebCore::Cookie>&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessConnection_CookiesAdded; }
    static const bool isSync = false;

    CookiesAdded(const String& host, const Vector<struct WebCore::Cookie>& cookies)
        : m_arguments(host, cookies)
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

#if HAVE(COOKIE_CHANGE_LISTENER_API)
class CookiesDeleted {
public:
    using Arguments = std::tuple<const String&, const Vector<struct WebCore::Cookie>&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessConnection_CookiesDeleted; }
    static const bool isSync = false;

    CookiesDeleted(const String& host, const Vector<struct WebCore::Cookie>& cookies)
        : m_arguments(host, cookies)
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

#if HAVE(COOKIE_CHANGE_LISTENER_API)
class AllCookiesDeleted {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessConnection_AllCookiesDeleted; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

class CheckProcessLocalPortForActivity {
public:
    using Arguments = std::tuple<const WebCore::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessConnection_CheckProcessLocalPortForActivity; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(WebCore::MessagePortChannelProvider::HasActivity&&)>&&);
    static void cancelReply(CompletionHandler<void(WebCore::MessagePortChannelProvider::HasActivity&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcessConnection_CheckProcessLocalPortForActivityReply; }
    using AsyncReply = CheckProcessLocalPortForActivityAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const WebCore::MessagePortChannelProvider::HasActivity& hasActivity);
    using Reply = std::tuple<WebCore::MessagePortChannelProvider::HasActivity&>;
    using ReplyArguments = std::tuple<WebCore::MessagePortChannelProvider::HasActivity>;
    explicit CheckProcessLocalPortForActivity(const WebCore::MessagePortIdentifier& port)
        : m_arguments(port)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class MessagesAvailableForPort {
public:
    using Arguments = std::tuple<const WebCore::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessConnection_MessagesAvailableForPort; }
    static const bool isSync = false;

    explicit MessagesAvailableForPort(const WebCore::MessagePortIdentifier& port)
        : m_arguments(port)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class BroadcastConsoleMessage {
public:
    using Arguments = std::tuple<JSC::MessageSource, JSC::MessageLevel, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessConnection_BroadcastConsoleMessage; }
    static const bool isSync = false;

    BroadcastConsoleMessage(JSC::MessageSource messageSource, JSC::MessageLevel messageLevel, const String& message)
        : m_arguments(messageSource, messageLevel, message)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

} // namespace NetworkProcessConnection
} // namespace Messages
