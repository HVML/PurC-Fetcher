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
#include "MessageNames.h"
#include "NavigatingToAppBoundDomain.h"
#include "NetworkConnectionToWebProcessMessagesReplies.h"
#include "SandboxExtension.h"
#include "WebPageProxyIdentifier.h"
#include "WebSocketIdentifier.h"
#include <WebCore/BlobPart.h>
#include <WebCore/Cookie.h>
#include <WebCore/FrameIdentifier.h>
#include <WebCore/MessageWithMessagePorts.h>
#include <WebCore/NetworkLoadInformation.h>
#include <WebCore/PageIdentifier.h>
#include <WebCore/ResourceLoadStatistics.h>
#include <wtf/Forward.h>
#include <wtf/HashSet.h>
#include <wtf/Optional.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace WebCore {
class NetworkLoadMetrics;
class RegistrableDomain;
class ResourceError;
class ResourceRequest;
class ResourceResponse;
enum class IncludeSecureCookies : bool;
enum class ShouldAskITP : bool;
enum class ShouldRelaxThirdPartyCookieBlocking : bool;
enum class StorageAccessScope : bool;
struct ClientOrigin;
struct Cookie;
struct MessagePortIdentifier;
struct MessageWithMessagePorts;
struct RequestStorageAccessResult;
struct SameSiteInfo;
}

namespace WebKit {
class DownloadID;
class NetworkResourceLoadParameters;
}

namespace Messages {
namespace NetworkConnectionToWebProcess {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::NetworkConnectionToWebProcess;
}

class ScheduleResourceLoad {
public:
    using Arguments = std::tuple<const WebKit::NetworkResourceLoadParameters&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_ScheduleResourceLoad; }
    static const bool isSync = false;

