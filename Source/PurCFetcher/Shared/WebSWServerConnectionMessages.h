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
#include "WebSWServerConnectionMessagesReplies.h"
#include <WebCore/ExceptionData.h>
#include <WebCore/ServiceWorkerClientIdentifier.h>
#include <WebCore/ServiceWorkerIdentifier.h>
#include <WebCore/ServiceWorkerTypes.h>
#include <wtf/Expected.h>
#include <wtf/Forward.h>
#include <wtf/Optional.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/text/WTFString.h>

namespace WebCore {
class ServiceWorkerRegistrationKey;
struct MessageWithMessagePorts;
struct SecurityOriginData;
struct ServiceWorkerClientData;
struct ServiceWorkerClientIdentifier;
struct ServiceWorkerFetchResult;
struct ServiceWorkerJobData;
}

namespace Messages {
namespace WebSWServerConnection {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::WebSWServerConnection;
}

class ScheduleJobInServer {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerJobData&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_ScheduleJobInServer; }
    static const bool isSync = false;

    explicit ScheduleJobInServer(const WebCore::ServiceWorkerJobData& jobData)
        : m_arguments(jobData)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ScheduleUnregisterJobInServer {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerJobIdentifier&, const WebCore::ServiceWorkerRegistrationIdentifier&, const WebCore::DocumentOrWorkerIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_ScheduleUnregisterJobInServer; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(Expected<bool, WebCore::ExceptionData>&&)>&&);
    static void cancelReply(CompletionHandler<void(Expected<bool, WebCore::ExceptionData>&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebSWServerConnection_ScheduleUnregisterJobInServerReply; }
    using AsyncReply = ScheduleUnregisterJobInServerAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Expected<bool, WebCore::ExceptionData>& result);
    using Reply = std::tuple<Expected<bool, WebCore::ExceptionData>&>;
    using ReplyArguments = std::tuple<Expected<bool, WebCore::ExceptionData>>;
    ScheduleUnregisterJobInServer(const WebCore::ServiceWorkerJobIdentifier& jobIdentifier, const WebCore::ServiceWorkerRegistrationIdentifier& identifier, const WebCore::DocumentOrWorkerIdentifier& documentIdentifier)
        : m_arguments(jobIdentifier, identifier, documentIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class FinishFetchingScriptInServer {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerFetchResult&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_FinishFetchingScriptInServer; }
    static const bool isSync = false;

    explicit FinishFetchingScriptInServer(const WebCore::ServiceWorkerFetchResult& result)
        : m_arguments(result)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class AddServiceWorkerRegistrationInServer {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerRegistrationIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_AddServiceWorkerRegistrationInServer; }
    static const bool isSync = false;

    explicit AddServiceWorkerRegistrationInServer(const WebCore::ServiceWorkerRegistrationIdentifier& identifier)
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

class RemoveServiceWorkerRegistrationInServer {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerRegistrationIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_RemoveServiceWorkerRegistrationInServer; }
    static const bool isSync = false;

    explicit RemoveServiceWorkerRegistrationInServer(const WebCore::ServiceWorkerRegistrationIdentifier& identifier)
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

class PostMessageToServiceWorker {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerIdentifier&, const WebCore::MessageWithMessagePorts&, const WebCore::ServiceWorkerOrClientIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_PostMessageToServiceWorker; }
    static const bool isSync = false;

    PostMessageToServiceWorker(const WebCore::ServiceWorkerIdentifier& destination, const WebCore::MessageWithMessagePorts& message, const WebCore::ServiceWorkerOrClientIdentifier& source)
        : m_arguments(destination, message, source)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidResolveRegistrationPromise {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerRegistrationKey&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_DidResolveRegistrationPromise; }
    static const bool isSync = false;

    explicit DidResolveRegistrationPromise(const WebCore::ServiceWorkerRegistrationKey& key)
        : m_arguments(key)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class MatchRegistration {
public:
    using Arguments = std::tuple<uint64_t, const WebCore::SecurityOriginData&, const URL&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_MatchRegistration; }
    static const bool isSync = false;

    MatchRegistration(uint64_t serviceRegistrationMatchRequestIdentifier, const WebCore::SecurityOriginData& topOrigin, const URL& clientURL)
        : m_arguments(serviceRegistrationMatchRequestIdentifier, topOrigin, clientURL)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class WhenRegistrationReady {
public:
    using Arguments = std::tuple<uint64_t, const WebCore::SecurityOriginData&, const URL&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_WhenRegistrationReady; }
    static const bool isSync = false;

    WhenRegistrationReady(uint64_t serviceRegistrationMatchRequestIdentifier, const WebCore::SecurityOriginData& topOrigin, const URL& clientURL)
        : m_arguments(serviceRegistrationMatchRequestIdentifier, topOrigin, clientURL)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class GetRegistrations {
public:
    using Arguments = std::tuple<uint64_t, const WebCore::SecurityOriginData&, const URL&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_GetRegistrations; }
    static const bool isSync = false;

    GetRegistrations(uint64_t serviceRegistrationMatchRequestIdentifier, const WebCore::SecurityOriginData& topOrigin, const URL& clientURL)
        : m_arguments(serviceRegistrationMatchRequestIdentifier, topOrigin, clientURL)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterServiceWorkerClient {
public:
    using Arguments = std::tuple<const WebCore::SecurityOriginData&, const WebCore::ServiceWorkerClientData&, const Optional<WebCore::ServiceWorkerRegistrationIdentifier>&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_RegisterServiceWorkerClient; }
    static const bool isSync = false;

    RegisterServiceWorkerClient(const WebCore::SecurityOriginData& topOrigin, const WebCore::ServiceWorkerClientData& data, const Optional<WebCore::ServiceWorkerRegistrationIdentifier>& controllingServiceWorkerRegistrationIdentifier, const String& userAgent)
        : m_arguments(topOrigin, data, controllingServiceWorkerRegistrationIdentifier, userAgent)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class UnregisterServiceWorkerClient {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerClientIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_UnregisterServiceWorkerClient; }
    static const bool isSync = false;

    explicit UnregisterServiceWorkerClient(const WebCore::ServiceWorkerClientIdentifier& identifier)
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

class TerminateWorkerFromClient {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_TerminateWorkerFromClient; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebSWServerConnection_TerminateWorkerFromClientReply; }
    using AsyncReply = TerminateWorkerFromClientAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit TerminateWorkerFromClient(const WebCore::ServiceWorkerIdentifier& workerIdentifier)
        : m_arguments(workerIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class WhenServiceWorkerIsTerminatedForTesting {
public:
    using Arguments = std::tuple<const WebCore::ServiceWorkerIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_WhenServiceWorkerIsTerminatedForTesting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebSWServerConnection_WhenServiceWorkerIsTerminatedForTestingReply; }
    using AsyncReply = WhenServiceWorkerIsTerminatedForTestingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit WhenServiceWorkerIsTerminatedForTesting(const WebCore::ServiceWorkerIdentifier& workerIdentifier)
        : m_arguments(workerIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetThrottleState {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_SetThrottleState; }
    static const bool isSync = false;

    explicit SetThrottleState(bool isThrottleable)
        : m_arguments(isThrottleable)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class StoreRegistrationsOnDisk {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebSWServerConnection_StoreRegistrationsOnDisk; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebSWServerConnection_StoreRegistrationsOnDiskReply; }
    using AsyncReply = StoreRegistrationsOnDiskAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

} // namespace WebSWServerConnection
} // namespace Messages

#endif // ENABLE(SERVICE_WORKER)
