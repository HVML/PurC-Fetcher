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
#include "StorageAreaImplIdentifier.h"
#include "StorageAreaMapMessagesReplies.h"
#include <wtf/Forward.h>
#include <wtf/Optional.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/text/WTFString.h>


namespace Messages {
namespace StorageAreaMap {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::StorageAreaMap;
}

class DidSetItem {
public:
    using Arguments = std::tuple<uint64_t, const String&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::StorageAreaMap_DidSetItem; }
    static const bool isSync = false;

    DidSetItem(uint64_t storageMapSeed, const String& key, bool quotaException)
        : m_arguments(storageMapSeed, key, quotaException)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidRemoveItem {
public:
    using Arguments = std::tuple<uint64_t, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::StorageAreaMap_DidRemoveItem; }
    static const bool isSync = false;

    DidRemoveItem(uint64_t storageMapSeed, const String& key)
        : m_arguments(storageMapSeed, key)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidClear {
public:
    using Arguments = std::tuple<uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::StorageAreaMap_DidClear; }
    static const bool isSync = false;

    explicit DidClear(uint64_t storageMapSeed)
        : m_arguments(storageMapSeed)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DispatchStorageEvent {
public:
    using Arguments = std::tuple<const Optional<WebKit::StorageAreaImplIdentifier>&, const String&, const String&, const String&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::StorageAreaMap_DispatchStorageEvent; }
    static const bool isSync = false;

    DispatchStorageEvent(const Optional<WebKit::StorageAreaImplIdentifier>& storageAreaImplID, const String& key, const String& oldValue, const String& newValue, const String& urlString)
        : m_arguments(storageAreaImplID, key, oldValue, newValue, urlString)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ClearCache {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::StorageAreaMap_ClearCache; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

} // namespace StorageAreaMap
} // namespace Messages