    explicit ScheduleResourceLoad(const WebKit::NetworkResourceLoadParameters& resourceLoadParameters)
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
    using Arguments = std::tuple<const WebKit::NetworkResourceLoadParameters&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_PerformSynchronousLoad; }
    static const bool isSync = true;

    using DelayedReply = PerformSynchronousLoadDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const WebCore::ResourceError& error, const WebCore::ResourceResponse& response, const Vector<char>& data);
    using Reply = std::tuple<WebCore::ResourceError&, WebCore::ResourceResponse&, Vector<char>&>;
    using ReplyArguments = std::tuple<WebCore::ResourceError, WebCore::ResourceResponse, Vector<char>>;
    explicit PerformSynchronousLoad(const WebKit::NetworkResourceLoadParameters& resourceLoadParameters)
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
    using Arguments = std::tuple<const WebKit::WebPageProxyIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_TestProcessIncomingSyncMessagesWhenWaitingForSyncReply; }
    static const bool isSync = true;

    using DelayedReply = TestProcessIncomingSyncMessagesWhenWaitingForSyncReplyDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool handled);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    explicit TestProcessIncomingSyncMessagesWhenWaitingForSyncReply(const WebKit::WebPageProxyIdentifier& pageID)
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
    using Arguments = std::tuple<const WebKit::NetworkResourceLoadParameters&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_LoadPing; }
    static const bool isSync = false;

    explicit LoadPing(const WebKit::NetworkResourceLoadParameters& resourceLoadParameters)
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
    using Arguments = std::tuple<const WebCore::PageIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_PageLoadCompleted; }
    static const bool isSync = false;

    explicit PageLoadCompleted(const WebCore::PageIdentifier& webPageID)
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
    using Arguments = std::tuple<const WebKit::WebPageProxyIdentifier&, const WebCore::PageIdentifier&, const WebCore::FrameIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_BrowsingContextRemoved; }
    static const bool isSync = false;

    BrowsingContextRemoved(const WebKit::WebPageProxyIdentifier& webPageProxyID, const WebCore::PageIdentifier& webPageID, const WebCore::FrameIdentifier& webFrameID)
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
    using Arguments = std::tuple<const Optional<uint64_t>&, const WebKit::NetworkResourceLoadParameters&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_PreconnectTo; }
    static const bool isSync = false;

    PreconnectTo(const Optional<uint64_t>& preconnectionIdentifier, const WebKit::NetworkResourceLoadParameters& loadParameters)
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
    using Arguments = std::tuple<const WebKit::DownloadID&, const WebCore::ResourceRequest&, Optional<WebKit::NavigatingToAppBoundDomain>, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_StartDownload; }
    static const bool isSync = false;

    StartDownload(const WebKit::DownloadID& downloadID, const WebCore::ResourceRequest& request, Optional<WebKit::NavigatingToAppBoundDomain> isNavigatingToAppBoundDomain, const String& suggestedName)
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
    using Arguments = std::tuple<uint64_t, const WebKit::DownloadID&, const WebCore::ResourceRequest&, const WebCore::ResourceResponse&, Optional<WebKit::NavigatingToAppBoundDomain>>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_ConvertMainResourceLoadToDownload; }
    static const bool isSync = false;

    ConvertMainResourceLoadToDownload(uint64_t mainResourceLoadIdentifier, const WebKit::DownloadID& downloadID, const WebCore::ResourceRequest& request, const WebCore::ResourceResponse& response, Optional<WebKit::NavigatingToAppBoundDomain> isNavigatingToAppBoundDomain)
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
    using Arguments = std::tuple<const URL&, const WebCore::SameSiteInfo&, const URL&, const WebCore::FrameIdentifier&, const WebCore::PageIdentifier&, WebCore::IncludeSecureCookies, WebCore::ShouldAskITP, WebCore::ShouldRelaxThirdPartyCookieBlocking>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_CookiesForDOM; }
    static const bool isSync = true;

    using DelayedReply = CookiesForDOMDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const String& cookieString, bool didAccessSecureCookies);
    using Reply = std::tuple<String&, bool&>;
    using ReplyArguments = std::tuple<String, bool>;
    CookiesForDOM(const URL& firstParty, const WebCore::SameSiteInfo& sameSiteInfo, const URL& url, const WebCore::FrameIdentifier& frameID, const WebCore::PageIdentifier& pageID, WebCore::IncludeSecureCookies includeSecureCookies, WebCore::ShouldAskITP shouldAskITP, WebCore::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking)
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
    using Arguments = std::tuple<const URL&, const WebCore::SameSiteInfo&, const URL&, const WebCore::FrameIdentifier&, const WebCore::PageIdentifier&, WebCore::ShouldAskITP, const String&, WebCore::ShouldRelaxThirdPartyCookieBlocking>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_SetCookiesFromDOM; }
    static const bool isSync = false;

    SetCookiesFromDOM(const URL& firstParty, const WebCore::SameSiteInfo& sameSiteInfo, const URL& url, const WebCore::FrameIdentifier& frameID, const WebCore::PageIdentifier& pageID, WebCore::ShouldAskITP shouldAskITP, const String& cookieString, WebCore::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking)
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
    using Arguments = std::tuple<const URL&, const WebCore::SameSiteInfo&, const URL&, const Optional<WebCore::FrameIdentifier>&, const Optional<WebCore::PageIdentifier>&, WebCore::IncludeSecureCookies, WebCore::ShouldAskITP, WebCore::ShouldRelaxThirdPartyCookieBlocking>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_CookieRequestHeaderFieldValue; }
    static const bool isSync = true;

    using DelayedReply = CookieRequestHeaderFieldValueDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const String& cookieString, bool didAccessSecureCookies);
    using Reply = std::tuple<String&, bool&>;
    using ReplyArguments = std::tuple<String, bool>;
    CookieRequestHeaderFieldValue(const URL& firstParty, const WebCore::SameSiteInfo& sameSiteInfo, const URL& url, const Optional<WebCore::FrameIdentifier>& frameID, const Optional<WebCore::PageIdentifier>& pageID, WebCore::IncludeSecureCookies includeSecureCookies, WebCore::ShouldAskITP shouldAskITP, WebCore::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking)
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
    using Arguments = std::tuple<const URL&, const WebCore::SameSiteInfo&, const URL&, const Optional<WebCore::FrameIdentifier>&, const Optional<WebCore::PageIdentifier>&, WebCore::ShouldAskITP, WebCore::ShouldRelaxThirdPartyCookieBlocking>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_GetRawCookies; }
    static const bool isSync = true;

    using DelayedReply = GetRawCookiesDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Vector<WebCore::Cookie>& cookies);
    using Reply = std::tuple<Vector<WebCore::Cookie>&>;
    using ReplyArguments = std::tuple<Vector<WebCore::Cookie>>;
    GetRawCookies(const URL& firstParty, const WebCore::SameSiteInfo& sameSiteInfo, const URL& url, const Optional<WebCore::FrameIdentifier>& frameID, const Optional<WebCore::PageIdentifier>& pageID, WebCore::ShouldAskITP shouldAskITP, WebCore::ShouldRelaxThirdPartyCookieBlocking shouldRelaxThirdPartyCookieBlocking)
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
    using Arguments = std::tuple<const WebCore::Cookie&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_SetRawCookie; }
    static const bool isSync = false;

    explicit SetRawCookie(const WebCore::Cookie& cookie)
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
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Vector<WebCore::Cookie>& cookies);
    using Reply = std::tuple<Vector<WebCore::Cookie>&>;
    using ReplyArguments = std::tuple<Vector<WebCore::Cookie>>;
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

