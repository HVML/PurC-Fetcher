/*
 * Copyright (C) 2016 Igalia S.L.
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

#include "NetworkDataTask.h"
#include "FrameIdentifier.h"
#include "NetworkLoadMetrics.h"
#include "PageIdentifier.h"
#include "ProtectionSpace.h"
#include "ResourceResponse.h"
#include <wtf/RunLoop.h>
#include <wtf/glib/GRefPtr.h>

namespace WebKit {

class NetworkDataTaskSoup final : public NetworkDataTask {
public:
    static Ref<NetworkDataTask> create(NetworkSession& session, NetworkDataTaskClient& client, const PurcFetcher::ResourceRequest& request, PurcFetcher::FrameIdentifier frameID, PurcFetcher::PageIdentifier pageID, PurcFetcher::StoredCredentialsPolicy storedCredentialsPolicy, PurcFetcher::ContentSniffingPolicy shouldContentSniff, PurcFetcher::ContentEncodingSniffingPolicy shouldContentEncodingSniff, bool shouldClearReferrerOnHTTPSToHTTPRedirect, bool dataTaskIsForMainFrameNavigation)
    {
        return adoptRef(*new NetworkDataTaskSoup(session, client, request, frameID, pageID, storedCredentialsPolicy, shouldContentSniff, shouldContentEncodingSniff, shouldClearReferrerOnHTTPSToHTTPRedirect, dataTaskIsForMainFrameNavigation));
    }

    ~NetworkDataTaskSoup();

private:
    NetworkDataTaskSoup(NetworkSession&, NetworkDataTaskClient&, const PurcFetcher::ResourceRequest&, PurcFetcher::FrameIdentifier, PurcFetcher::PageIdentifier, PurcFetcher::StoredCredentialsPolicy, PurcFetcher::ContentSniffingPolicy, PurcFetcher::ContentEncodingSniffingPolicy, bool shouldClearReferrerOnHTTPSToHTTPRedirect, bool dataTaskIsForMainFrameNavigation);

    void cancel() override;
    void resume() override;
    void invalidateAndCancel() override;
    NetworkDataTask::State state() const override;

    void setPendingDownloadLocation(const String&, SandboxExtension::Handle&&, bool /*allowOverwrite*/) override;
    String suggestedFilename() const override;

    void timeoutFired();
    void startTimeout();
    void stopTimeout();

    enum class WasBlockingCookies { No, Yes };
    void createRequest(PurcFetcher::ResourceRequest&&, WasBlockingCookies);
    void clearRequest();
    static void sendRequestCallback(SoupRequest*, GAsyncResult*, NetworkDataTaskSoup*);
    void didSendRequest(GRefPtr<GInputStream>&&);
    void dispatchDidReceiveResponse();
    void dispatchDidCompleteWithError(const PurcFetcher::ResourceError&);

    static gboolean tlsConnectionAcceptCertificateCallback(GTlsConnection*, GTlsCertificate*, GTlsCertificateFlags, NetworkDataTaskSoup*);
    bool tlsConnectionAcceptCertificate(GTlsCertificate*, GTlsCertificateFlags);

    void applyAuthenticationToRequest(PurcFetcher::ResourceRequest&);
    static void authenticateCallback(SoupSession*, SoupMessage*, SoupAuth*, gboolean retrying, NetworkDataTaskSoup*);
    void authenticate(PurcFetcher::AuthenticationChallenge&&);
    void continueAuthenticate(PurcFetcher::AuthenticationChallenge&&);

    static void skipInputStreamForRedirectionCallback(GInputStream*, GAsyncResult*, NetworkDataTaskSoup*);
    void skipInputStreamForRedirection();
    void didFinishSkipInputStreamForRedirection();
    bool shouldStartHTTPRedirection();
    void continueHTTPRedirection();

    static void readCallback(GInputStream*, GAsyncResult*, NetworkDataTaskSoup*);
    void read();
    void didRead(gssize bytesRead);
    void didFinishRead();

    static void requestNextPartCallback(SoupMultipartInputStream*, GAsyncResult*, NetworkDataTaskSoup*);
    void requestNextPart();
    void didRequestNextPart(GRefPtr<GInputStream>&&);
    void didFinishRequestNextPart();

    static void gotHeadersCallback(SoupMessage*, NetworkDataTaskSoup*);
    void didGetHeaders();

    static void wroteBodyDataCallback(SoupMessage*, SoupBuffer*, NetworkDataTaskSoup*);
    void didWriteBodyData(uint64_t bytesSent);

    void download();
    static void writeDownloadCallback(GOutputStream*, GAsyncResult*, NetworkDataTaskSoup*);
    void writeDownload();
    void didWriteDownload(gsize bytesWritten);
    void didFailDownload(const PurcFetcher::ResourceError&);
    void didFinishDownload();
    void cleanDownloadFiles();

    void didFail(const PurcFetcher::ResourceError&);

    static void networkEventCallback(SoupMessage*, GSocketClientEvent, GIOStream*, NetworkDataTaskSoup*);
    void networkEvent(GSocketClientEvent, GIOStream*);
#if SOUP_CHECK_VERSION(2, 49, 91)
    static void startingCallback(SoupMessage*, NetworkDataTaskSoup*);
#else
    static void requestStartedCallback(SoupSession*, SoupMessage*, SoupSocket*, NetworkDataTaskSoup*);
#endif
#if SOUP_CHECK_VERSION(2, 67, 1)
    bool shouldAllowHSTSPolicySetting() const;
    bool shouldAllowHSTSProtocolUpgrade() const;
    void protocolUpgradedViaHSTS(SoupMessage*);
    static void hstsEnforced(SoupHSTSEnforcer*, SoupMessage*, NetworkDataTaskSoup*);
#endif
    void didStartRequest();
    static void restartedCallback(SoupMessage*, NetworkDataTaskSoup*);
    void didRestart();

    PurcFetcher::FrameIdentifier m_frameID;
    PurcFetcher::PageIdentifier m_pageID;
    State m_state { State::Suspended };
    PurcFetcher::ContentSniffingPolicy m_shouldContentSniff;
    GRefPtr<SoupRequest> m_soupRequest;
    GRefPtr<SoupMessage> m_soupMessage;
    GRefPtr<GInputStream> m_inputStream;
    GRefPtr<SoupMultipartInputStream> m_multipartInputStream;
    GRefPtr<GCancellable> m_cancellable;
    GRefPtr<GAsyncResult> m_pendingResult;
    PurcFetcher::ProtectionSpace m_protectionSpaceForPersistentStorage;
    PurcFetcher::Credential m_credentialForPersistentStorage;
    PurcFetcher::ResourceRequest m_currentRequest;
    PurcFetcher::ResourceResponse m_response;
    Vector<char> m_readBuffer;
    unsigned m_redirectCount { 0 };
    uint64_t m_bodyDataTotalBytesSent { 0 };
    GRefPtr<GFile> m_downloadDestinationFile;
    GRefPtr<GFile> m_downloadIntermediateFile;
    GRefPtr<GOutputStream> m_downloadOutputStream;
    bool m_allowOverwriteDownload { false };
    PurcFetcher::NetworkLoadMetrics m_networkLoadMetrics;
    MonotonicTime m_startTime;
    bool m_isBlockingCookies { false };
    RunLoop::Timer<NetworkDataTaskSoup> m_timeoutSource;
};

} // namespace WebKit
