/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "ArgumentCoders.h"
#include "CacheStorageEngine.h"
#include "Connection.h"
#include "CacheStorageConnection.h"
#include "SessionID.h"
#include <wtf/Forward.h>
#include <wtf/RefCounted.h>

namespace IPC {

template<> struct AsyncReplyError<PurcFetcher::DOMCacheEngine::CacheIdentifierOrError> {
    static PurcFetcher::DOMCacheEngine::CacheIdentifierOrError create() { return makeUnexpected(PurcFetcher::DOMCacheEngine::Error::Internal); };
};
template<> struct AsyncReplyError<PurcFetcher::DOMCacheEngine::RecordIdentifiersOrError> {
    static PurcFetcher::DOMCacheEngine::RecordIdentifiersOrError create() { return makeUnexpected(PurcFetcher::DOMCacheEngine::Error::Internal); };
};
template<> struct AsyncReplyError<PurcFetcher::DOMCacheEngine::CacheInfosOrError> {
    static PurcFetcher::DOMCacheEngine::CacheInfosOrError create() { return makeUnexpected(PurcFetcher::DOMCacheEngine::Error::Internal); };
};
template<> struct AsyncReplyError<PurcFetcher::DOMCacheEngine::RecordsOrError> {
    static PurcFetcher::DOMCacheEngine::RecordsOrError create() { return makeUnexpected(PurcFetcher::DOMCacheEngine::Error::Internal); };
};

}

namespace PurcFetcher {

class NetworkConnectionToWebProcess;

class CacheStorageEngineConnection : public RefCounted<CacheStorageEngineConnection> {
public:
    static Ref<CacheStorageEngineConnection> create(NetworkConnectionToWebProcess& connection) { return adoptRef(*new CacheStorageEngineConnection(connection)); }
    ~CacheStorageEngineConnection();
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&);

private:
    explicit CacheStorageEngineConnection(NetworkConnectionToWebProcess&);

    void open(PurcFetcher::ClientOrigin&&, String&& cacheName, PurcFetcher::DOMCacheEngine::CacheIdentifierCallback&&);
    void remove(uint64_t cacheIdentifier, PurcFetcher::DOMCacheEngine::CacheIdentifierCallback&&);
    void caches(PurcFetcher::ClientOrigin&&, uint64_t updateCounter, PurcFetcher::DOMCacheEngine::CacheInfosCallback&&);

    void retrieveRecords(uint64_t cacheIdentifier, PurcFetcher::RetrieveRecordsOptions&&, PurcFetcher::DOMCacheEngine::RecordsCallback&&);
    void deleteMatchingRecords(uint64_t cacheIdentifier, PurcFetcher::ResourceRequest&&, PurcFetcher::CacheQueryOptions&&, PurcFetcher::DOMCacheEngine::RecordIdentifiersCallback&&);
    void putRecords(uint64_t cacheIdentifier, Vector<PurcFetcher::DOMCacheEngine::Record>&&, PurcFetcher::DOMCacheEngine::RecordIdentifiersCallback&&);

    void reference(uint64_t cacheIdentifier);
    void dereference(uint64_t cacheIdentifier);

    void clearMemoryRepresentation(PurcFetcher::ClientOrigin&&, CompletionHandler<void(Optional<PurcFetcher::DOMCacheEngine::Error>&&)>&&);
    void engineRepresentation( CompletionHandler<void(String&&)>&&);
    
    PAL::SessionID sessionID() const;

    NetworkConnectionToWebProcess& m_connection;
    HashMap<CacheStorage::CacheIdentifier, CacheStorage::LockCount> m_cachesLocks;
};

}
