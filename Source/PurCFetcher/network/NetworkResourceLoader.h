/*
 * Copyright (C) 2012-2015 Apple Inc. All rights reserved.
 * Copyright (C) 2019 ~ 2020, Beijing FMSoft Technologies Co., Ltd.
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

#include "DownloadID.h"
#include "MessageSender.h"
#include "NetworkCache.h"
#include "NetworkConnectionToWebProcess.h"
#include "NetworkConnectionToWebProcessMessagesReplies.h"
#include "NetworkLoadClient.h"
#include "NetworkResourceLoadIdentifier.h"
#include "NetworkResourceLoadParameters.h"
#include "AdClickAttribution.h"
#include "ContentSecurityPolicyClient.h"
#include "CrossOriginAccessControl.h"
#include "ResourceResponse.h"
//#include "SecurityPolicyViolationEvent.h"
#include "Timer.h"
#include <wtf/WeakPtr.h>

namespace PurcFetcher {
class FormData;
class NetworkStorageSession;
class ResourceRequest;
}

namespace WebKit {

class NetworkConnectionToWebProcess;
class NetworkLoad;
class NetworkLoadChecker;
class ServiceWorkerFetchTask;
class WebSWServerConnection;

enum class NegotiatedLegacyTLS : bool;

struct ResourceLoadInfo;

namespace NetworkCache {
class Entry;
}

class NetworkResourceLoader final
    : public RefCounted<NetworkResourceLoader>
    , public NetworkLoadClient
    , public IPC::MessageSender
    , public PurcFetcher::ContentSecurityPolicyClient
    , public PurcFetcher::CrossOriginAccessControlCheckDisabler
    , public CanMakeWeakPtr<NetworkResourceLoader> {
public:
    static Ref<NetworkResourceLoader> create(NetworkResourceLoadParameters&& parameters, NetworkConnectionToWebProcess& connection, Messages::NetworkConnectionToWebProcess::PerformSynchronousLoadDelayedReply&& reply = nullptr)
    {
        return adoptRef(*new NetworkResourceLoader(WTFMove(parameters), connection, WTFMove(reply)));
    }
    virtual ~NetworkResourceLoader();

    const PurcFetcher::ResourceRequest& originalRequest() const { return m_parameters.request; }

    NetworkLoad* networkLoad() const { return m_networkLoad.get(); }

    void start();
    void abort();

    // Message handlers.
    void didReceiveNetworkResourceLoaderMessage(IPC::Connection&, IPC::Decoder&);

    void continueWillSendRequest(PurcFetcher::ResourceRequest&& newRequest, bool isAllowedToAskUserForCredentials);

    const PurcFetcher::ResourceResponse& response() const { return m_response; }

    NetworkConnectionToWebProcess& connectionToWebProcess() const { return m_connection; }
    PAL::SessionID sessionID() const { return m_connection->sessionID(); }
    ResourceLoadIdentifier identifier() const { return m_parameters.identifier; }
    PurcFetcher::FrameIdentifier frameID() const { return m_parameters.webFrameID; }
    PurcFetcher::PageIdentifier pageID() const { return m_parameters.webPageID; }
    const NetworkResourceLoadParameters& parameters() const { return m_parameters; }

    NetworkCache::GlobalFrameID globalFrameID() { return { m_parameters.webPageProxyID, pageID(), frameID() }; }

    struct SynchronousLoadData;

    // NetworkLoadClient.
    void didSendData(unsigned long long bytesSent, unsigned long long totalBytesToBeSent) final;
    bool isSynchronous() const final;
    bool isAllowedToAskUserForCredentials() const final { return m_isAllowedToAskUserForCredentials; }
    void willSendRedirectedRequest(PurcFetcher::ResourceRequest&&, PurcFetcher::ResourceRequest&& redirectRequest, PurcFetcher::ResourceResponse&&) final;
    void didReceiveResponse(PurcFetcher::ResourceResponse&&, ResponseCompletionHandler&&) final;
    void didReceiveBuffer(Ref<PurcFetcher::SharedBuffer>&&, int reportedEncodedDataLength) final;
    void didFinishLoading(const PurcFetcher::NetworkLoadMetrics&) final;
    void didFailLoading(const PurcFetcher::ResourceError&) final;
    void didBlockAuthenticationChallenge() final;
    void didReceiveChallenge(const PurcFetcher::AuthenticationChallenge&) final;
    bool shouldCaptureExtraNetworkLoadMetrics() const final;

    // CrossOriginAccessControlCheckDisabler
    bool crossOriginAccessControlCheckEnabled() const override;
        
    void convertToDownload(DownloadID, const PurcFetcher::ResourceRequest&, const PurcFetcher::ResourceResponse&);

    bool isMainResource() const { return m_parameters.request.requester() == PurcFetcher::ResourceRequest::Requester::Main; }
    bool isMainFrameLoad() const { return isMainResource() && m_parameters.frameAncestorOrigins.isEmpty(); }
    bool isCrossOriginPrefetch() const;

    bool isAlwaysOnLoggingAllowed() const;

#if ENABLE(RESOURCE_LOAD_STATISTICS) && !RELEASE_LOG_DISABLED
    static bool shouldLogCookieInformation(NetworkConnectionToWebProcess&, const PAL::SessionID&);
    static void logCookieInformation(NetworkConnectionToWebProcess&, const String& label, const void* loggedObject, const PurcFetcher::NetworkStorageSession&, const URL& firstParty, const PurcFetcher::SameSiteInfo&, const URL&, const String& referrer, Optional<PurcFetcher::FrameIdentifier>, Optional<PurcFetcher::PageIdentifier>, Optional<uint64_t> identifier);
#endif

    void disableExtraNetworkLoadMetricsCapture() { m_shouldCaptureExtraNetworkLoadMetrics = false; }

    bool isKeptAlive() const { return m_isKeptAlive; }

    void consumeSandboxExtensionsIfNeeded();

#if ENABLE(SERVICE_WORKER)
    void startWithServiceWorker();
    void serviceWorkerDidNotHandle(ServiceWorkerFetchTask*);
#endif

private:
    NetworkResourceLoader(NetworkResourceLoadParameters&&, NetworkConnectionToWebProcess&, Messages::NetworkConnectionToWebProcess::PerformSynchronousLoadDelayedReply&&);

    // IPC::MessageSender
    IPC::Connection* messageSenderConnection() const override;
    uint64_t messageSenderDestinationID() const override { return m_parameters.identifier; }

    bool canUseCache(const PurcFetcher::ResourceRequest&) const;
    bool canUseCachedRedirect(const PurcFetcher::ResourceRequest&) const;

    void tryStoreAsCacheEntry();
    void retrieveCacheEntry(const PurcFetcher::ResourceRequest&);
    void retrieveCacheEntryInternal(std::unique_ptr<NetworkCache::Entry>&&, PurcFetcher::ResourceRequest&&);
    void didRetrieveCacheEntry(std::unique_ptr<NetworkCache::Entry>);
    void sendResultForCacheEntry(std::unique_ptr<NetworkCache::Entry>);
    void validateCacheEntry(std::unique_ptr<NetworkCache::Entry>);
    void dispatchWillSendRequestForCacheEntry(PurcFetcher::ResourceRequest&&, std::unique_ptr<NetworkCache::Entry>&&);

    bool shouldInterruptLoadForXFrameOptions(const String&, const URL&);
    bool shouldInterruptLoadForCSPFrameAncestorsOrXFrameOptions(const PurcFetcher::ResourceResponse&);

    enum class FirstLoad { No, Yes };
    void startNetworkLoad(PurcFetcher::ResourceRequest&&, FirstLoad);
    void restartNetworkLoad(PurcFetcher::ResourceRequest&&);
    void continueDidReceiveResponse();
    void didReceiveMainResourceResponse(const PurcFetcher::ResourceResponse&);

    enum class LoadResult {
        Unknown,
        Success,
        Failure,
        Cancel
    };
    void cleanup(LoadResult);
    
    void platformDidReceiveResponse(const PurcFetcher::ResourceResponse&);

    void startBufferingTimerIfNeeded();
    void bufferingTimerFired();
    void sendBuffer(PurcFetcher::SharedBuffer&, size_t encodedDataLength);

    void consumeSandboxExtensions();
    void invalidateSandboxExtensions();

#if ENABLE(RESOURCE_LOAD_STATISTICS) && !RELEASE_LOG_DISABLED
    void logCookieInformation() const;
#endif

    void continueWillSendRedirectedRequest(PurcFetcher::ResourceRequest&&, PurcFetcher::ResourceRequest&& redirectRequest, PurcFetcher::ResourceResponse&&, Optional<PurcFetcher::AdClickAttribution::Conversion>&&);
    void didFinishWithRedirectResponse(PurcFetcher::ResourceRequest&&, PurcFetcher::ResourceRequest&& redirectRequest, PurcFetcher::ResourceResponse&&);
    PurcFetcher::ResourceResponse sanitizeResponseIfPossible(PurcFetcher::ResourceResponse&&, PurcFetcher::ResourceResponse::SanitizationType);

    // ContentSecurityPolicyClient
    void sendCSPViolationReport(URL&&, Ref<PurcFetcher::FormData>&&) final;
//    void enqueueSecurityPolicyViolationEvent(PurcFetcher::SecurityPolicyViolationEvent::Init&&) final;

    void logSlowCacheRetrieveIfNeeded(const NetworkCache::Cache::RetrieveInfo&);

    void handleAdClickAttributionConversion(PurcFetcher::AdClickAttribution::Conversion&&, const URL&, const PurcFetcher::ResourceRequest&);

    Optional<Seconds> validateCacheEntryForMaxAgeCapValidation(const PurcFetcher::ResourceRequest&, const PurcFetcher::ResourceRequest& redirectRequest, const PurcFetcher::ResourceResponse&);

    ResourceLoadInfo resourceLoadInfo();

    const NetworkResourceLoadParameters m_parameters;

    Ref<NetworkConnectionToWebProcess> m_connection;

    std::unique_ptr<NetworkLoad> m_networkLoad;

    PurcFetcher::ResourceResponse m_response;

    size_t m_bufferedDataEncodedDataLength { 0 };
    RefPtr<PurcFetcher::SharedBuffer> m_bufferedData;
    unsigned m_redirectCount { 0 };

    std::unique_ptr<SynchronousLoadData> m_synchronousLoadData;

    bool m_wasStarted { false };
    bool m_didConsumeSandboxExtensions { false };
    bool m_isAllowedToAskUserForCredentials { false };
    size_t m_numBytesReceived { 0 };

    unsigned m_retrievedDerivedDataCount { 0 };

    PurcFetcher::Timer m_bufferingTimer;
    RefPtr<NetworkCache::Cache> m_cache;
    RefPtr<PurcFetcher::SharedBuffer> m_bufferedDataForCache;
    std::unique_ptr<NetworkCache::Entry> m_cacheEntryForValidation;
    std::unique_ptr<NetworkCache::Entry> m_cacheEntryForMaxAgeCapValidation;
    bool m_isWaitingContinueWillSendRequestForCachedRedirect { false };
    std::unique_ptr<NetworkCache::Entry> m_cacheEntryWaitingForContinueDidReceiveResponse;
    std::unique_ptr<NetworkLoadChecker> m_networkLoadChecker;
    bool m_shouldRestartLoad { false };
    ResponseCompletionHandler m_responseCompletionHandler;
    bool m_shouldCaptureExtraNetworkLoadMetrics { false };
    bool m_isKeptAlive { false };

    Optional<NetworkActivityTracker> m_networkActivityTracker;

    // gengyue
    int m_httpresponsecode;

#if ENABLE(SERVICE_WORKER)
    std::unique_ptr<ServiceWorkerFetchTask> m_serviceWorkerFetchTask;
#endif
    NetworkResourceLoadIdentifier m_resourceLoadID;
    PurcFetcher::ResourceResponse m_redirectResponse;
};

} // namespace WebKit
