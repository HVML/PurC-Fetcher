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
#include "StorageAreaIdentifier.h"
#include "StorageAreaImplIdentifier.h"
#include "StorageManagerSetMessagesReplies.h"
#include "StorageNamespaceIdentifier.h"
#include <wtf/Forward.h>
#include <wtf/HashMap.h>
#include <wtf/Optional.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/text/WTFString.h>

namespace PAL {
class SessionID;
}

namespace WebCore {
struct SecurityOriginData;
}

namespace Messages {
namespace StorageManagerSet {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::StorageManagerSet;
}

class ConnectToLocalStorageArea {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const WebKit::StorageNamespaceIdentifier&, const WebCore::SecurityOriginData&>;

    static IPC::MessageName name() { return IPC::MessageName::StorageManagerSet_ConnectToLocalStorageArea; }
    static const bool isSync = true;

    using DelayedReply = ConnectToLocalStorageAreaDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Optional<WebKit::StorageAreaIdentifier>& storageAreaID);
    using Reply = std::tuple<Optional<WebKit::StorageAreaIdentifier>&>;
    using ReplyArguments = std::tuple<Optional<WebKit::StorageAreaIdentifier>>;
    ConnectToLocalStorageArea(const PAL::SessionID& sessionID, const WebKit::StorageNamespaceIdentifier& storageNamespaceID, const WebCore::SecurityOriginData& securityOriginData)
        : m_arguments(sessionID, storageNamespaceID, securityOriginData)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ConnectToTransientLocalStorageArea {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const WebKit::StorageNamespaceIdentifier&, const WebCore::SecurityOriginData&, const WebCore::SecurityOriginData&>;

    static IPC::MessageName name() { return IPC::MessageName::StorageManagerSet_ConnectToTransientLocalStorageArea; }
    static const bool isSync = true;

    using DelayedReply = ConnectToTransientLocalStorageAreaDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Optional<WebKit::StorageAreaIdentifier>& storageAreaID);
    using Reply = std::tuple<Optional<WebKit::StorageAreaIdentifier>&>;
    using ReplyArguments = std::tuple<Optional<WebKit::StorageAreaIdentifier>>;
    ConnectToTransientLocalStorageArea(const PAL::SessionID& sessionID, const WebKit::StorageNamespaceIdentifier& storageNamespaceID, const WebCore::SecurityOriginData& topLevelSecurityOriginData, const WebCore::SecurityOriginData& securityOriginData)
        : m_arguments(sessionID, storageNamespaceID, topLevelSecurityOriginData, securityOriginData)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ConnectToSessionStorageArea {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const WebKit::StorageNamespaceIdentifier&, const WebCore::SecurityOriginData&>;

    static IPC::MessageName name() { return IPC::MessageName::StorageManagerSet_ConnectToSessionStorageArea; }
    static const bool isSync = true;

    using DelayedReply = ConnectToSessionStorageAreaDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Optional<WebKit::StorageAreaIdentifier>& storageAreaID);
    using Reply = std::tuple<Optional<WebKit::StorageAreaIdentifier>&>;
    using ReplyArguments = std::tuple<Optional<WebKit::StorageAreaIdentifier>>;
    ConnectToSessionStorageArea(const PAL::SessionID& sessionID, const WebKit::StorageNamespaceIdentifier& storageNamespaceID, const WebCore::SecurityOriginData& securityOriginData)
        : m_arguments(sessionID, storageNamespaceID, securityOriginData)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DisconnectFromStorageArea {
public:
    using Arguments = std::tuple<const WebKit::StorageAreaIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::StorageManagerSet_DisconnectFromStorageArea; }
    static const bool isSync = false;

    explicit DisconnectFromStorageArea(const WebKit::StorageAreaIdentifier& storageAreaID)
        : m_arguments(storageAreaID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class GetValues {
public:
    using Arguments = std::tuple<const WebKit::StorageAreaIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::StorageManagerSet_GetValues; }
    static const bool isSync = true;

    using DelayedReply = GetValuesDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const HashMap<String, String>& values);
    using Reply = std::tuple<HashMap<String, String>&>;
    using ReplyArguments = std::tuple<HashMap<String, String>>;
    explicit GetValues(const WebKit::StorageAreaIdentifier& storageAreaID)
        : m_arguments(storageAreaID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class CloneSessionStorageNamespace {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const WebKit::StorageNamespaceIdentifier&, const WebKit::StorageNamespaceIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::StorageManagerSet_CloneSessionStorageNamespace; }
    static const bool isSync = false;

    CloneSessionStorageNamespace(const PAL::SessionID& sessionID, const WebKit::StorageNamespaceIdentifier& fromStorageNamespaceID, const WebKit::StorageNamespaceIdentifier& toStorageNamespaceID)
        : m_arguments(sessionID, fromStorageNamespaceID, toStorageNamespaceID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetItem {
public:
    using Arguments = std::tuple<const WebKit::StorageAreaIdentifier&, const WebKit::StorageAreaImplIdentifier&, uint64_t, const String&, const String&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::StorageManagerSet_SetItem; }
    static const bool isSync = false;

    SetItem(const WebKit::StorageAreaIdentifier& storageAreaID, const WebKit::StorageAreaImplIdentifier& storageAreaImplID, uint64_t storageMapSeed, const String& key, const String& value, const String& urlString)
        : m_arguments(storageAreaID, storageAreaImplID, storageMapSeed, key, value, urlString)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RemoveItem {
public:
    using Arguments = std::tuple<const WebKit::StorageAreaIdentifier&, const WebKit::StorageAreaImplIdentifier&, uint64_t, const String&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::StorageManagerSet_RemoveItem; }
    static const bool isSync = false;

    RemoveItem(const WebKit::StorageAreaIdentifier& storageAreaID, const WebKit::StorageAreaImplIdentifier& storageAreaImplID, uint64_t storageMapSeed, const String& key, const String& urlString)
        : m_arguments(storageAreaID, storageAreaImplID, storageMapSeed, key, urlString)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class Clear {
public:
    using Arguments = std::tuple<const WebKit::StorageAreaIdentifier&, const WebKit::StorageAreaImplIdentifier&, uint64_t, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::StorageManagerSet_Clear; }
    static const bool isSync = false;

    Clear(const WebKit::StorageAreaIdentifier& storageAreaID, const WebKit::StorageAreaImplIdentifier& storageAreaImplID, uint64_t storageMapSeed, const String& urlString)
        : m_arguments(storageAreaID, storageAreaImplID, storageMapSeed, urlString)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

} // namespace StorageManagerSet
} // namespace Messages
