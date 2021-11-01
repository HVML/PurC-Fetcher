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

#include "config.h"
#include "NetworkProcessProxy.h"

#include "AuthenticationManager.h"
#include "DownloadProxyMessages.h"
#if ENABLE(LEGACY_CUSTOM_PROTOCOL_MANAGER)
#include "LegacyCustomProtocolManagerMessages.h"
#include "LegacyCustomProtocolManagerProxyMessages.h"
#endif
#include "Logging.h"
#include "NetworkProcessCreationParameters.h"
#include "NetworkProcessMessages.h"
#include "NetworkProcessProxyMessages.h"
#include "SandboxExtension.h"
#if HAVE(SEC_KEY_PROXY)
#include "SecKeyProxyStore.h"
#endif
#include <wtf/CompletionHandler.h>

#define MESSAGE_CHECK(assertion) MESSAGE_CHECK_BASE(assertion, connection())

namespace PurcFetcher {
using namespace PurcFetcher;

NetworkProcessProxy::NetworkProcessProxy()
{
}

NetworkProcessProxy::~NetworkProcessProxy()
{
}

void NetworkProcessProxy::didReceiveMessage(IPC::Connection&, IPC::Decoder&)
{
}

void NetworkProcessProxy::didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, std::unique_ptr<IPC::Encoder>&)
{
}

void NetworkProcessProxy::didClose(IPC::Connection&)
{
}

void NetworkProcessProxy::didReceiveInvalidMessage(IPC::Connection&, IPC::MessageName)
{
}

void NetworkProcessProxy::didReceiveNetworkProcessProxyMessage(IPC::Connection&, IPC::Decoder&)
{
}

void NetworkProcessProxy::didReceiveAuthenticationChallenge(PAL::SessionID, WebPageProxyIdentifier, const Optional<SecurityOriginData>&, PurcFetcher::AuthenticationChallenge&&, bool, uint64_t)
{
}

void NetworkProcessProxy::negotiatedLegacyTLS(WebPageProxyIdentifier)
{
}

void NetworkProcessProxy::didNegotiateModernTLS(WebPageProxyIdentifier, const PurcFetcher::AuthenticationChallenge&)
{
}

void NetworkProcessProxy::didFetchWebsiteData(CallbackID, const WebsiteData&)
{
}

void NetworkProcessProxy::didDeleteWebsiteData(CallbackID)
{
}

void NetworkProcessProxy::didDeleteWebsiteDataForOrigins(CallbackID)
{
}

void NetworkProcessProxy::setWebProcessHasUploads(PurcFetcher::ProcessIdentifier, bool)
{
}

void NetworkProcessProxy::logDiagnosticMessage(WebPageProxyIdentifier, const String&, const String&, PurcFetcher::ShouldSample)
{
}

void NetworkProcessProxy::logDiagnosticMessageWithResult(WebPageProxyIdentifier, const String&, const String&, uint32_t, PurcFetcher::ShouldSample)
{
}

void NetworkProcessProxy::logDiagnosticMessageWithValue(WebPageProxyIdentifier, const String&, const String&, double, unsigned, PurcFetcher::ShouldSample)
{
}

void NetworkProcessProxy::retrieveCacheStorageParameters(PAL::SessionID)
{
}


void NetworkProcessProxy::terminateWebProcess(PurcFetcher::ProcessIdentifier)
{
}

void NetworkProcessProxy::requestStorageSpace(PAL::SessionID, const PurcFetcher::ClientOrigin&, uint64_t, uint64_t, uint64_t, CompletionHandler<void(Optional<uint64_t> quota)>&&)
{
}

void NetworkProcessProxy::syncAllCookies()
{
}

void NetworkProcessProxy::didSyncAllCookies()
{
}

void NetworkProcessProxy::terminateUnresponsiveServiceWorkerProcesses(PurcFetcher::ProcessIdentifier)
{
}

void NetworkProcessProxy::setIsHoldingLockedFiles(bool)
{
}

void NetworkProcessProxy::getAppBoundDomains(PAL::SessionID, CompletionHandler<void(HashSet<PurcFetcher::RegistrableDomain>&&)>&&)
{
}

void NetworkProcessProxy::resourceLoadDidSendRequest(WebPageProxyIdentifier, ResourceLoadInfo&&, PurcFetcher::ResourceRequest&&, Optional<IPC::FormDataReference>&&)
{
}

void NetworkProcessProxy::resourceLoadDidPerformHTTPRedirection(WebPageProxyIdentifier, ResourceLoadInfo&&, PurcFetcher::ResourceResponse&&, PurcFetcher::ResourceRequest&&)
{
}

void NetworkProcessProxy::resourceLoadDidReceiveChallenge(WebPageProxyIdentifier, ResourceLoadInfo&&, PurcFetcher::AuthenticationChallenge&&)
{
}

void NetworkProcessProxy::resourceLoadDidReceiveResponse(WebPageProxyIdentifier, ResourceLoadInfo&&, PurcFetcher::ResourceResponse&&)
{
}

void NetworkProcessProxy::resourceLoadDidCompleteWithError(WebPageProxyIdentifier, ResourceLoadInfo&&, PurcFetcher::ResourceResponse&&, PurcFetcher::ResourceError&&)
{
}

void NetworkProcessProxy::testProcessIncomingSyncMessagesWhenWaitingForSyncReply(WebPageProxyIdentifier, Messages::NetworkProcessProxy::TestProcessIncomingSyncMessagesWhenWaitingForSyncReplyDelayedReply&&)
{
}

} // namespace PurcFetcher

#undef MESSAGE_CHECK
