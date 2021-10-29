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

#include "CacheStorageEngineCaches.h"
#include "NetworkCacheData.h"
#include "WebsiteData.h"
#include "ClientOrigin.h"
#include "StorageQuotaManager.h"
#include "SessionID.h"
#include <wtf/HashMap.h>
#include <wtf/RefCounted.h>
#include <wtf/WeakPtr.h>
#include <wtf/WorkQueue.h>

namespace IPC {
class Connection;
}

namespace WTF {
class CallbackAggregator;
};

namespace PurcFetcher {
struct RetrieveRecordsOptions;
}

namespace PurcFetcher {

class NetworkProcess;

namespace CacheStorage {

using CacheIdentifier = uint64_t;
using LockCount = uint64_t;

class Engine : public RefCounted<Engine>, public CanMakeWeakPtr<Engine> {
public:
    ~Engine();

    static void from(NetworkProcess&, PAL::SessionID, Function<void(Engine&)>&&);
    static void destroyEngine(NetworkProcess&, PAL::SessionID);
    static void fetchEntries(NetworkProcess&, PAL::SessionID, bool shouldComputeSize, CompletionHandler<void(Vector<WebsiteData::Entry>)>&&);

    static void open(NetworkProcess&, PAL::SessionID, PurcFetcher::ClientOrigin&&, String&& cacheName, PurcFetcher::DOMCacheEngine::CacheIdentifierCallback&&);
    static void remove(NetworkProcess&, PAL::SessionID, uint64_t cacheIdentifier, PurcFetcher::DOMCacheEngine::CacheIdentifierCallback&&);
    static void retrieveCaches(NetworkProcess&, PAL::SessionID, PurcFetcher::ClientOrigin&&, uint64_t updateCounter, PurcFetcher::DOMCacheEngine::CacheInfosCallback&&);

    static void retrieveRecords(NetworkProcess&, PAL::SessionID, uint64_t cacheIdentifier, PurcFetcher::RetrieveRecordsOptions&&, PurcFetcher::DOMCacheEngine::RecordsCallback&&);
    static void putRecords(NetworkProcess&, PAL::SessionID, uint64_t cacheIdentifier, Vector<PurcFetcher::DOMCacheEngine::Record>&&, PurcFetcher::DOMCacheEngine::RecordIdentifiersCallback&&);
    static void deleteMatchingRecords(NetworkProcess&, PAL::SessionID, uint64_t cacheIdentifier, PurcFetcher::ResourceRequest&&, PurcFetcher::CacheQueryOptions&&, PurcFetcher::DOMCacheEngine::RecordIdentifiersCallback&&);

    static void lock(NetworkProcess&, PAL::SessionID, uint64_t cacheIdentifier);
    static void unlock(NetworkProcess&, PAL::SessionID, uint64_t cacheIdentifier);

    static void clearMemoryRepresentation(NetworkProcess&, PAL::SessionID, PurcFetcher::ClientOrigin&&, PurcFetcher::DOMCacheEngine::CompletionCallback&&);
    static void representation(NetworkProcess&, PAL::SessionID, CompletionHandler<void(String&&)>&&);

    static void clearAllCaches(NetworkProcess&, PAL::SessionID, CompletionHandler<void()>&&);
    static void clearCachesForOrigin(NetworkProcess&, PAL::SessionID, PurcFetcher::SecurityOriginData&&, CompletionHandler<void()>&&);

    static void initializeQuotaUser(NetworkProcess&, PAL::SessionID, const PurcFetcher::ClientOrigin&, CompletionHandler<void()>&&);

    static uint64_t diskUsage(const String& rootPath, const PurcFetcher::ClientOrigin&);
    void requestSpace(const PurcFetcher::ClientOrigin&, uint64_t spaceRequested, CompletionHandler<void(PurcFetcher::StorageQuotaManager::Decision)>&&);

    bool shouldPersist() const { return !!m_ioQueue;}

    void writeFile(const String& filename, NetworkCache::Data&&, PurcFetcher::DOMCacheEngine::CompletionCallback&&);
    void readFile(const String& filename, CompletionHandler<void(const NetworkCache::Data&, int error)>&&);
    void removeFile(const String& filename);
    void writeSizeFile(const String&, uint64_t size, CompletionHandler<void()>&&);
    static Optional<uint64_t> readSizeFile(const String&);

    const String& rootPath() const { return m_rootPath; }
    const NetworkCache::Salt& salt() const { return m_salt.value(); }
    uint64_t nextCacheIdentifier() { return ++m_nextCacheIdentifier; }

private:
    Engine(PAL::SessionID, NetworkProcess&, String&& rootPath);

