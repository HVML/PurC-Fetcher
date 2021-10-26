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
#include "WebCookieManagerMessagesReplies.h"
#include "Cookie.h"
#include <wtf/Forward.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace PAL {
class SessionID;
}

namespace WebCore {
enum class HTTPCookieAcceptPolicy : uint8_t;
struct Cookie;
}

namespace WebKit {
enum class SoupCookiePersistentStorageType : bool;
}

namespace Messages {
namespace WebCookieManager {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::WebCookieManager;
}

class GetHostnamesWithCookies {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::WebCookieManager_GetHostnamesWithCookies; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(Vector<String>&&)>&&);
    static void cancelReply(CompletionHandler<void(Vector<String>&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebCookieManager_GetHostnamesWithCookiesReply; }
    using AsyncReply = GetHostnamesWithCookiesAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Vector<String>& hostnames);
    using Reply = std::tuple<Vector<String>&>;
    using ReplyArguments = std::tuple<Vector<String>>;
    explicit GetHostnamesWithCookies(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DeleteCookiesForHostnames {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const Vector<String>&>;

    static IPC::MessageName name() { return IPC::MessageName::WebCookieManager_DeleteCookiesForHostnames; }
    static const bool isSync = false;

    DeleteCookiesForHostnames(const PAL::SessionID& sessionID, const Vector<String>& hostnames)
        : m_arguments(sessionID, hostnames)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DeleteAllCookies {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::WebCookieManager_DeleteAllCookies; }
    static const bool isSync = false;

    explicit DeleteAllCookies(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetCookie {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const Vector<WebCore::Cookie>&>;

    static IPC::MessageName name() { return IPC::MessageName::WebCookieManager_SetCookie; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebCookieManager_SetCookieReply; }
    using AsyncReply = SetCookieAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetCookie(const PAL::SessionID& sessionID, const Vector<WebCore::Cookie>& cookie)
        : m_arguments(sessionID, cookie)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetCookies {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const Vector<WebCore::Cookie>&, const URL&, const URL&>;

    static IPC::MessageName name() { return IPC::MessageName::WebCookieManager_SetCookies; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebCookieManager_SetCookiesReply; }
    using AsyncReply = SetCookiesAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetCookies(const PAL::SessionID& sessionID, const Vector<WebCore::Cookie>& cookies, const URL& url, const URL& mainDocumentURL)
        : m_arguments(sessionID, cookies, url, mainDocumentURL)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class GetAllCookies {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::WebCookieManager_GetAllCookies; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(Vector<WebCore::Cookie>&&)>&&);
    static void cancelReply(CompletionHandler<void(Vector<WebCore::Cookie>&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebCookieManager_GetAllCookiesReply; }
    using AsyncReply = GetAllCookiesAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Vector<WebCore::Cookie>& cookies);
    using Reply = std::tuple<Vector<WebCore::Cookie>&>;
    using ReplyArguments = std::tuple<Vector<WebCore::Cookie>>;
    explicit GetAllCookies(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class GetCookies {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const URL&>;

    static IPC::MessageName name() { return IPC::MessageName::WebCookieManager_GetCookies; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(Vector<WebCore::Cookie>&&)>&&);
    static void cancelReply(CompletionHandler<void(Vector<WebCore::Cookie>&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebCookieManager_GetCookiesReply; }
    using AsyncReply = GetCookiesAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Vector<WebCore::Cookie>& cookies);
    using Reply = std::tuple<Vector<WebCore::Cookie>&>;
    using ReplyArguments = std::tuple<Vector<WebCore::Cookie>>;
    GetCookies(const PAL::SessionID& sessionID, const URL& url)
        : m_arguments(sessionID, url)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DeleteCookie {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const WebCore::Cookie&>;

    static IPC::MessageName name() { return IPC::MessageName::WebCookieManager_DeleteCookie; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebCookieManager_DeleteCookieReply; }
    using AsyncReply = DeleteCookieAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    DeleteCookie(const PAL::SessionID& sessionID, const WebCore::Cookie& cookie)
        : m_arguments(sessionID, cookie)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DeleteAllCookiesModifiedSince {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const WallTime&>;

    static IPC::MessageName name() { return IPC::MessageName::WebCookieManager_DeleteAllCookiesModifiedSince; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebCookieManager_DeleteAllCookiesModifiedSinceReply; }
    using AsyncReply = DeleteAllCookiesModifiedSinceAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    DeleteAllCookiesModifiedSince(const PAL::SessionID& sessionID, const WallTime& time)
        : m_arguments(sessionID, time)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetHTTPCookieAcceptPolicy {
public:
    using Arguments = std::tuple<WebCore::HTTPCookieAcceptPolicy>;

    static IPC::MessageName name() { return IPC::MessageName::WebCookieManager_SetHTTPCookieAcceptPolicy; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebCookieManager_SetHTTPCookieAcceptPolicyReply; }
    using AsyncReply = SetHTTPCookieAcceptPolicyAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit SetHTTPCookieAcceptPolicy(WebCore::HTTPCookieAcceptPolicy policy)
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

class GetHTTPCookieAcceptPolicy {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebCookieManager_GetHTTPCookieAcceptPolicy; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(WebCore::HTTPCookieAcceptPolicy&&)>&&);
    static void cancelReply(CompletionHandler<void(WebCore::HTTPCookieAcceptPolicy&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebCookieManager_GetHTTPCookieAcceptPolicyReply; }
    using AsyncReply = GetHTTPCookieAcceptPolicyAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, WebCore::HTTPCookieAcceptPolicy policy);
    using Reply = std::tuple<WebCore::HTTPCookieAcceptPolicy&>;
    using ReplyArguments = std::tuple<WebCore::HTTPCookieAcceptPolicy>;
    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class StartObservingCookieChanges {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::WebCookieManager_StartObservingCookieChanges; }
    static const bool isSync = false;

    explicit StartObservingCookieChanges(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class StopObservingCookieChanges {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::WebCookieManager_StopObservingCookieChanges; }
    static const bool isSync = false;

    explicit StopObservingCookieChanges(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if USE(SOUP)
class SetCookiePersistentStorage {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const String&, WebKit::SoupCookiePersistentStorageType>;

    static IPC::MessageName name() { return IPC::MessageName::WebCookieManager_SetCookiePersistentStorage; }
    static const bool isSync = false;

    SetCookiePersistentStorage(const PAL::SessionID& sessionID, const String& storagePath, WebKit::SoupCookiePersistentStorageType storageType)
        : m_arguments(sessionID, storagePath, storageType)
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

} // namespace WebCookieManager
} // namespace Messages
