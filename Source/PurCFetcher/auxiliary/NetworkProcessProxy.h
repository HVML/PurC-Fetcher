/*
 * Copyright (C) 2012-2020 Apple Inc. All rights reserved.
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

#include "ResourceError.h"
#include "Connection.h"
#include "CallbackID.h"
#include "WebPageProxyIdentifier.h"
#include "ProcessIdentifier.h"
#include "NetworkProcessProxyMessagesReplies.h"
#include "UserContentControllerIdentifier.h"
#include "CrossSiteNavigationDataTransfer.h"
#include "RegistrableDomain.h"
#include <memory>
#include <wtf/Deque.h>

#if ENABLE(LEGACY_CUSTOM_PROTOCOL_MANAGER)
#include "LegacyCustomProtocolManagerProxy.h"
#endif

namespace IPC {
class FormDataReference;
}

namespace PAL {
class SessionID;
}

namespace PurcFetcher {
class AuthenticationChallenge;
class ProtectionSpace;
class ResourceRequest;
enum class ShouldSample : bool;
enum class StorageAccessPromptWasShown : bool;
enum class StorageAccessWasGranted : bool;
enum class StoredCredentialsPolicy : uint8_t;
class SecurityOrigin;
struct SecurityOriginData;
struct ClientOrigin;
}

namespace PurcFetcher {

class DownloadProxy;
class DownloadProxyMap;
class WebPageProxy;
class WebProcessPool;
class WebUserContentControllerProxy;

enum class ShouldGrandfatherStatistics : bool;
enum class StorageAccessStatus : uint8_t;
enum class WebsiteDataFetchOption : uint8_t;
enum class WebsiteDataType : uint32_t;

struct FrameInfoData;
struct NetworkProcessCreationParameters;
struct ResourceLoadInfo;
struct WebsiteData;

class NetworkProcessProxy final : public CanMakeWeakPtr<NetworkProcessProxy> {
    WTF_MAKE_FAST_ALLOCATED;
public:
    explicit NetworkProcessProxy();
    ~NetworkProcessProxy();

private:
    // IPC::Connection::Client
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&);
    void didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, std::unique_ptr<IPC::Encoder>&);
    void didClose(IPC::Connection&);
    void didReceiveInvalidMessage(IPC::Connection&, IPC::MessageName);
    void didReceiveSyncNetworkProcessProxyMessage(IPC::Connection&, IPC::Decoder&, std::unique_ptr<IPC::Encoder>&);

    // Message handlers
    void didReceiveNetworkProcessProxyMessage(IPC::Connection&, IPC::Decoder&);
    void didReceiveAuthenticationChallenge(PAL::SessionID, WebPageProxyIdentifier, const Optional<PurcFetcher::SecurityOriginData>&, PurcFetcher::AuthenticationChallenge&&, bool, uint64_t challengeID);
    void negotiatedLegacyTLS(WebPageProxyIdentifier);
    void didNegotiateModernTLS(WebPageProxyIdentifier, const PurcFetcher::AuthenticationChallenge&);
    void didFetchWebsiteData(CallbackID, const WebsiteData&);
    void didDeleteWebsiteData(CallbackID);
    void didDeleteWebsiteDataForOrigins(CallbackID);
    void setWebProcessHasUploads(PurcFetcher::ProcessIdentifier, bool);
    void logDiagnosticMessage(WebPageProxyIdentifier, const String& message, const String& description, PurcFetcher::ShouldSample);
    void logDiagnosticMessageWithResult(WebPageProxyIdentifier, const String& message, const String& description, uint32_t result, PurcFetcher::ShouldSample);
    void logDiagnosticMessageWithValue(WebPageProxyIdentifier, const String& message, const String& description, double value, unsigned significantFigures, PurcFetcher::ShouldSample);
    void retrieveCacheStorageParameters(PAL::SessionID);

    void terminateWebProcess(PurcFetcher::ProcessIdentifier);

    void requestStorageSpace(PAL::SessionID, const PurcFetcher::ClientOrigin&, uint64_t quota, uint64_t currentSize, uint64_t spaceRequired, CompletionHandler<void(Optional<uint64_t> quota)>&&);

    void syncAllCookies();
    void didSyncAllCookies();
    void terminateUnresponsiveServiceWorkerProcesses(PurcFetcher::ProcessIdentifier);
    void setIsHoldingLockedFiles(bool);
    void getAppBoundDomains(PAL::SessionID, CompletionHandler<void(HashSet<PurcFetcher::RegistrableDomain>&&)>&&);

    void resourceLoadDidSendRequest(WebPageProxyIdentifier, ResourceLoadInfo&&, PurcFetcher::ResourceRequest&&, Optional<IPC::FormDataReference>&&);
    void resourceLoadDidPerformHTTPRedirection(WebPageProxyIdentifier, ResourceLoadInfo&&, PurcFetcher::ResourceResponse&&, PurcFetcher::ResourceRequest&&);
    void resourceLoadDidReceiveChallenge(WebPageProxyIdentifier, ResourceLoadInfo&&, PurcFetcher::AuthenticationChallenge&&);
    void resourceLoadDidReceiveResponse(WebPageProxyIdentifier, ResourceLoadInfo&&, PurcFetcher::ResourceResponse&&);
    void resourceLoadDidCompleteWithError(WebPageProxyIdentifier, ResourceLoadInfo&&, PurcFetcher::ResourceResponse&&, PurcFetcher::ResourceError&&);

    void testProcessIncomingSyncMessagesWhenWaitingForSyncReply(WebPageProxyIdentifier, Messages::NetworkProcessProxy::TestProcessIncomingSyncMessagesWhenWaitingForSyncReplyDelayedReply&&);
};

} // namespace PurcFetcher