class RegisterFileBlobURL {
public:
    using Arguments = std::tuple<const URL&, const String&, const WebKit::SandboxExtension::Handle&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_RegisterFileBlobURL; }
    static const bool isSync = false;

    RegisterFileBlobURL(const URL& url, const String& path, const WebKit::SandboxExtension::Handle& extensionHandle, const String& contentType)
        : m_arguments(url, path, extensionHandle, contentType)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterBlobURL {
public:
    using Arguments = std::tuple<const URL&, const Vector<WebCore::BlobPart>&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_RegisterBlobURL; }
    static const bool isSync = false;

    RegisterBlobURL(const URL& url, const Vector<WebCore::BlobPart>& blobParts, const String& contentType)
        : m_arguments(url, blobParts, contentType)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterBlobURLFromURL {
public:
    using Arguments = std::tuple<const URL&, const URL&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_RegisterBlobURLFromURL; }
    static const bool isSync = false;

    RegisterBlobURLFromURL(const URL& url, const URL& srcURL)
        : m_arguments(url, srcURL)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterBlobURLOptionallyFileBacked {
public:
    using Arguments = std::tuple<const URL&, const URL&, const String&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_RegisterBlobURLOptionallyFileBacked; }
    static const bool isSync = false;

    RegisterBlobURLOptionallyFileBacked(const URL& url, const URL& srcURL, const String& fileBackedPath, const String& contentType)
        : m_arguments(url, srcURL, fileBackedPath, contentType)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterBlobURLForSlice {
public:
    using Arguments = std::tuple<const URL&, const URL&, int64_t, int64_t>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_RegisterBlobURLForSlice; }
    static const bool isSync = false;

    RegisterBlobURLForSlice(const URL& url, const URL& srcURL, int64_t start, int64_t end)
        : m_arguments(url, srcURL, start, end)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class UnregisterBlobURL {
public:
    using Arguments = std::tuple<const URL&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_UnregisterBlobURL; }
    static const bool isSync = false;

    explicit UnregisterBlobURL(const URL& url)
        : m_arguments(url)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class BlobSize {
public:
    using Arguments = std::tuple<const URL&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_BlobSize; }
    static const bool isSync = true;

    using DelayedReply = BlobSizeDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, uint64_t resultSize);
    using Reply = std::tuple<uint64_t&>;
    using ReplyArguments = std::tuple<uint64_t>;
    explicit BlobSize(const URL& url)
        : m_arguments(url)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class WriteBlobsToTemporaryFiles {
public:
    using Arguments = std::tuple<const Vector<String>&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_WriteBlobsToTemporaryFiles; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(Vector<String>&&)>&&);
    static void cancelReply(CompletionHandler<void(Vector<String>&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkConnectionToWebProcess_WriteBlobsToTemporaryFilesReply; }
    using AsyncReply = WriteBlobsToTemporaryFilesAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Vector<String>& fileNames);
    using Reply = std::tuple<Vector<String>&>;
    using ReplyArguments = std::tuple<Vector<String>>;
    explicit WriteBlobsToTemporaryFiles(const Vector<String>& blobURLs)
        : m_arguments(blobURLs)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

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
    using Arguments = std::tuple<const URL&, const String&, const WebKit::WebSocketIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_CreateSocketStream; }
    static const bool isSync = false;

    CreateSocketStream(const URL& url, const String& cachePartition, const WebKit::WebSocketIdentifier& identifier)
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
    using Arguments = std::tuple<const WebCore::ResourceRequest&, const String&, const WebKit::WebSocketIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_CreateSocketChannel; }
    static const bool isSync = false;

    CreateSocketChannel(const WebCore::ResourceRequest& request, const String& protocol, const WebKit::WebSocketIdentifier& identifier)
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

