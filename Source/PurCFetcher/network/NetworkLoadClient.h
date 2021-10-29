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

#include "ResourceError.h"
#include "ResourceRequest.h"
#include "ResourceResponse.h"

namespace PurcFetcher {
class AuthenticationChallenge;
class NetworkLoadMetrics;
class SharedBuffer;
enum class PolicyAction : uint8_t;
}

namespace PurcFetcher {

using ResponseCompletionHandler = CompletionHandler<void(PurcFetcher::PolicyAction)>;

class NetworkLoadClient {
public:
    virtual ~NetworkLoadClient() { }

    virtual bool isSynchronous() const = 0;

    virtual bool isAllowedToAskUserForCredentials() const = 0;

    virtual void didSendData(unsigned long long bytesSent, unsigned long long totalBytesToBeSent) = 0;
    virtual void willSendRedirectedRequest(PurcFetcher::ResourceRequest&&, PurcFetcher::ResourceRequest&& redirectRequest, PurcFetcher::ResourceResponse&& redirectResponse) = 0;
    virtual void didReceiveResponse(PurcFetcher::ResourceResponse&&, ResponseCompletionHandler&&) = 0;
    virtual void didReceiveBuffer(Ref<PurcFetcher::SharedBuffer>&&, int reportedEncodedDataLength) = 0;
    virtual void didFinishLoading(const PurcFetcher::NetworkLoadMetrics&) = 0;
    virtual void didFailLoading(const PurcFetcher::ResourceError&) = 0;
    virtual void didBlockAuthenticationChallenge() { };
    virtual void didReceiveChallenge(const PurcFetcher::AuthenticationChallenge&) { };
    virtual bool shouldCaptureExtraNetworkLoadMetrics() const { return false; }
};

} // namespace PurcFetcher
