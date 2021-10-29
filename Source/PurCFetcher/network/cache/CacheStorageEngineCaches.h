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

#include "CacheStorageEngineCache.h"
#include "NetworkCacheStorage.h"
#include "ClientOrigin.h"
#include <wtf/CompletionHandler.h>
#include <wtf/Deque.h>

namespace PurcFetcher {
class StorageQuotaManager;
}

namespace WebKit {

namespace CacheStorage {

class Engine;

class Caches final : public RefCounted<Caches> {
public:
    static String cachesSizeFilename(const String& cachesRootsPath);
    static Ref<Caches> create(Engine&, PurcFetcher::ClientOrigin&&, String&& rootPath);
    ~Caches();

    static void retrieveOriginFromDirectory(const String& folderPath, WorkQueue&, WTF::CompletionHandler<void(Optional<PurcFetcher::ClientOrigin>&&)>&&);

    void initialize(PurcFetcher::DOMCacheEngine::CompletionCallback&&);
    void open(const String& name, PurcFetcher::DOMCacheEngine::CacheIdentifierCallback&&);
    void remove(uint64_t identifier, PurcFetcher::DOMCacheEngine::CacheIdentifierCallback&&);
    void dispose(Cache&);

    void detach();

    bool isInitialized() const { return m_isInitialized; }
    void cacheInfos(uint64_t updateCounter, PurcFetcher::DOMCacheEngine::CacheInfosCallback&&);

    Cache* find(uint64_t identifier);
    void appendRepresentation(StringBuilder&) const;

    void readRecordsList(Cache&, NetworkCache::Storage::TraverseHandler&&);
    void readRecord(const NetworkCache::Key&, WTF::Function<void(Expected<PurcFetcher::DOMCacheEngine::Record, PurcFetcher::DOMCacheEngine::Error>&&)>&&);

    void requestSpace(uint64_t spaceRequired, PurcFetcher::DOMCacheEngine::CompletionCallback&&);
    void writeRecord(const Cache&, const RecordInformation&, PurcFetcher::DOMCacheEngine::Record&&, uint64_t previousRecordSize, PurcFetcher::DOMCacheEngine::CompletionCallback&&);

    void removeCacheEntry(const NetworkCache::Key&);
    void removeRecord(const RecordInformation&);

    const NetworkCache::Salt& salt() const;
    const PurcFetcher::ClientOrigin& origin() const { return m_origin; }

    bool shouldPersist() const { return !m_rootPath.isNull(); }

    void clear(WTF::CompletionHandler<void()>&&);
    void clearMemoryRepresentation();

    uint64_t storageSize() const;
    void updateSizeFile(CompletionHandler<void()>&&);

private:
    Caches(Engine&, PurcFetcher::ClientOrigin&&, String&& rootPath);

    void initializeSize();
    void readCachesFromDisk(WTF::Function<void(Expected<Vector<Cache>, PurcFetcher::DOMCacheEngine::Error>&&)>&&);
    void writeCachesToDisk(PurcFetcher::DOMCacheEngine::CompletionCallback&&);

    void storeOrigin(PurcFetcher::DOMCacheEngine::CompletionCallback&&);
    static Optional<PurcFetcher::ClientOrigin> readOrigin(const NetworkCache::Data&);

    Cache* find(const String& name);
    void clearPendingWritingCachesToDiskCallbacks();

    void makeDirty() { ++m_updateCounter; }
    bool isDirty(uint64_t updateCounter) const;

    bool hasActiveCache() const;

    bool m_isInitialized { false };
    Engine* m_engine { nullptr };
    uint64_t m_updateCounter { 0 };
    PurcFetcher::ClientOrigin m_origin;
    String m_rootPath;
    uint64_t m_size { 0 };
    Vector<Cache> m_caches;
    Vector<Cache> m_removedCaches;
    RefPtr<NetworkCache::Storage> m_storage;
    HashMap<NetworkCache::Key, PurcFetcher::DOMCacheEngine::Record> m_volatileStorage;
    mutable Optional<NetworkCache::Salt> m_volatileSalt;
    Vector<PurcFetcher::DOMCacheEngine::CompletionCallback> m_pendingInitializationCallbacks;
    bool m_isWritingCachesToDisk { false };
    Deque<CompletionHandler<void(Optional<PurcFetcher::DOMCacheEngine::Error>)>> m_pendingWritingCachesToDiskCallbacks;
};

} // namespace CacheStorage

} // namespace WebKit
