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

#if ENABLE(SERVICE_WORKER)

#include "ArgumentCoders.h"
#include "Connection.h"
#include "MessageNames.h"
#include "WebSWServerToContextConnectionMessagesReplies.h"
#include <WebCore/ExceptionData.h>
#include <WebCore/ServiceWorkerIdentifier.h>
#include <WebCore/ServiceWorkerJobDataIdentifier.h>
#include <wtf/Forward.h>
#include <wtf/Optional.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/text/WTFString.h>

namespace WebCore {
struct MessageWithMessagePorts;
struct ServiceWorkerClientIdentifier;
struct ServiceWorkerClientQueryOptions;
}

namespace Messages {
namespace WebSWServerToContextConnection {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::WebSWServerToContextConnection;
}

class ScriptContextFailedToStart {
public:
    using Arguments = std::tuple<const Optional<WebCore::ServiceWorkerJobDataIdentifier>&, const WebCore::ServiceWorkerIdentifier&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerToContextConnection_ScriptContextFailedToStart; }
    static const bool isSync = false;

    ScriptContextFailedToStart(const Optional<WebCore::ServiceWorkerJobDataIdentifier>& jobDataIdentifier, const WebCore::ServiceWorkerIdentifier& serviceWorkerIdentifier, const String& message)
        : m_arguments(jobDataIdentifier, serviceWorkerIdentifier, message)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ScriptContextStarted {
public:
    using Arguments = std::tuple<const Optional<WebCore::ServiceWorkerJobDataIdentifier>&, const WebCore::ServiceWorkerIdentifier&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerToContextConnection_ScriptContextStarted; }
    static const bool isSync = false;

    ScriptContextStarted(const Optional<WebCore::ServiceWorkerJobDataIdentifier>& jobDataIdentifier, const WebCore::ServiceWorkerIdentifier& serviceWorkerIdentifier, bool doesHandleFetch)
        : m_arguments(jobDataIdentifier, serviceWorkerIdentifier, doesHandleFetch)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidFinishInstall {
public:
    using Arguments = std::tuple<const Optional<WebCore::ServiceWorkerJobDataIdentifier>&, const WebCore::ServiceWorkerIdentifier&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerToContextConnection_DidFinishInstall; }
    static const bool isSync = false;

    DidFinishInstall(const Optional<WebCore::ServiceWorkerJobDataIdentifier>& jobDataIdentifier, const WebCore::ServiceWorkerIdentifier& serviceWorkerIdentifier, bool wasSuccessful)
        : m_arguments(jobDataIdentifier, serviceWorkerIdentifier, wasSuccessful)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidFinishActivation {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerToContextConnection_DidFinishActivation; }
    static const bool isSync = false;

    explicit DidFinishActivation(const WebCore::ServiceWorkerIdentifier& identifier)
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

class SetServiceWorkerHasPendingEvents {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerIdentifier&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerToContextConnection_SetServiceWorkerHasPendingEvents; }
    static const bool isSync = false;

    SetServiceWorkerHasPendingEvents(const WebCore::ServiceWorkerIdentifier& identifier, bool hasPendingEvents)
        : m_arguments(identifier, hasPendingEvents)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SkipWaiting {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerToContextConnection_SkipWaiting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebSWServerToContextConnection_SkipWaitingReply; }
    using AsyncReply = SkipWaitingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit SkipWaiting(const WebCore::ServiceWorkerIdentifier& identifier)
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

class WorkerTerminated {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerToContextConnection_WorkerTerminated; }
    static const bool isSync = false;

    explicit WorkerTerminated(const WebCore::ServiceWorkerIdentifier& identifier)
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

class FindClientByIdentifier {
public:
    using Arguments = std::tuple<uint64_t, const WebCore::ServiceWorkerIdentifier&, const WebCore::ServiceWorkerClientIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerToContextConnection_FindClientByIdentifier; }
    static const bool isSync = false;

    FindClientByIdentifier(uint64_t requestIdentifier, const WebCore::ServiceWorkerIdentifier& serviceWorkerIdentifier, const WebCore::ServiceWorkerClientIdentifier& clientIdentifier)
        : m_arguments(requestIdentifier, serviceWorkerIdentifier, clientIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class MatchAll {
public:
    using Arguments = std::tuple<uint64_t, const WebCore::ServiceWorkerIdentifier&, const WebCore::ServiceWorkerClientQueryOptions&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerToContextConnection_MatchAll; }
    static const bool isSync = false;

    MatchAll(uint64_t matchAllRequestIdentifier, const WebCore::ServiceWorkerIdentifier& serviceWorkerIdentifier, const WebCore::ServiceWorkerClientQueryOptions& options)
        : m_arguments(matchAllRequestIdentifier, serviceWorkerIdentifier, options)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class Claim {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerToContextConnection_Claim; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(Optional<WebCore::ExceptionData>&&)>&&);
    static void cancelReply(CompletionHandler<void(Optional<WebCore::ExceptionData>&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebSWServerToContextConnection_ClaimReply; }
    using AsyncReply = ClaimAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Optional<WebCore::ExceptionData>& result);
    using Reply = std::tuple<Optional<WebCore::ExceptionData>&>;
    using ReplyArguments = std::tuple<Optional<WebCore::ExceptionData>>;
    explicit Claim(const WebCore::ServiceWorkerIdentifier& serviceWorkerIdentifier)
        : m_arguments(serviceWorkerIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetScriptResource {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerIdentifier&, const URL&, const String&, const URL&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerToContextConnection_SetScriptResource; }
    static const bool isSync = false;

    SetScriptResource(const WebCore::ServiceWorkerIdentifier& identifier, const URL& scriptURL, const String& script, const URL& responseURL, const String& mimeType)
        : m_arguments(identifier, scriptURL, script, responseURL, mimeType)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class PostMessageToServiceWorkerClient {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerClientIdentifier&, const WebCore::MessageWithMessagePorts&, const WebCore::ServiceWorkerIdentifier&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerToContextConnection_PostMessageToServiceWorkerClient; }
    static const bool isSync = false;

    PostMessageToServiceWorkerClient(const WebCore::ServiceWorkerClientIdentifier& destination, const WebCore::MessageWithMessagePorts& message, const WebCore::ServiceWorkerIdentifier& source, const String& sourceOrigin)
        : m_arguments(destination, message, source, sourceOrigin)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidFailHeartBeatCheck {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerToContextConnection_DidFailHeartBeatCheck; }
    static const bool isSync = false;

    explicit DidFailHeartBeatCheck(const WebCore::ServiceWorkerIdentifier& identifier)
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

} // namespace WebSWServerToContextConnection
} // namespace Messages

#endif // ENABLE(SERVICE_WORKER)