#if ENABLE(RESOURCE_LOAD_STATISTICS)
class RemoveStorageAccessForFrame {
public:
    using Arguments = std::tuple<const WebCore::FrameIdentifier&, const WebCore::PageIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_RemoveStorageAccessForFrame; }
    static const bool isSync = false;

    RemoveStorageAccessForFrame(const WebCore::FrameIdentifier& frameID, const WebCore::PageIdentifier& pageID)
        : m_arguments(frameID, pageID)
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

#if ENABLE(RESOURCE_LOAD_STATISTICS)
class ClearPageSpecificDataForResourceLoadStatistics {
public:
    using Arguments = std::tuple<const WebCore::PageIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_ClearPageSpecificDataForResourceLoadStatistics; }
    static const bool isSync = false;

    explicit ClearPageSpecificDataForResourceLoadStatistics(const WebCore::PageIdentifier& pageID)
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
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
class LogUserInteraction {
public:
    using Arguments = std::tuple<const WebCore::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_LogUserInteraction; }
    static const bool isSync = false;

    explicit LogUserInteraction(const WebCore::RegistrableDomain& domain)
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

#if ENABLE(RESOURCE_LOAD_STATISTICS)
class ResourceLoadStatisticsUpdated {
public:
    using Arguments = std::tuple<const Vector<WebCore::ResourceLoadStatistics>&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_ResourceLoadStatisticsUpdated; }
    static const bool isSync = false;

    explicit ResourceLoadStatisticsUpdated(const Vector<WebCore::ResourceLoadStatistics>& statistics)
        : m_arguments(statistics)
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

#if ENABLE(RESOURCE_LOAD_STATISTICS)
class HasStorageAccess {
public:
    using Arguments = std::tuple<const WebCore::RegistrableDomain&, const WebCore::RegistrableDomain&, const WebCore::FrameIdentifier&, const WebCore::PageIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_HasStorageAccess; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkConnectionToWebProcess_HasStorageAccessReply; }
    using AsyncReply = HasStorageAccessAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool hasStorageAccess);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    HasStorageAccess(const WebCore::RegistrableDomain& subFrameDomain, const WebCore::RegistrableDomain& topFrameDomain, const WebCore::FrameIdentifier& frameID, const WebCore::PageIdentifier& pageID)
        : m_arguments(subFrameDomain, topFrameDomain, frameID, pageID)
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

#if ENABLE(RESOURCE_LOAD_STATISTICS)
class RequestStorageAccess {
public:
    using Arguments = std::tuple<const WebCore::RegistrableDomain&, const WebCore::RegistrableDomain&, const WebCore::FrameIdentifier&, const WebCore::PageIdentifier&, const WebKit::WebPageProxyIdentifier&, WebCore::StorageAccessScope>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_RequestStorageAccess; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(WebCore::RequestStorageAccessResult&&)>&&);
    static void cancelReply(CompletionHandler<void(WebCore::RequestStorageAccessResult&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkConnectionToWebProcess_RequestStorageAccessReply; }
    using AsyncReply = RequestStorageAccessAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const WebCore::RequestStorageAccessResult& result);
    using Reply = std::tuple<WebCore::RequestStorageAccessResult&>;
    using ReplyArguments = std::tuple<WebCore::RequestStorageAccessResult>;
    RequestStorageAccess(const WebCore::RegistrableDomain& subFrameDomain, const WebCore::RegistrableDomain& topFrameDomain, const WebCore::FrameIdentifier& frameID, const WebCore::PageIdentifier& webPageID, const WebKit::WebPageProxyIdentifier& webPageProxyID, WebCore::StorageAccessScope scope)
        : m_arguments(subFrameDomain, topFrameDomain, frameID, webPageID, webPageProxyID, scope)
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

