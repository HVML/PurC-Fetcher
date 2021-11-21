/*
 * Copyright (C) 2010-2020 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "ArgumentCoders.h"
#include "Connection.h"
#include "Cookie.h"
#include "FrameIdentifier.h"
#include "MessageNames.h"
#include "MessageWithMessagePorts.h"
#include "NavigatingToAppBoundDomain.h"
#include "NetworkConnectionToWebProcessMessagesReplies.h"
#include "PageIdentifier.h"
#include "WebPageProxyIdentifier.h"
#include "WebSocketIdentifier.h"
#include <NetworkLoadInformation.h>
#include <wtf/Forward.h>
#include <wtf/HashSet.h>
#include <wtf/Optional.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace PurCFetcher {
class DownloadID;
class NetworkLoadMetrics;
class NetworkResourceLoadParameters;
class RegistrableDomain;
class ResourceError;
class ResourceRequest;
class ResourceResponse;
enum class IncludeSecureCookies : bool;
enum class ShouldAskITP : bool;
enum class ShouldRelaxThirdPartyCookieBlocking : bool;
struct ClientOrigin;
struct Cookie;
struct MessagePortIdentifier;
struct MessageWithMessagePorts;
struct SameSiteInfo;
}

namespace Messages {
namespace NetworkConnectionToWebProcess {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::NetworkConnectionToWebProcess;
}

class ScheduleResourceLoad {
public:
    using Arguments = std::tuple<const PurCFetcher::NetworkResourceLoadParameters&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_ScheduleResourceLoad; }
    static const bool isSync = false;

    explicit ScheduleResourceLoad(const PurCFetcher::NetworkResourceLoadParameters& resourceLoadParameters)
        : m_arguments(resourceLoadParameters)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class PerformSynchronousLoad {
public:
    using Arguments = std::tuple<const PurCFetcher::NetworkResourceLoadParameters&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_PerformSynchronousLoad; }
    static const bool isSync = true;

    using DelayedReply = PerformSynchronousLoadDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const PurCFetcher::ResourceError& error, const PurCFetcher::ResourceResponse& response, const Vector<char>& data);
    using Reply = std::tuple<PurCFetcher::ResourceError&, PurCFetcher::ResourceResponse&, Vector<char>&>;
    using ReplyArguments = std::tuple<PurCFetcher::ResourceError, PurCFetcher::ResourceResponse, Vector<char>>;
    explicit PerformSynchronousLoad(const PurCFetcher::NetworkResourceLoadParameters& resourceLoadParameters)
        : m_arguments(resourceLoadParameters)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class TestProcessIncomingSyncMessagesWhenWaitingForSyncReply {
public:
    using Arguments = std::tuple<const PurCFetcher::WebPageProxyIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_TestProcessIncomingSyncMessagesWhenWaitingForSyncReply; }
    static const bool isSync = true;

    using DelayedReply = TestProcessIncomingSyncMessagesWhenWaitingForSyncReplyDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool handled);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    explicit TestProcessIncomingSyncMessagesWhenWaitingForSyncReply(const PurCFetcher::WebPageProxyIdentifier& pageID)
        : m_arguments(pageID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class LoadPing {
public:
    using Arguments = std::tuple<const PurCFetcher::NetworkResourceLoadParameters&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_LoadPing; }
    static const bool isSync = false;

    explicit LoadPing(const PurCFetcher::NetworkResourceLoadParameters& resourceLoadParameters)
        : m_arguments(resourceLoadParameters)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RemoveLoadIdentifier {
public:
    using Arguments = std::tuple<uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_RemoveLoadIdentifier; }
    static const bool isSync = false;

    explicit RemoveLoadIdentifier(uint64_t resourceLoadIdentifier)
        : m_arguments(resourceLoadIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class PageLoadCompleted {
public:
    using Arguments = std::tuple<const PurCFetcher::PageIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_PageLoadCompleted; }
    static const bool isSync = false;

    explicit PageLoadCompleted(const PurCFetcher::PageIdentifier& webPageID)
        : m_arguments(webPageID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class BrowsingContextRemoved {
public:
    using Arguments = std::tuple<const PurCFetcher::WebPageProxyIdentifier&, const PurCFetcher::PageIdentifier&, const PurCFetcher::FrameIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_BrowsingContextRemoved; }
    static const bool isSync = false;

    BrowsingContextRemoved(const PurCFetcher::WebPageProxyIdentifier& webPageProxyID, const PurCFetcher::PageIdentifier& webPageID, const PurCFetcher::FrameIdentifier& webFrameID)
        : m_arguments(webPageProxyID, webPageID, webFrameID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class PrefetchDNS {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_PrefetchDNS; }
    static const bool isSync = false;

    explicit PrefetchDNS(const String& hostname)
        : m_arguments(hostname)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class PreconnectTo {
public:
    using Arguments = std::tuple<const Optional<uint64_t>&, const PurCFetcher::NetworkResourceLoadParameters&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_PreconnectTo; }
    static const bool isSync = false;

    PreconnectTo(const Optional<uint64_t>& preconnectionIdentifier, const PurCFetcher::NetworkResourceLoadParameters& loadParameters)
        : m_arguments(preconnectionIdentifier, loadParameters)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class StartDownload {
public:
    using Arguments = std::tuple<const PurCFetcher::DownloadID&, const PurCFetcher::ResourceRequest&, Optional<PurCFetcher::NavigatingToAppBoundDomain>, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_StartDownload; }
    static const bool isSync = false;

    StartDownload(const PurCFetcher::DownloadID& downloadID, const PurCFetcher::ResourceRequest& request, Optional<PurCFetcher::NavigatingToAppBoundDomain> isNavigatingToAppBoundDomain, const String& suggestedName)
        : m_arguments(downloadID, request, isNavigatingToAppBoundDomain, suggestedName)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ConvertMainResourceLoadToDownload {
public:
    using Arguments = std::tuple<uint64_t, const PurCFetcher::DownloadID&, const PurCFetcher::ResourceRequest&, const PurCFetcher::ResourceResponse&, Optional<PurCFetcher::NavigatingToAppBoundDomain>>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_ConvertMainResourceLoadToDownload; }
    static const bool isSync = false;

    ConvertMainResourceLoadToDownload(uint64_t mainResourceLoadIdentifier, const PurCFetcher::DownloadID& downloadID, const PurCFetcher::ResourceRequest& request, const PurCFetcher::ResourceResponse& response, Optional<PurCFetcher::NavigatingToAppBoundDomain> isNavigatingToAppBoundDomain)
        : m_arguments(mainResourceLoadIdentifier, downloadID, request, response, isNavigatingToAppBoundDomain)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class CookiesForDOM {
public:
    using Arguments = std::tuple<const URL&, const PurCFetcher::SameSiteInfo&, const URL&, const PurCFetcher::FrameIdentifier&, const PurCFetcher::PageIdentifier&, PurCFetcher::IncludeSecureCookies, PurCFetcher::ShouldAskITP, PurCFetcher::ShouldRelaxThirdPartyCookieBlocking>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_CookiesForDOM; }
    static const bool isSync = true;

    using DelayedReply = CookiesForDOMDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const String& cookieString, bool didAccessSecureCookies);
    using Reply = std::tuple<String&, bool&>;
    using ReplyArguments = std::tuple<String, bool>;
    CookiesForDOM(const URL& firstParty, const PurCFetcher::SameSiteInfo& sameSiteInfo, const URL& url, const PurCFetcher::FrameIdentifier& frameID, const PurCFetcher::PageIdentifier& pageID, PurCFetcher::IncludeSecureCookies includeSecureCookies, PurCFetcher::ShouldAskITP shouldAskITP, PurCFetcher::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking)
        : m_arguments(firstParty, sameSiteInfo, url, frameID, pageID, includeSecureCookies, shouldAskITP, shouldRelaxThirdPartyCookieBlocking)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetCookiesFromDOM {
public:
    using Arguments = std::tuple<const URL&, const PurCFetcher::SameSiteInfo&, const URL&, const PurCFetcher::FrameIdentifier&, const PurCFetcher::PageIdentifier&, PurCFetcher::ShouldAskITP, const String&, PurCFetcher::ShouldRelaxThirdPartyCookieBlocking>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_SetCookiesFromDOM; }
    static const bool isSync = false;

    SetCookiesFromDOM(const URL& firstParty, const PurCFetcher::SameSiteInfo& sameSiteInfo, const URL& url, const PurCFetcher::FrameIdentifier& frameID, const PurCFetcher::PageIdentifier& pageID, PurCFetcher::ShouldAskITP shouldAskITP, const String& cookieString, PurCFetcher::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking)
        : m_arguments(firstParty, sameSiteInfo, url, frameID, pageID, shouldAskITP, cookieString, shouldRelaxThirdPartyCookieBlocking)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class CookieRequestHeaderFieldValue {
public:
    using Arguments = std::tuple<const URL&, const PurCFetcher::SameSiteInfo&, const URL&, const Optional<PurCFetcher::FrameIdentifier>&, const Optional<PurCFetcher::PageIdentifier>&, PurCFetcher::IncludeSecureCookies, PurCFetcher::ShouldAskITP, PurCFetcher::ShouldRelaxThirdPartyCookieBlocking>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_CookieRequestHeaderFieldValue; }
    static const bool isSync = true;

    using DelayedReply = CookieRequestHeaderFieldValueDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const String& cookieString, bool didAccessSecureCookies);
    using Reply = std::tuple<String&, bool&>;
    using ReplyArguments = std::tuple<String, bool>;
    CookieRequestHeaderFieldValue(const URL& firstParty, const PurCFetcher::SameSiteInfo& sameSiteInfo, const URL& url, const Optional<PurCFetcher::FrameIdentifier>& frameID, const Optional<PurCFetcher::PageIdentifier>& pageID, PurCFetcher::IncludeSecureCookies includeSecureCookies, PurCFetcher::ShouldAskITP shouldAskITP, PurCFetcher::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking)
        : m_arguments(firstParty, sameSiteInfo, url, frameID, pageID, includeSecureCookies, shouldAskITP, shouldRelaxThirdPartyCookieBlocking)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class GetRawCookies {
public:
    using Arguments = std::tuple<const URL&, const PurCFetcher::SameSiteInfo&, const URL&, const Optional<PurCFetcher::FrameIdentifier>&, const Optional<PurCFetcher::PageIdentifier>&, PurCFetcher::ShouldAskITP, PurCFetcher::ShouldRelaxThirdPartyCookieBlocking>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_GetRawCookies; }
    static const bool isSync = true;

    using DelayedReply = GetRawCookiesDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Vector<PurCFetcher::Cookie>& cookies);
    using Reply = std::tuple<Vector<PurCFetcher::Cookie>&>;
    using ReplyArguments = std::tuple<Vector<PurCFetcher::Cookie>>;
    GetRawCookies(const URL& firstParty, const PurCFetcher::SameSiteInfo& sameSiteInfo, const URL& url, const Optional<PurCFetcher::FrameIdentifier>& frameID, const Optional<PurCFetcher::PageIdentifier>& pageID, PurCFetcher::ShouldAskITP shouldAskITP, PurCFetcher::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking)
        : m_arguments(firstParty, sameSiteInfo, url, frameID, pageID, shouldAskITP, shouldRelaxThirdPartyCookieBlocking)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetRawCookie {
public:
    using Arguments = std::tuple<const PurCFetcher::Cookie&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_SetRawCookie; }
    static const bool isSync = false;

    explicit SetRawCookie(const PurCFetcher::Cookie& cookie)
        : m_arguments(cookie)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DeleteCookie {
public:
    using Arguments = std::tuple<const URL&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_DeleteCookie; }
    static const bool isSync = false;

    DeleteCookie(const URL& url, const String& cookieName)
        : m_arguments(url, cookieName)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DomCookiesForHost {
public:
    using Arguments = std::tuple<const String&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_DomCookiesForHost; }
    static const bool isSync = true;

    using DelayedReply = DomCookiesForHostDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Vector<PurCFetcher::Cookie>& cookies);
    using Reply = std::tuple<Vector<PurCFetcher::Cookie>&>;
    using ReplyArguments = std::tuple<Vector<PurCFetcher::Cookie>>;
    DomCookiesForHost(const String& host, bool subscribeToCookieChangeNotifications)
        : m_arguments(host, subscribeToCookieChangeNotifications)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if HAVE(COOKIE_CHANGE_LISTENER_API)
class UnsubscribeFromCookieChangeNotifications {
public:
    using Arguments = std::tuple<const HashSet<String>&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_UnsubscribeFromCookieChangeNotifications; }
    static const bool isSync = false;

    explicit UnsubscribeFromCookieChangeNotifications(const HashSet<String>& hosts)
        : m_arguments(hosts)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

class SetCaptureExtraNetworkLoadMetricsEnabled {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_SetCaptureExtraNetworkLoadMetricsEnabled; }
    static const bool isSync = false;

    explicit SetCaptureExtraNetworkLoadMetricsEnabled(bool enabled)
        : m_arguments(enabled)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class CreateSocketStream {
public:
    using Arguments = std::tuple<const URL&, const String&, const PurCFetcher::WebSocketIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_CreateSocketStream; }
    static const bool isSync = false;

    CreateSocketStream(const URL& url, const String& cachePartition, const PurCFetcher::WebSocketIdentifier& identifier)
        : m_arguments(url, cachePartition, identifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class CreateSocketChannel {
public:
    using Arguments = std::tuple<const PurCFetcher::ResourceRequest&, const String&, const PurCFetcher::WebSocketIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_CreateSocketChannel; }
    static const bool isSync = false;

    CreateSocketChannel(const PurCFetcher::ResourceRequest& request, const String& protocol, const PurCFetcher::WebSocketIdentifier& identifier)
        : m_arguments(request, protocol, identifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class AddOriginAccessWhitelistEntry {
public:
    using Arguments = std::tuple<const String&, const String&, const String&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_AddOriginAccessWhitelistEntry; }
    static const bool isSync = false;

    AddOriginAccessWhitelistEntry(const String& sourceOrigin, const String& destinationProtocol, const String& destinationHost, bool allowDestinationSubdomains)
        : m_arguments(sourceOrigin, destinationProtocol, destinationHost, allowDestinationSubdomains)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RemoveOriginAccessWhitelistEntry {
public:
    using Arguments = std::tuple<const String&, const String&, const String&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_RemoveOriginAccessWhitelistEntry; }
    static const bool isSync = false;

    RemoveOriginAccessWhitelistEntry(const String& sourceOrigin, const String& destinationProtocol, const String& destinationHost, bool allowDestinationSubdomains)
        : m_arguments(sourceOrigin, destinationProtocol, destinationHost, allowDestinationSubdomains)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ResetOriginAccessWhitelists {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_ResetOriginAccessWhitelists; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class GetNetworkLoadInformationResponse {
public:
    using Arguments = std::tuple<uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_GetNetworkLoadInformationResponse; }
    static const bool isSync = true;

    using DelayedReply = GetNetworkLoadInformationResponseDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const PurCFetcher::ResourceResponse& response);
    using Reply = std::tuple<PurCFetcher::ResourceResponse&>;
    using ReplyArguments = std::tuple<PurCFetcher::ResourceResponse>;
    explicit GetNetworkLoadInformationResponse(uint64_t resourceLoadIdentifier)
        : m_arguments(resourceLoadIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class GetNetworkLoadIntermediateInformation {
public:
    using Arguments = std::tuple<uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_GetNetworkLoadIntermediateInformation; }
    static const bool isSync = true;

    using DelayedReply = GetNetworkLoadIntermediateInformationDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Vector<PurCFetcher::NetworkTransactionInformation>& transactions);
    using Reply = std::tuple<Vector<PurCFetcher::NetworkTransactionInformation>&>;
    using ReplyArguments = std::tuple<Vector<PurCFetcher::NetworkTransactionInformation>>;
    explicit GetNetworkLoadIntermediateInformation(uint64_t resourceLoadIdentifier)
        : m_arguments(resourceLoadIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class TakeNetworkLoadInformationMetrics {
public:
    using Arguments = std::tuple<uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_TakeNetworkLoadInformationMetrics; }
    static const bool isSync = true;

    using DelayedReply = TakeNetworkLoadInformationMetricsDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const PurCFetcher::NetworkLoadMetrics& networkMetrics);
    using Reply = std::tuple<PurCFetcher::NetworkLoadMetrics&>;
    using ReplyArguments = std::tuple<PurCFetcher::NetworkLoadMetrics>;
    explicit TakeNetworkLoadInformationMetrics(uint64_t resourceLoadIdentifier)
        : m_arguments(resourceLoadIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if ENABLE(SERVICE_WORKER)
class EstablishSWContextConnection {
public:
    using Arguments = std::tuple<const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_EstablishSWContextConnection; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkConnectionToWebProcess_EstablishSWContextConnectionReply; }
    using AsyncReply = EstablishSWContextConnectionAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit EstablishSWContextConnection(const PurCFetcher::RegistrableDomain& domain)
        : m_arguments(domain)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if ENABLE(SERVICE_WORKER)
class CloseSWContextConnection {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_CloseSWContextConnection; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

class UpdateQuotaBasedOnSpaceUsageForTesting {
public:
    using Arguments = std::tuple<const PurCFetcher::ClientOrigin&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_UpdateQuotaBasedOnSpaceUsageForTesting; }
    static const bool isSync = false;

    explicit UpdateQuotaBasedOnSpaceUsageForTesting(const PurCFetcher::ClientOrigin& origin)
        : m_arguments(origin)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class CreateNewMessagePortChannel {
public:
    using Arguments = std::tuple<const PurCFetcher::MessagePortIdentifier&, const PurCFetcher::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_CreateNewMessagePortChannel; }
    static const bool isSync = false;

    CreateNewMessagePortChannel(const PurCFetcher::MessagePortIdentifier& port1, const PurCFetcher::MessagePortIdentifier& port2)
        : m_arguments(port1, port2)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class EntangleLocalPortInThisProcessToRemote {
public:
    using Arguments = std::tuple<const PurCFetcher::MessagePortIdentifier&, const PurCFetcher::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_EntangleLocalPortInThisProcessToRemote; }
    static const bool isSync = false;

    EntangleLocalPortInThisProcessToRemote(const PurCFetcher::MessagePortIdentifier& local, const PurCFetcher::MessagePortIdentifier& remote)
        : m_arguments(local, remote)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class MessagePortDisentangled {
public:
    using Arguments = std::tuple<const PurCFetcher::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_MessagePortDisentangled; }
    static const bool isSync = false;

    explicit MessagePortDisentangled(const PurCFetcher::MessagePortIdentifier& local)
        : m_arguments(local)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class MessagePortClosed {
public:
    using Arguments = std::tuple<const PurCFetcher::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_MessagePortClosed; }
    static const bool isSync = false;

    explicit MessagePortClosed(const PurCFetcher::MessagePortIdentifier& local)
        : m_arguments(local)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class TakeAllMessagesForPort {
public:
    using Arguments = std::tuple<const PurCFetcher::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_TakeAllMessagesForPort; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(Vector<PurCFetcher::MessageWithMessagePorts>&&, uint64_t&&)>&&);
    static void cancelReply(CompletionHandler<void(Vector<PurCFetcher::MessageWithMessagePorts>&&, uint64_t&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkConnectionToWebProcess_TakeAllMessagesForPortReply; }
    using AsyncReply = TakeAllMessagesForPortAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Vector<PurCFetcher::MessageWithMessagePorts>& messages, uint64_t messageBatchIdentifier);
    using Reply = std::tuple<Vector<PurCFetcher::MessageWithMessagePorts>&, uint64_t&>;
    using ReplyArguments = std::tuple<Vector<PurCFetcher::MessageWithMessagePorts>, uint64_t>;
    explicit TakeAllMessagesForPort(const PurCFetcher::MessagePortIdentifier& port)
        : m_arguments(port)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class PostMessageToRemote {
public:
    using Arguments = std::tuple<const PurCFetcher::MessageWithMessagePorts&, const PurCFetcher::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_PostMessageToRemote; }
    static const bool isSync = false;

    PostMessageToRemote(const PurCFetcher::MessageWithMessagePorts& message, const PurCFetcher::MessagePortIdentifier& remote)
        : m_arguments(message, remote)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class CheckRemotePortForActivity {
public:
    using Arguments = std::tuple<const PurCFetcher::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_CheckRemotePortForActivity; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkConnectionToWebProcess_CheckRemotePortForActivityReply; }
    using AsyncReply = CheckRemotePortForActivityAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool hasActivity);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    explicit CheckRemotePortForActivity(const PurCFetcher::MessagePortIdentifier& port)
        : m_arguments(port)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidDeliverMessagePortMessages {
public:
    using Arguments = std::tuple<uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_DidDeliverMessagePortMessages; }
    static const bool isSync = false;

    explicit DidDeliverMessagePortMessages(uint64_t messageBatchIdentifier)
        : m_arguments(messageBatchIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterURLSchemesAsCORSEnabled {
public:
    using Arguments = std::tuple<const Vector<String>&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_RegisterURLSchemesAsCORSEnabled; }
    static const bool isSync = false;

    explicit RegisterURLSchemesAsCORSEnabled(const Vector<String>& schemes)
        : m_arguments(schemes)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

} // namespace NetworkConnectionToWebProcess
} // namespace Messages
