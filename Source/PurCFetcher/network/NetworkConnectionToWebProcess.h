/*
 * Copyright (C) 2012-2019 Apple Inc. All rights reserved.
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

#include "CacheStorageEngineConnection.h"
#include "Connection.h"
#include "DownloadID.h"
#include "NetworkActivityTracker.h"
#include "NetworkConnectionToWebProcessMessagesReplies.h"
#include "NetworkResourceLoadMap.h"
#include "PolicyDecision.h"
#include "SandboxExtension.h"
#include "WebPageProxyIdentifier.h"
//#include "WebResourceLoadObserver.h"
#include "WebSocketIdentifier.h"
#include "FrameIdentifier.h"
#include "MessagePortChannelProvider.h"
#include "MessagePortIdentifier.h"
#include "NetworkLoadInformation.h"
#include "NetworkStorageSession.h"
#include "PageIdentifier.h"
#include "ProcessIdentifier.h"
#include "RegistrableDomain.h"
#include <wtf/RefCounted.h>

namespace PAL {
class SessionID;
}

namespace PurcFetcher {
class BlobDataFileReference;
class BlobPart;
class BlobRegistryImpl;
class ResourceError;
class ResourceRequest;
enum class StorageAccessScope : bool;
enum class ShouldAskITP : bool;
struct RequestStorageAccessResult;
struct SameSiteInfo;

enum class HTTPCookieAcceptPolicy : uint8_t;
enum class IncludeSecureCookies : bool;
}

namespace PurcFetcher {

class NetworkSchemeRegistry;
class NetworkProcess;
class NetworkResourceLoader;
class NetworkResourceLoadParameters;
class NetworkSession;
class NetworkSocketChannel;
class NetworkSocketStream;
class ServiceWorkerFetchTask;
class WebSWServerConnection;
class WebSWServerToContextConnection;
typedef uint64_t ResourceLoadIdentifier;

namespace NetworkCache {
struct DataKey;
}

class NetworkConnectionToWebProcess
    : public RefCounted<NetworkConnectionToWebProcess>
#if ENABLE(APPLE_PAY_REMOTE_UI)
    , public WebPaymentCoordinatorProxy::Client
#endif
#if HAVE(COOKIE_CHANGE_LISTENER_API)
    , public PurcFetcher::CookieChangeObserver
#endif
    , IPC::Connection::Client {
public:
    using RegistrableDomain = PurcFetcher::RegistrableDomain;

    static Ref<NetworkConnectionToWebProcess> create(NetworkProcess&, PurcFetcher::ProcessIdentifier, PAL::SessionID, IPC::Connection::Identifier);
    virtual ~NetworkConnectionToWebProcess();
    
    PAL::SessionID sessionID() const { return m_sessionID; }
    NetworkSession* networkSession();

    IPC::Connection& connection() { return m_connection.get(); }
    NetworkProcess& networkProcess() { return m_networkProcess.get(); }

    void didCleanupResourceLoader(NetworkResourceLoader&);
    void transferKeptAliveLoad(NetworkResourceLoader&);
    void setOnLineState(bool);

    bool captureExtraNetworkLoadMetricsEnabled() const { return m_captureExtraNetworkLoadMetricsEnabled; }

    RefPtr<PurcFetcher::BlobDataFileReference> getBlobDataFileReferenceForPath(const String& path);

    void cleanupForSuspension(Function<void()>&&);
    void endSuspension();

    void getNetworkLoadInformationResponse(ResourceLoadIdentifier identifier, CompletionHandler<void(const PurcFetcher::ResourceResponse&)>&& completionHandler)
    {
        completionHandler(m_networkLoadInformationByID.get(identifier).response);
    }

    void getNetworkLoadIntermediateInformation(ResourceLoadIdentifier identifier, CompletionHandler<void(const Vector<PurcFetcher::NetworkTransactionInformation>&)>&& completionHandler)
    {
        completionHandler(m_networkLoadInformationByID.get(identifier).transactions);
    }

    void takeNetworkLoadInformationMetrics(ResourceLoadIdentifier identifier, CompletionHandler<void(const PurcFetcher::NetworkLoadMetrics&)>&& completionHandler)
    {
        completionHandler(m_networkLoadInformationByID.take(identifier).metrics);
    }

    void addNetworkLoadInformation(ResourceLoadIdentifier identifier, PurcFetcher::NetworkLoadInformation&& information)
    {
        ASSERT(!m_networkLoadInformationByID.contains(identifier));
        m_networkLoadInformationByID.add(identifier, WTFMove(information));
    }

    void addNetworkLoadInformationMetrics(ResourceLoadIdentifier identifier, const PurcFetcher::NetworkLoadMetrics& metrics)
    {
        ASSERT(m_networkLoadInformationByID.contains(identifier));
        m_networkLoadInformationByID.ensure(identifier, [] {
            return PurcFetcher::NetworkLoadInformation { };
        }).iterator->value.metrics = metrics;
    }

    void removeNetworkLoadInformation(ResourceLoadIdentifier identifier)
    {
        m_networkLoadInformationByID.remove(identifier);
    }

    Optional<NetworkActivityTracker> startTrackingResourceLoad(PurcFetcher::PageIdentifier, ResourceLoadIdentifier resourceID, bool isTopResource);
    void stopTrackingResourceLoad(ResourceLoadIdentifier resourceID, NetworkActivityTracker::CompletionCode);

    void removeSocketChannel(WebSocketIdentifier);

    PurcFetcher::ProcessIdentifier webProcessIdentifier() const { return m_webProcessIdentifier; }

    void checkProcessLocalPortForActivity(const PurcFetcher::MessagePortIdentifier&, CompletionHandler<void(PurcFetcher::MessagePortChannelProvider::HasActivity)>&&);

#if ENABLE(SERVICE_WORKER)
    void serverToContextConnectionNoLongerNeeded();
    WebSWServerConnection& swConnection();
    std::unique_ptr<ServiceWorkerFetchTask> createFetchTask(NetworkResourceLoader&, const PurcFetcher::ResourceRequest&);
#endif

    NetworkSchemeRegistry& schemeRegistry() { return m_schemeRegistry.get(); }

    void cookieAcceptPolicyChanged(PurcFetcher::HTTPCookieAcceptPolicy);

private:
    NetworkConnectionToWebProcess(NetworkProcess&, PurcFetcher::ProcessIdentifier, PAL::SessionID, IPC::Connection::Identifier);

    void didFinishPreconnection(uint64_t preconnectionIdentifier, const PurcFetcher::ResourceError&);
    PurcFetcher::NetworkStorageSession* storageSession();

    // IPC::Connection::Client
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;
    void didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, std::unique_ptr<IPC::Encoder>&) override;
    void didClose(IPC::Connection&) override;
    void didReceiveInvalidMessage(IPC::Connection&, IPC::MessageName) override;

    // Message handlers.
    void didReceiveNetworkConnectionToWebProcessMessage(IPC::Connection&, IPC::Decoder&);
    void didReceiveSyncNetworkConnectionToWebProcessMessage(IPC::Connection&, IPC::Decoder&, std::unique_ptr<IPC::Encoder>&);

    void scheduleResourceLoad(NetworkResourceLoadParameters&&);
    void performSynchronousLoad(NetworkResourceLoadParameters&&, Messages::NetworkConnectionToWebProcess::PerformSynchronousLoadDelayedReply&&);
    void testProcessIncomingSyncMessagesWhenWaitingForSyncReply(WebPageProxyIdentifier, Messages::NetworkConnectionToWebProcess::TestProcessIncomingSyncMessagesWhenWaitingForSyncReplyDelayedReply&&);
    void loadPing(NetworkResourceLoadParameters&&);
    void prefetchDNS(const String&);
    void preconnectTo(Optional<uint64_t> preconnectionIdentifier, NetworkResourceLoadParameters&&);

    void removeLoadIdentifier(ResourceLoadIdentifier);
    void pageLoadCompleted(PurcFetcher::PageIdentifier);
    void browsingContextRemoved(WebPageProxyIdentifier, PurcFetcher::PageIdentifier, PurcFetcher::FrameIdentifier);
    void crossOriginRedirectReceived(ResourceLoadIdentifier, const URL& redirectURL);
    void startDownload(DownloadID, const PurcFetcher::ResourceRequest&, Optional<NavigatingToAppBoundDomain>, const String& suggestedName = { });
    void convertMainResourceLoadToDownload(uint64_t mainResourceLoadIdentifier, DownloadID, const PurcFetcher::ResourceRequest&, const PurcFetcher::ResourceResponse&, Optional<NavigatingToAppBoundDomain>);

    void registerURLSchemesAsCORSEnabled(Vector<String>&& schemes);

    void cookiesForDOM(const URL& firstParty, const PurcFetcher::SameSiteInfo&, const URL&, PurcFetcher::FrameIdentifier, PurcFetcher::PageIdentifier, PurcFetcher::IncludeSecureCookies, PurcFetcher::ShouldAskITP, PurcFetcher::ShouldRelaxThirdPartyCookieBlocking, CompletionHandler<void(String cookieString, bool secureCookiesAccessed)>&&);
    void setCookiesFromDOM(const URL& firstParty, const PurcFetcher::SameSiteInfo&, const URL&, PurcFetcher::FrameIdentifier, PurcFetcher::PageIdentifier, PurcFetcher::ShouldAskITP, const String&, PurcFetcher::ShouldRelaxThirdPartyCookieBlocking);
    void cookieRequestHeaderFieldValue(const URL& firstParty, const PurcFetcher::SameSiteInfo&, const URL&, Optional<PurcFetcher::FrameIdentifier>, Optional<PurcFetcher::PageIdentifier>, PurcFetcher::IncludeSecureCookies, PurcFetcher::ShouldAskITP, PurcFetcher::ShouldRelaxThirdPartyCookieBlocking, CompletionHandler<void(String cookieString, bool secureCookiesAccessed)>&&);
    void getRawCookies(const URL& firstParty, const PurcFetcher::SameSiteInfo&, const URL&, Optional<PurcFetcher::FrameIdentifier>, Optional<PurcFetcher::PageIdentifier>, PurcFetcher::ShouldAskITP, PurcFetcher::ShouldRelaxThirdPartyCookieBlocking, CompletionHandler<void(Vector<PurcFetcher::Cookie>&&)>&&);
    void setRawCookie(const PurcFetcher::Cookie&);
    void deleteCookie(const URL&, const String& cookieName);

    void setCaptureExtraNetworkLoadMetricsEnabled(bool);

    void createSocketStream(URL&&, String cachePartition, WebSocketIdentifier);

    void createSocketChannel(const PurcFetcher::ResourceRequest&, const String& protocol, WebSocketIdentifier);
    void updateQuotaBasedOnSpaceUsageForTesting(const PurcFetcher::ClientOrigin&);

#if ENABLE(SERVICE_WORKER)
    void establishSWServerConnection();
    void establishSWContextConnection(PurcFetcher::RegistrableDomain&&, CompletionHandler<void()>&&);
    void closeSWContextConnection();
    void unregisterSWConnection();
#endif

    void createNewMessagePortChannel(const PurcFetcher::MessagePortIdentifier& port1, const PurcFetcher::MessagePortIdentifier& port2);
    void entangleLocalPortInThisProcessToRemote(const PurcFetcher::MessagePortIdentifier& local, const PurcFetcher::MessagePortIdentifier& remote);
    void messagePortDisentangled(const PurcFetcher::MessagePortIdentifier&);
    void messagePortClosed(const PurcFetcher::MessagePortIdentifier&);
    void takeAllMessagesForPort(const PurcFetcher::MessagePortIdentifier&, CompletionHandler<void(Vector<PurcFetcher::MessageWithMessagePorts>&&, uint64_t)>&&);
    void postMessageToRemote(PurcFetcher::MessageWithMessagePorts&&, const PurcFetcher::MessagePortIdentifier&);
    void checkRemotePortForActivity(const PurcFetcher::MessagePortIdentifier, CompletionHandler<void(bool)>&&);
    void didDeliverMessagePortMessages(uint64_t messageBatchIdentifier);

#if USE(LIBWEBRTC)
    NetworkRTCProvider& rtcProvider();
#endif
#if ENABLE(WEB_RTC)
    NetworkMDNSRegister& mdnsRegister() { return m_mdnsRegister; }
#endif

    CacheStorageEngineConnection& cacheStorageConnection();

#if ENABLE(RESOURCE_LOAD_STATISTICS)
    void removeStorageAccessForFrame(PurcFetcher::FrameIdentifier, PurcFetcher::PageIdentifier);
    void clearPageSpecificDataForResourceLoadStatistics(PurcFetcher::PageIdentifier);

    void logUserInteraction(const RegistrableDomain&);
    void resourceLoadStatisticsUpdated(Vector<PurcFetcher::ResourceLoadStatistics>&&);
    void hasStorageAccess(const RegistrableDomain& subFrameDomain, const RegistrableDomain& topFrameDomain, PurcFetcher::FrameIdentifier, PurcFetcher::PageIdentifier, CompletionHandler<void(bool)>&&);
    void requestStorageAccess(const RegistrableDomain& subFrameDomain, const RegistrableDomain& topFrameDomain, PurcFetcher::FrameIdentifier, PurcFetcher::PageIdentifier, WebPageProxyIdentifier, PurcFetcher::StorageAccessScope, CompletionHandler<void(PurcFetcher::RequestStorageAccessResult)>&&);
    void requestStorageAccessUnderOpener(PurcFetcher::RegistrableDomain&& domainInNeedOfStorageAccess, PurcFetcher::PageIdentifier openerPageID, PurcFetcher::RegistrableDomain&& openerDomain);
#endif

    void addOriginAccessWhitelistEntry(const String& sourceOrigin, const String& destinationProtocol, const String& destinationHost, bool allowDestinationSubdomains);
    void removeOriginAccessWhitelistEntry(const String& sourceOrigin, const String& destinationProtocol, const String& destinationHost, bool allowDestinationSubdomains);
    void resetOriginAccessWhitelists();

    uint64_t nextMessageBatchIdentifier(Function<void()>&&);

    void domCookiesForHost(const String& host, bool subscribeToCookieChangeNotifications, CompletionHandler<void(const Vector<PurcFetcher::Cookie>&)>&&);

#if HAVE(COOKIE_CHANGE_LISTENER_API)
    void unsubscribeFromCookieChangeNotifications(const HashSet<String>& hosts);

    // PurcFetcher::CookieChangeObserver.
    void cookiesAdded(const String& host, const Vector<PurcFetcher::Cookie>&) final;
    void cookiesDeleted(const String& host, const Vector<PurcFetcher::Cookie>&) final;
    void allCookiesDeleted() final;
#endif

    struct ResourceNetworkActivityTracker {
        ResourceNetworkActivityTracker() = default;
        ResourceNetworkActivityTracker(const ResourceNetworkActivityTracker&) = default;
        ResourceNetworkActivityTracker(ResourceNetworkActivityTracker&&) = default;
        ResourceNetworkActivityTracker(PurcFetcher::PageIdentifier pageID)
            : pageID { pageID }
            , isRootActivity { true }
            , networkActivity { NetworkActivityTracker::Label::LoadPage }
        {
        }

        ResourceNetworkActivityTracker(PurcFetcher::PageIdentifier pageID, ResourceLoadIdentifier resourceID)
            : pageID { pageID }
            , resourceID { resourceID }
            , networkActivity { NetworkActivityTracker::Label::LoadResource }
        {
        }

        PurcFetcher::PageIdentifier pageID;
        ResourceLoadIdentifier resourceID { 0 };
        bool isRootActivity { false };
        NetworkActivityTracker networkActivity;
    };

    void stopAllNetworkActivityTracking();
    void stopAllNetworkActivityTrackingForPage(PurcFetcher::PageIdentifier);
    size_t findRootNetworkActivity(PurcFetcher::PageIdentifier);
    size_t findNetworkActivityTracker(ResourceLoadIdentifier resourceID);

    void hasUploadStateChanged(bool);

#if ENABLE(APPLE_PAY_REMOTE_UI)
    WebPaymentCoordinatorProxy& paymentCoordinator();

    // WebPaymentCoordinatorProxy::Client
    IPC::Connection* paymentCoordinatorConnection(const WebPaymentCoordinatorProxy&) final;
    UIViewController *paymentCoordinatorPresentingViewController(const WebPaymentCoordinatorProxy&) final;
    const String& paymentCoordinatorBoundInterfaceIdentifier(const WebPaymentCoordinatorProxy&) final;
    const String& paymentCoordinatorCTDataConnectionServiceType(const WebPaymentCoordinatorProxy&) final;
    const String& paymentCoordinatorSourceApplicationBundleIdentifier(const WebPaymentCoordinatorProxy&) final;
    const String& paymentCoordinatorSourceApplicationSecondaryIdentifier(const WebPaymentCoordinatorProxy&) final;
    std::unique_ptr<PaymentAuthorizationPresenter> paymentCoordinatorAuthorizationPresenter(WebPaymentCoordinatorProxy&, PKPaymentRequest *) final;
    void paymentCoordinatorAddMessageReceiver(WebPaymentCoordinatorProxy&, IPC::ReceiverName, IPC::MessageReceiver&) final;
    void paymentCoordinatorRemoveMessageReceiver(WebPaymentCoordinatorProxy&, IPC::ReceiverName) final;
#endif

    Ref<IPC::Connection> m_connection;
    Ref<NetworkProcess> m_networkProcess;
    PAL::SessionID m_sessionID;

    HashMap<WebSocketIdentifier, RefPtr<NetworkSocketStream>> m_networkSocketStreams;
    HashMap<WebSocketIdentifier, std::unique_ptr<NetworkSocketChannel>> m_networkSocketChannels;
    NetworkResourceLoadMap m_networkResourceLoaders;
    Vector<ResourceNetworkActivityTracker> m_networkActivityTrackers;

    HashMap<ResourceLoadIdentifier, PurcFetcher::NetworkLoadInformation> m_networkLoadInformationByID;


#if USE(LIBWEBRTC)
    RefPtr<NetworkRTCProvider> m_rtcProvider;
#endif
#if ENABLE(WEB_RTC)
    NetworkMDNSRegister m_mdnsRegister;
#endif
#if HAVE(COOKIE_CHANGE_LISTENER_API)
    HashSet<String> m_hostsWithCookieListeners;
#endif

    bool m_captureExtraNetworkLoadMetricsEnabled { false };

    RefPtr<CacheStorageEngineConnection> m_cacheStorageConnection;

#if ENABLE(SERVICE_WORKER)
    WeakPtr<WebSWServerConnection> m_swConnection;
    std::unique_ptr<WebSWServerToContextConnection> m_swContextConnection;
#endif

#if ENABLE(APPLE_PAY_REMOTE_UI)
    std::unique_ptr<WebPaymentCoordinatorProxy> m_paymentCoordinator;
#endif
    const PurcFetcher::ProcessIdentifier m_webProcessIdentifier;

    HashSet<PurcFetcher::MessagePortIdentifier> m_processEntangledPorts;
    HashMap<uint64_t, Function<void()>> m_messageBatchDeliveryCompletionHandlers;
    Ref<NetworkSchemeRegistry> m_schemeRegistry;
};

} // namespace PurcFetcher