    void open(const PurcFetcher::ClientOrigin&, const String& cacheName, PurcFetcher::DOMCacheEngine::CacheIdentifierCallback&&);
    void remove(uint64_t cacheIdentifier, PurcFetcher::DOMCacheEngine::CacheIdentifierCallback&&);
    void retrieveCaches(const PurcFetcher::ClientOrigin&, uint64_t updateCounter, PurcFetcher::DOMCacheEngine::CacheInfosCallback&&);

    void clearAllCaches(CompletionHandler<void()>&&);
    void clearAllCachesFromDisk(CompletionHandler<void()>&&);
    void clearCachesForOrigin(const PurcFetcher::SecurityOriginData&, CompletionHandler<void()>&&);
    void clearCachesForOriginFromDisk(const PurcFetcher::SecurityOriginData&, CompletionHandler<void()>&&);
    void deleteDirectoryRecursivelyOnBackgroundThread(const String& path, CompletionHandler<void()>&&);

    void clearMemoryRepresentation(const PurcFetcher::ClientOrigin&, PurcFetcher::DOMCacheEngine::CompletionCallback&&);
    String representation();

    void retrieveRecords(uint64_t cacheIdentifier, PurcFetcher::RetrieveRecordsOptions&&, PurcFetcher::DOMCacheEngine::RecordsCallback&&);
    void putRecords(uint64_t cacheIdentifier, Vector<PurcFetcher::DOMCacheEngine::Record>&&, PurcFetcher::DOMCacheEngine::RecordIdentifiersCallback&&);
    void deleteMatchingRecords(uint64_t cacheIdentifier, PurcFetcher::ResourceRequest&&, PurcFetcher::CacheQueryOptions&&, PurcFetcher::DOMCacheEngine::RecordIdentifiersCallback&&);

    void lock(uint64_t cacheIdentifier);
    void unlock(uint64_t cacheIdentifier);

    String cachesRootPath(const PurcFetcher::ClientOrigin&);

    void fetchEntries(bool /* shouldComputeSize */, CompletionHandler<void(Vector<WebsiteData::Entry>)>&&);

    void getDirectories(CompletionHandler<void(const Vector<String>&)>&&);
    void fetchDirectoryEntries(bool shouldComputeSize, const Vector<String>& folderPaths, CompletionHandler<void(Vector<WebsiteData::Entry>)>&&);
    void clearCachesForOriginFromDirectories(const Vector<String>&, const PurcFetcher::SecurityOriginData&, CompletionHandler<void()>&&);

    void initialize(PurcFetcher::DOMCacheEngine::CompletionCallback&&);

    using CachesOrError = Expected<std::reference_wrapper<Caches>, PurcFetcher::DOMCacheEngine::Error>;
    using CachesCallback = Function<void(CachesOrError&&)>;
    void readCachesFromDisk(const PurcFetcher::ClientOrigin&, CachesCallback&&);

    using CacheOrError = Expected<std::reference_wrapper<Cache>, PurcFetcher::DOMCacheEngine::Error>;
    using CacheCallback = Function<void(CacheOrError&&)>;
    void readCache(uint64_t cacheIdentifier, CacheCallback&&);

    CompletionHandler<void()> createClearTask(CompletionHandler<void()>&&);

    Cache* cache(uint64_t cacheIdentifier);

    PAL::SessionID m_sessionID;
    WeakPtr<NetworkProcess> m_networkProcess;
    HashMap<PurcFetcher::ClientOrigin, RefPtr<Caches>> m_caches;
    uint64_t m_nextCacheIdentifier { 0 };
    String m_rootPath;
    RefPtr<WorkQueue> m_ioQueue;
    Optional<NetworkCache::Salt> m_salt;
    HashMap<CacheIdentifier, LockCount> m_cacheLocks;
    Vector<PurcFetcher::DOMCacheEngine::CompletionCallback> m_initializationCallbacks;
    HashMap<uint64_t, PurcFetcher::DOMCacheEngine::CompletionCallback> m_pendingWriteCallbacks;
    HashMap<uint64_t, CompletionHandler<void(const NetworkCache::Data&, int error)>> m_pendingReadCallbacks;
    uint64_t m_pendingCallbacksCounter { 0 };
    Vector<PurcFetcher::DOMCacheEngine::CompletionCallback> m_pendingClearCallbacks;
    uint64_t m_clearTaskCounter { 0 };
};

} // namespace CacheStorage

} // namespace PurcFetcher
