/*
 * Copyright (C) 2015 Apple Inc. All rights reserved.
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
#include "NetworkDataTask.h"
#include "NetworkLoadClient.h"
#include "NetworkLoadParameters.h"
#include "AuthenticationChallenge.h"
#include <wtf/CompletionHandler.h>
#include <wtf/text/WTFString.h>

namespace PurcFetcher {
class BlobRegistryImpl;
}

namespace WebKit {

class NetworkProcess;

class NetworkLoad final : private NetworkDataTaskClient {
    WTF_MAKE_FAST_ALLOCATED;
public:
    NetworkLoad(NetworkLoadClient&, NetworkLoadParameters&&, NetworkSession&);
    ~NetworkLoad();

    void cancel();

    bool isAllowedToAskUserForCredentials() const;

    const PurcFetcher::ResourceRequest& currentRequest() const { return m_currentRequest; }
    void updateRequestAfterRedirection(PurcFetcher::ResourceRequest&) const;

    const NetworkLoadParameters& parameters() const { return m_parameters; }

    void continueWillSendRequest(PurcFetcher::ResourceRequest&&);

    void convertTaskToDownload(PendingDownload&, const PurcFetcher::ResourceRequest&, const PurcFetcher::ResourceResponse&, ResponseCompletionHandler&&);
    void setPendingDownloadID(DownloadID);
    void setSuggestedFilename(const String&);
    void setPendingDownload(PendingDownload&);
    DownloadID pendingDownloadID() { return m_task->pendingDownloadID(); }

    bool shouldCaptureExtraNetworkLoadMetrics() const final;

    String description() const;

private:
    void initialize(NetworkSession&);

    // NetworkDataTaskClient
    void willPerformHTTPRedirection(PurcFetcher::ResourceResponse&&, PurcFetcher::ResourceRequest&&, RedirectCompletionHandler&&) final;
    void didReceiveChallenge(PurcFetcher::AuthenticationChallenge&&, NegotiatedLegacyTLS, ChallengeCompletionHandler&&) final;
    void didReceiveResponse(PurcFetcher::ResourceResponse&&, NegotiatedLegacyTLS, ResponseCompletionHandler&&) final;
    void didReceiveData(Ref<PurcFetcher::SharedBuffer>&&) final;
    void didCompleteWithError(const PurcFetcher::ResourceError&, const PurcFetcher::NetworkLoadMetrics&) final;
    void didSendData(uint64_t totalBytesSent, uint64_t totalBytesExpectedToSend) final;
    void wasBlocked() final;
    void cannotShowURL() final;
    void wasBlockedByRestrictions() final;
    void didNegotiateModernTLS(const PurcFetcher::AuthenticationChallenge&) final;

    void notifyDidReceiveResponse(PurcFetcher::ResourceResponse&&, NegotiatedLegacyTLS, ResponseCompletionHandler&&);
    void throttleDelayCompleted();

    std::reference_wrapper<NetworkLoadClient> m_client;
    Ref<NetworkProcess> m_networkProcess;
    const NetworkLoadParameters m_parameters;
    CompletionHandler<void(PurcFetcher::ResourceRequest&&)> m_redirectCompletionHandler;
    RefPtr<NetworkDataTask> m_task;
    
    struct Throttle;
    std::unique_ptr<Throttle> m_throttle;
    Seconds m_loadThrottleLatency;

    PurcFetcher::ResourceRequest m_currentRequest; // Updated on redirects.
};

} // namespace WebKit
