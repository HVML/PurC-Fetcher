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
#include "CacheStorageEngineConnectionMessagesReplies.h"
#include "Connection.h"
#include "MessageNames.h"
#include "DOMCacheEngine.h"
#include <wtf/Forward.h>
#include <wtf/Optional.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace WebCore {
class ResourceRequest;
struct CacheQueryOptions;
struct ClientOrigin;
struct RetrieveRecordsOptions;
}

namespace Messages {
namespace CacheStorageEngineConnection {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::CacheStorageEngineConnection;
}

class Reference {
public:
    using Arguments = std::tuple<uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::CacheStorageEngineConnection_Reference; }
    static const bool isSync = false;

    explicit Reference(uint64_t cacheIdentifier)
        : m_arguments(cacheIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class Dereference {
public:
    using Arguments = std::tuple<uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::CacheStorageEngineConnection_Dereference; }
    static const bool isSync = false;

    explicit Dereference(uint64_t cacheIdentifier)
        : m_arguments(cacheIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class Open {
public:
    using Arguments = std::tuple<const WebCore::ClientOrigin&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::CacheStorageEngineConnection_Open; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(WebCore::DOMCacheEngine::CacheIdentifierOrError&&)>&&);
    static void cancelReply(CompletionHandler<void(WebCore::DOMCacheEngine::CacheIdentifierOrError&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::CacheStorageEngineConnection_OpenReply; }
    using AsyncReply = OpenAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const WebCore::DOMCacheEngine::CacheIdentifierOrError& result);
    using Reply = std::tuple<WebCore::DOMCacheEngine::CacheIdentifierOrError&>;
    using ReplyArguments = std::tuple<WebCore::DOMCacheEngine::CacheIdentifierOrError>;
    Open(const WebCore::ClientOrigin& origin, const String& cacheName)
        : m_arguments(origin, cacheName)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class Remove {
public:
    using Arguments = std::tuple<uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::CacheStorageEngineConnection_Remove; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(WebCore::DOMCacheEngine::CacheIdentifierOrError&&)>&&);
    static void cancelReply(CompletionHandler<void(WebCore::DOMCacheEngine::CacheIdentifierOrError&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::CacheStorageEngineConnection_RemoveReply; }
    using AsyncReply = RemoveAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const WebCore::DOMCacheEngine::CacheIdentifierOrError& result);
    using Reply = std::tuple<WebCore::DOMCacheEngine::CacheIdentifierOrError&>;
    using ReplyArguments = std::tuple<WebCore::DOMCacheEngine::CacheIdentifierOrError>;
    explicit Remove(uint64_t cacheIdentifier)
        : m_arguments(cacheIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class Caches {
public:
    using Arguments = std::tuple<const WebCore::ClientOrigin&, uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::CacheStorageEngineConnection_Caches; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(WebCore::DOMCacheEngine::CacheInfosOrError&&)>&&);
    static void cancelReply(CompletionHandler<void(WebCore::DOMCacheEngine::CacheInfosOrError&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::CacheStorageEngineConnection_CachesReply; }
    using AsyncReply = CachesAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const WebCore::DOMCacheEngine::CacheInfosOrError& result);
    using Reply = std::tuple<WebCore::DOMCacheEngine::CacheInfosOrError&>;
    using ReplyArguments = std::tuple<WebCore::DOMCacheEngine::CacheInfosOrError>;
    Caches(const WebCore::ClientOrigin& origin, uint64_t updateCounter)
        : m_arguments(origin, updateCounter)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RetrieveRecords {
public:
    using Arguments = std::tuple<uint64_t, const WebCore::RetrieveRecordsOptions&>;

    static IPC::MessageName name() { return IPC::MessageName::CacheStorageEngineConnection_RetrieveRecords; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(WebCore::DOMCacheEngine::RecordsOrError&&)>&&);
    static void cancelReply(CompletionHandler<void(WebCore::DOMCacheEngine::RecordsOrError&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::CacheStorageEngineConnection_RetrieveRecordsReply; }
    using AsyncReply = RetrieveRecordsAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const WebCore::DOMCacheEngine::RecordsOrError& result);
    using Reply = std::tuple<WebCore::DOMCacheEngine::RecordsOrError&>;
    using ReplyArguments = std::tuple<WebCore::DOMCacheEngine::RecordsOrError>;
    RetrieveRecords(uint64_t cacheIdentifier, const WebCore::RetrieveRecordsOptions& options)
        : m_arguments(cacheIdentifier, options)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DeleteMatchingRecords {
public:
    using Arguments = std::tuple<uint64_t, const WebCore::ResourceRequest&, const WebCore::CacheQueryOptions&>;

    static IPC::MessageName name() { return IPC::MessageName::CacheStorageEngineConnection_DeleteMatchingRecords; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(WebCore::DOMCacheEngine::RecordIdentifiersOrError&&)>&&);
    static void cancelReply(CompletionHandler<void(WebCore::DOMCacheEngine::RecordIdentifiersOrError&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::CacheStorageEngineConnection_DeleteMatchingRecordsReply; }
    using AsyncReply = DeleteMatchingRecordsAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const WebCore::DOMCacheEngine::RecordIdentifiersOrError& result);
    using Reply = std::tuple<WebCore::DOMCacheEngine::RecordIdentifiersOrError&>;
    using ReplyArguments = std::tuple<WebCore::DOMCacheEngine::RecordIdentifiersOrError>;
    DeleteMatchingRecords(uint64_t cacheIdentifier, const WebCore::ResourceRequest& request, const WebCore::CacheQueryOptions& options)
        : m_arguments(cacheIdentifier, request, options)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class PutRecords {
public:
    using Arguments = std::tuple<uint64_t, const Vector<WebCore::DOMCacheEngine::Record>&>;

    static IPC::MessageName name() { return IPC::MessageName::CacheStorageEngineConnection_PutRecords; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(WebCore::DOMCacheEngine::RecordIdentifiersOrError&&)>&&);
    static void cancelReply(CompletionHandler<void(WebCore::DOMCacheEngine::RecordIdentifiersOrError&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::CacheStorageEngineConnection_PutRecordsReply; }
    using AsyncReply = PutRecordsAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const WebCore::DOMCacheEngine::RecordIdentifiersOrError& result);
    using Reply = std::tuple<WebCore::DOMCacheEngine::RecordIdentifiersOrError&>;
    using ReplyArguments = std::tuple<WebCore::DOMCacheEngine::RecordIdentifiersOrError>;
    PutRecords(uint64_t cacheIdentifier, const Vector<WebCore::DOMCacheEngine::Record>& record)
        : m_arguments(cacheIdentifier, record)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ClearMemoryRepresentation {
public:
    using Arguments = std::tuple<const WebCore::ClientOrigin&>;

    static IPC::MessageName name() { return IPC::MessageName::CacheStorageEngineConnection_ClearMemoryRepresentation; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(Optional<WebCore::DOMCacheEngine::Error>&&)>&&);
    static void cancelReply(CompletionHandler<void(Optional<WebCore::DOMCacheEngine::Error>&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::CacheStorageEngineConnection_ClearMemoryRepresentationReply; }
    using AsyncReply = ClearMemoryRepresentationAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Optional<WebCore::DOMCacheEngine::Error>& error);
    using Reply = std::tuple<Optional<WebCore::DOMCacheEngine::Error>&>;
    using ReplyArguments = std::tuple<Optional<WebCore::DOMCacheEngine::Error>>;
    explicit ClearMemoryRepresentation(const WebCore::ClientOrigin& origin)
        : m_arguments(origin)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class EngineRepresentation {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::CacheStorageEngineConnection_EngineRepresentation; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(String&&)>&&);
    static void cancelReply(CompletionHandler<void(String&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::CacheStorageEngineConnection_EngineRepresentationReply; }
    using AsyncReply = EngineRepresentationAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const String& representation);
    using Reply = std::tuple<String&>;
    using ReplyArguments = std::tuple<String>;
    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

} // namespace CacheStorageEngineConnection
} // namespace Messages