#if ENABLE(RESOURCE_LOAD_STATISTICS)
class RequestStorageAccessUnderOpener {
public:
    using Arguments = std::tuple<const WebCore::RegistrableDomain&, const WebCore::PageIdentifier&, const WebCore::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_RequestStorageAccessUnderOpener; }
    static const bool isSync = false;

    RequestStorageAccessUnderOpener(const WebCore::RegistrableDomain& domainInNeedOfStorageAccess, const WebCore::PageIdentifier& openerPageID, const WebCore::RegistrableDomain& openerDomain)
        : m_arguments(domainInNeedOfStorageAccess, openerPageID, openerDomain)
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
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const WebCore::ResourceResponse& response);
    using Reply = std::tuple<WebCore::ResourceResponse&>;
    using ReplyArguments = std::tuple<WebCore::ResourceResponse>;
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
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Vector<WebCore::NetworkTransactionInformation>& transactions);
    using Reply = std::tuple<Vector<WebCore::NetworkTransactionInformation>&>;
    using ReplyArguments = std::tuple<Vector<WebCore::NetworkTransactionInformation>>;
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
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const WebCore::NetworkLoadMetrics& networkMetrics);
    using Reply = std::tuple<WebCore::NetworkLoadMetrics&>;
    using ReplyArguments = std::tuple<WebCore::NetworkLoadMetrics>;
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
    using Arguments = std::tuple<const WebCore::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_EstablishSWContextConnection; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkConnectionToWebProcess_EstablishSWContextConnectionReply; }
    using AsyncReply = EstablishSWContextConnectionAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit EstablishSWContextConnection(const WebCore::RegistrableDomain& domain)
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
    using Arguments = std::tuple<const WebCore::ClientOrigin&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_UpdateQuotaBasedOnSpaceUsageForTesting; }
    static const bool isSync = false;

    explicit UpdateQuotaBasedOnSpaceUsageForTesting(const WebCore::ClientOrigin& origin)
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
    using Arguments = std::tuple<const WebCore::MessagePortIdentifier&, const WebCore::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_CreateNewMessagePortChannel; }
    static const bool isSync = false;

    CreateNewMessagePortChannel(const WebCore::MessagePortIdentifier& port1, const WebCore::MessagePortIdentifier& port2)
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
    using Arguments = std::tuple<const WebCore::MessagePortIdentifier&, const WebCore::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_EntangleLocalPortInThisProcessToRemote; }
    static const bool isSync = false;

    EntangleLocalPortInThisProcessToRemote(const WebCore::MessagePortIdentifier& local, const WebCore::MessagePortIdentifier& remote)
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
    using Arguments = std::tuple<const WebCore::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_MessagePortDisentangled; }
    static const bool isSync = false;

    explicit MessagePortDisentangled(const WebCore::MessagePortIdentifier& local)
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
    using Arguments = std::tuple<const WebCore::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_MessagePortClosed; }
    static const bool isSync = false;

    explicit MessagePortClosed(const WebCore::MessagePortIdentifier& local)
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
    using Arguments = std::tuple<const WebCore::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_TakeAllMessagesForPort; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(Vector<WebCore::MessageWithMessagePorts>&&, uint64_t&&)>&&);
    static void cancelReply(CompletionHandler<void(Vector<WebCore::MessageWithMessagePorts>&&, uint64_t&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkConnectionToWebProcess_TakeAllMessagesForPortReply; }
    using AsyncReply = TakeAllMessagesForPortAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Vector<WebCore::MessageWithMessagePorts>& messages, uint64_t messageBatchIdentifier);
    using Reply = std::tuple<Vector<WebCore::MessageWithMessagePorts>&, uint64_t&>;
    using ReplyArguments = std::tuple<Vector<WebCore::MessageWithMessagePorts>, uint64_t>;
    explicit TakeAllMessagesForPort(const WebCore::MessagePortIdentifier& port)
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
    using Arguments = std::tuple<const WebCore::MessageWithMessagePorts&, const WebCore::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_PostMessageToRemote; }
    static const bool isSync = false;

    PostMessageToRemote(const WebCore::MessageWithMessagePorts& message, const WebCore::MessagePortIdentifier& remote)
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
    using Arguments = std::tuple<const WebCore::MessagePortIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkConnectionToWebProcess_CheckRemotePortForActivity; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkConnectionToWebProcess_CheckRemotePortForActivityReply; }
    using AsyncReply = CheckRemotePortForActivityAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool hasActivity);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    explicit CheckRemotePortForActivity(const WebCore::MessagePortIdentifier& port)
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
