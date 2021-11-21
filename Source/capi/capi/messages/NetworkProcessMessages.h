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
#include "Attachment.h"
#include "Connection.h"
#include "CrossSiteNavigationDataTransfer.h"
#include "LocalStorageDatabaseTracker.h"
#include "MessageNames.h"
#include "NavigatingToAppBoundDomain.h"
#include "NetworkProcessMessagesReplies.h"
#include "PageIdentifier.h"
#include "RegistrableDomain.h"
#include "SandboxExtension.h"
#include "SecurityOriginData.h"
#include "WebPageProxyIdentifier.h"
#include "WebsiteDataFetchOption.h"
#include "WebsiteDataType.h"
#include <ProcessIdentifier.h>
#include <wtf/Forward.h>
#include <wtf/HashSet.h>
#include <wtf/OptionSet.h>
#include <wtf/Optional.h>
#include <wtf/Seconds.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/Vector.h>
#include <wtf/WallTime.h>
#include <wtf/text/WTFString.h>

namespace IPC {
class DataReference;
}

namespace PAL {
class SessionID;
}

namespace PurCFetcher {
class CallbackID;
class CertificateInfo;
class CurlProxySettings;
class DownloadID;
class RegistrableDomain;
class ResourceRequest;
enum class SameSiteStrictEnforcementEnabled : bool;
enum class ShouldGrandfatherStatistics : bool;
enum class CacheModel : uint8_t;
enum class FirstPartyWebsiteDataRemovalMode : uint8_t;
enum class HTTPCookieAcceptPolicy : uint8_t;
enum class StoredCredentialsPolicy : uint8_t;
enum class ThirdPartyCookieBlockingMode : uint8_t;
struct NetworkProcessCreationParameters;
struct SoupNetworkProxySettings;
struct WebsiteDataStoreParameters;
}

namespace Messages {
namespace NetworkProcess {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::NetworkProcess;
}

class InitializeNetworkProcess {
public:
    using Arguments = std::tuple<const PurCFetcher::NetworkProcessCreationParameters&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_InitializeNetworkProcess; }
    static const bool isSync = false;

    explicit InitializeNetworkProcess(const PurCFetcher::NetworkProcessCreationParameters& processCreationParameters)
        : m_arguments(processCreationParameters)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class CreateNetworkConnectionToWebProcess {
public:
    using Arguments = std::tuple<const PurCFetcher::ProcessIdentifier&, const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_CreateNetworkConnectionToWebProcess; }
    static const bool isSync = true;

    using DelayedReply = CreateNetworkConnectionToWebProcessDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Optional<IPC::Attachment>& connectionIdentifier, PurCFetcher::HTTPCookieAcceptPolicy cookieAcceptPolicy);
    using Reply = std::tuple<Optional<IPC::Attachment>&, PurCFetcher::HTTPCookieAcceptPolicy&>;
    using ReplyArguments = std::tuple<Optional<IPC::Attachment>, PurCFetcher::HTTPCookieAcceptPolicy>;
    CreateNetworkConnectionToWebProcess(const PurCFetcher::ProcessIdentifier& processIdentifier, const PAL::SessionID& sessionID)
        : m_arguments(processIdentifier, sessionID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if USE(SOUP)
class SetIgnoreTLSErrors {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetIgnoreTLSErrors; }
    static const bool isSync = false;

    explicit SetIgnoreTLSErrors(bool ignoreTLSErrors)
        : m_arguments(ignoreTLSErrors)
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

#if USE(SOUP)
class UserPreferredLanguagesChanged {
public:
    using Arguments = std::tuple<const Vector<String>&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_UserPreferredLanguagesChanged; }
    static const bool isSync = false;

    explicit UserPreferredLanguagesChanged(const Vector<String>& languages)
        : m_arguments(languages)
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

#if USE(SOUP)
class SetNetworkProxySettings {
public:
    using Arguments = std::tuple<const PurCFetcher::SoupNetworkProxySettings&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetNetworkProxySettings; }
    static const bool isSync = false;

    explicit SetNetworkProxySettings(const PurCFetcher::SoupNetworkProxySettings& settings)
        : m_arguments(settings)
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

#if USE(SOUP)
class PrefetchDNS {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_PrefetchDNS; }
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
#endif

#if USE(CURL)
class SetNetworkProxySettings {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::CurlProxySettings&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetNetworkProxySettings; }
    static const bool isSync = false;

    SetNetworkProxySettings(const PAL::SessionID& sessionID, const PurCFetcher::CurlProxySettings& settings)
        : m_arguments(sessionID, settings)
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

class ClearCachedCredentials {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ClearCachedCredentials; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class AddWebsiteDataStore {
public:
    using Arguments = std::tuple<const PurCFetcher::WebsiteDataStoreParameters&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_AddWebsiteDataStore; }
    static const bool isSync = false;

    explicit AddWebsiteDataStore(const PurCFetcher::WebsiteDataStoreParameters& websiteDataStoreParameters)
        : m_arguments(websiteDataStoreParameters)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DestroySession {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_DestroySession; }
    static const bool isSync = false;

    explicit DestroySession(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class FetchWebsiteData {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const OptionSet<PurCFetcher::WebsiteDataType>&, const OptionSet<PurCFetcher::WebsiteDataFetchOption>&, const PurCFetcher::CallbackID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_FetchWebsiteData; }
    static const bool isSync = false;

    FetchWebsiteData(const PAL::SessionID& sessionID, const OptionSet<PurCFetcher::WebsiteDataType>& websiteDataTypes, const OptionSet<PurCFetcher::WebsiteDataFetchOption>& fetchOptions, const PurCFetcher::CallbackID& callbackID)
        : m_arguments(sessionID, websiteDataTypes, fetchOptions, callbackID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DeleteWebsiteData {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const OptionSet<PurCFetcher::WebsiteDataType>&, const WallTime&, const PurCFetcher::CallbackID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_DeleteWebsiteData; }
    static const bool isSync = false;

    DeleteWebsiteData(const PAL::SessionID& sessionID, const OptionSet<PurCFetcher::WebsiteDataType>& websiteDataTypes, const WallTime& modifiedSince, const PurCFetcher::CallbackID& callbackID)
        : m_arguments(sessionID, websiteDataTypes, modifiedSince, callbackID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DeleteWebsiteDataForOrigins {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const OptionSet<PurCFetcher::WebsiteDataType>&, const Vector<PurCFetcher::SecurityOriginData>&, const Vector<String>&, const Vector<String>&, const Vector<PurCFetcher::RegistrableDomain>&, const PurCFetcher::CallbackID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_DeleteWebsiteDataForOrigins; }
    static const bool isSync = false;

    DeleteWebsiteDataForOrigins(const PAL::SessionID& sessionID, const OptionSet<PurCFetcher::WebsiteDataType>& websiteDataTypes, const Vector<PurCFetcher::SecurityOriginData>& origins, const Vector<String>& cookieHostNames, const Vector<String>& HSTSCacheHostNames, const Vector<PurCFetcher::RegistrableDomain>& registrableDomains, const PurCFetcher::CallbackID& callbackID)
        : m_arguments(sessionID, websiteDataTypes, origins, cookieHostNames, HSTSCacheHostNames, registrableDomains, callbackID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RenameOriginInWebsiteData {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const URL&, const URL&, const OptionSet<PurCFetcher::WebsiteDataType>&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_RenameOriginInWebsiteData; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_RenameOriginInWebsiteDataReply; }
    using AsyncReply = RenameOriginInWebsiteDataAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    RenameOriginInWebsiteData(const PAL::SessionID& sessionID, const URL& oldDomain, const URL& newDomain, const OptionSet<PurCFetcher::WebsiteDataType>& websiteDataTypes)
        : m_arguments(sessionID, oldDomain, newDomain, websiteDataTypes)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DownloadRequest {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::DownloadID&, const PurCFetcher::ResourceRequest&, Optional<PurCFetcher::NavigatingToAppBoundDomain>, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_DownloadRequest; }
    static const bool isSync = false;

    DownloadRequest(const PAL::SessionID& sessionID, const PurCFetcher::DownloadID& downloadID, const PurCFetcher::ResourceRequest& request, Optional<PurCFetcher::NavigatingToAppBoundDomain> isNavigatingToAppBoundDomain, const String& suggestedFilename)
        : m_arguments(sessionID, downloadID, request, isNavigatingToAppBoundDomain, suggestedFilename)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ResumeDownload {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::DownloadID&, const IPC::DataReference&, const String&, const PurCFetcher::SandboxExtension::Handle&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ResumeDownload; }
    static const bool isSync = false;

    ResumeDownload(const PAL::SessionID& sessionID, const PurCFetcher::DownloadID& downloadID, const IPC::DataReference& resumeData, const String& path, const PurCFetcher::SandboxExtension::Handle& sandboxExtensionHandle)
        : m_arguments(sessionID, downloadID, resumeData, path, sandboxExtensionHandle)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class CancelDownload {
public:
    using Arguments = std::tuple<const PurCFetcher::DownloadID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_CancelDownload; }
    static const bool isSync = false;

    explicit CancelDownload(const PurCFetcher::DownloadID& downloadID)
        : m_arguments(downloadID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if PLATFORM(COCOA)
class PublishDownloadProgress {
public:
    using Arguments = std::tuple<const PurCFetcher::DownloadID&, const URL&, const PurCFetcher::SandboxExtension::Handle&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_PublishDownloadProgress; }
    static const bool isSync = false;

    PublishDownloadProgress(const PurCFetcher::DownloadID& downloadID, const URL& url, const PurCFetcher::SandboxExtension::Handle& sandboxExtensionHandle)
        : m_arguments(downloadID, url, sandboxExtensionHandle)
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

class ApplicationDidEnterBackground {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ApplicationDidEnterBackground; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ApplicationWillEnterForeground {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ApplicationWillEnterForeground; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ContinueWillSendRequest {
public:
    using Arguments = std::tuple<const PurCFetcher::DownloadID&, const PurCFetcher::ResourceRequest&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ContinueWillSendRequest; }
    static const bool isSync = false;

    ContinueWillSendRequest(const PurCFetcher::DownloadID& downloadID, const PurCFetcher::ResourceRequest& request)
        : m_arguments(downloadID, request)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ContinueDecidePendingDownloadDestination {
public:
    using Arguments = std::tuple<const PurCFetcher::DownloadID&, const String&, const PurCFetcher::SandboxExtension::Handle&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ContinueDecidePendingDownloadDestination; }
    static const bool isSync = false;

    ContinueDecidePendingDownloadDestination(const PurCFetcher::DownloadID& downloadID, const String& destination, const PurCFetcher::SandboxExtension::Handle& sandboxExtensionHandle, bool allowOverwrite)
        : m_arguments(downloadID, destination, sandboxExtensionHandle, allowOverwrite)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if PLATFORM(COCOA)
class SetQOS {
public:
    using Arguments = std::tuple<const int&, const int&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetQOS; }
    static const bool isSync = false;

    SetQOS(const int& latencyQOS, const int& throughputQOS)
        : m_arguments(latencyQOS, throughputQOS)
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

#if PLATFORM(COCOA)
class SetStorageAccessAPIEnabled {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetStorageAccessAPIEnabled; }
    static const bool isSync = false;

    explicit SetStorageAccessAPIEnabled(bool enabled)
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
#endif

class SetAllowsAnySSLCertificateForWebSocket {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetAllowsAnySSLCertificateForWebSocket; }
    static const bool isSync = true;

    using DelayedReply = SetAllowsAnySSLCertificateForWebSocketDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit SetAllowsAnySSLCertificateForWebSocket(bool enabled)
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

class SyncAllCookies {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SyncAllCookies; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class AllowSpecificHTTPSCertificateForHost {
public:
    using Arguments = std::tuple<const PurCFetcher::CertificateInfo&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_AllowSpecificHTTPSCertificateForHost; }
    static const bool isSync = false;

    AllowSpecificHTTPSCertificateForHost(const PurCFetcher::CertificateInfo& certificate, const String& host)
        : m_arguments(certificate, host)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetCacheModel {
public:
    using Arguments = std::tuple<PurCFetcher::CacheModel>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetCacheModel; }
    static const bool isSync = false;

    explicit SetCacheModel(PurCFetcher::CacheModel cacheModel)
        : m_arguments(cacheModel)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetCacheModelSynchronouslyForTesting {
public:
    using Arguments = std::tuple<PurCFetcher::CacheModel>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetCacheModelSynchronouslyForTesting; }
    static const bool isSync = true;

    using DelayedReply = SetCacheModelSynchronouslyForTestingDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit SetCacheModelSynchronouslyForTesting(PurCFetcher::CacheModel cacheModel)
        : m_arguments(cacheModel)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ProcessDidTransitionToBackground {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ProcessDidTransitionToBackground; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ProcessDidTransitionToForeground {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ProcessDidTransitionToForeground; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ProcessWillSuspendImminentlyForTestingSync {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ProcessWillSuspendImminentlyForTestingSync; }
    static const bool isSync = true;

    using DelayedReply = ProcessWillSuspendImminentlyForTestingSyncDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class PrepareToSuspend {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_PrepareToSuspend; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_PrepareToSuspendReply; }
    using AsyncReply = PrepareToSuspendAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit PrepareToSuspend(bool isSuspensionImminent)
        : m_arguments(isSuspensionImminent)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ProcessDidResume {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ProcessDidResume; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class PreconnectTo {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::WebPageProxyIdentifier&, const PurCFetcher::PageIdentifier&, const URL&, const String&, PurCFetcher::StoredCredentialsPolicy, Optional<PurCFetcher::NavigatingToAppBoundDomain>>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_PreconnectTo; }
    static const bool isSync = false;

    PreconnectTo(const PAL::SessionID& sessionID, const PurCFetcher::WebPageProxyIdentifier& webPageProxyID, const PurCFetcher::PageIdentifier& webPageID, const URL& url, const String& userAgent, PurCFetcher::StoredCredentialsPolicy storedCredentialsPolicy, Optional<PurCFetcher::NavigatingToAppBoundDomain> isNavigatingToAppBoundDomain)
        : m_arguments(sessionID, webPageProxyID, webPageID, url, userAgent, storedCredentialsPolicy, isNavigatingToAppBoundDomain)
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
class ClearPrevalentResource {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ClearPrevalentResource; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_ClearPrevalentResourceReply; }
    using AsyncReply = ClearPrevalentResourceAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    ClearPrevalentResource(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& resourceDomain)
        : m_arguments(sessionID, resourceDomain)
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
class ClearUserInteraction {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ClearUserInteraction; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_ClearUserInteractionReply; }
    using AsyncReply = ClearUserInteractionAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    ClearUserInteraction(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& resourceDomain)
        : m_arguments(sessionID, resourceDomain)
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
class DumpResourceLoadStatistics {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_DumpResourceLoadStatistics; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(String&&)>&&);
    static void cancelReply(CompletionHandler<void(String&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_DumpResourceLoadStatisticsReply; }
    using AsyncReply = DumpResourceLoadStatisticsAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const String& dumpedStatistics);
    using Reply = std::tuple<String&>;
    using ReplyArguments = std::tuple<String>;
    explicit DumpResourceLoadStatistics(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
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
class SetResourceLoadStatisticsEnabled {
public:
    using Arguments = std::tuple<const PAL::SessionID&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetResourceLoadStatisticsEnabled; }
    static const bool isSync = false;

    SetResourceLoadStatisticsEnabled(const PAL::SessionID& sessionID, bool enabled)
        : m_arguments(sessionID, enabled)
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
class SetResourceLoadStatisticsLogTestingEvent {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetResourceLoadStatisticsLogTestingEvent; }
    static const bool isSync = false;

    explicit SetResourceLoadStatisticsLogTestingEvent(bool enabled)
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
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
class UpdatePrevalentDomainsToBlockCookiesFor {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const Vector<PurCFetcher::RegistrableDomain>&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_UpdatePrevalentDomainsToBlockCookiesFor; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_UpdatePrevalentDomainsToBlockCookiesForReply; }
    using AsyncReply = UpdatePrevalentDomainsToBlockCookiesForAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    UpdatePrevalentDomainsToBlockCookiesFor(const PAL::SessionID& sessionID, const Vector<PurCFetcher::RegistrableDomain>& domainsToBlock)
        : m_arguments(sessionID, domainsToBlock)
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
class IsGrandfathered {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_IsGrandfathered; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_IsGrandfatheredReply; }
    using AsyncReply = IsGrandfatheredAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool isGrandfathered);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    IsGrandfathered(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& targetDomain)
        : m_arguments(sessionID, targetDomain)
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
class IsPrevalentResource {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_IsPrevalentResource; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_IsPrevalentResourceReply; }
    using AsyncReply = IsPrevalentResourceAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool isPrevalent);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    IsPrevalentResource(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& targetDomain)
        : m_arguments(sessionID, targetDomain)
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
class IsVeryPrevalentResource {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_IsVeryPrevalentResource; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_IsVeryPrevalentResourceReply; }
    using AsyncReply = IsVeryPrevalentResourceAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool isVeryPrevalent);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    IsVeryPrevalentResource(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& targetDomain)
        : m_arguments(sessionID, targetDomain)
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
class SetAgeCapForClientSideCookies {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const Optional<Seconds>&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetAgeCapForClientSideCookies; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetAgeCapForClientSideCookiesReply; }
    using AsyncReply = SetAgeCapForClientSideCookiesAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetAgeCapForClientSideCookies(const PAL::SessionID& sessionID, const Optional<Seconds>& seconds)
        : m_arguments(sessionID, seconds)
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
class SetLastSeen {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, const Seconds&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetLastSeen; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetLastSeenReply; }
    using AsyncReply = SetLastSeenAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetLastSeen(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& resourceDomain, const Seconds& seconds)
        : m_arguments(sessionID, resourceDomain, seconds)
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
class MergeStatisticForTesting {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&, const Seconds&, bool, const Seconds&, bool, bool, bool, uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_MergeStatisticForTesting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_MergeStatisticForTestingReply; }
    using AsyncReply = MergeStatisticForTestingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    MergeStatisticForTesting(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& resourceDomain, const PurCFetcher::RegistrableDomain& topFrameDomain1, const PurCFetcher::RegistrableDomain& topFrameDomain2, const Seconds& lastSeen, bool hadUserInteraction, const Seconds& mostRecentUserInteraction, bool isGrandfathered, bool isPrevalent, bool isVeryPrevalent, uint64_t dataRecordsRemoved)
        : m_arguments(sessionID, resourceDomain, topFrameDomain1, topFrameDomain2, lastSeen, hadUserInteraction, mostRecentUserInteraction, isGrandfathered, isPrevalent, isVeryPrevalent, dataRecordsRemoved)
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
class InsertExpiredStatisticForTesting {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, bool, bool, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_InsertExpiredStatisticForTesting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_InsertExpiredStatisticForTestingReply; }
    using AsyncReply = InsertExpiredStatisticForTestingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    InsertExpiredStatisticForTesting(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& resourceDomain, bool hadUserInteraction, bool isScheduledForAllButCookieDataRemoval, bool isPrevalent)
        : m_arguments(sessionID, resourceDomain, hadUserInteraction, isScheduledForAllButCookieDataRemoval, isPrevalent)
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
class SetPrevalentResource {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetPrevalentResource; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetPrevalentResourceReply; }
    using AsyncReply = SetPrevalentResourceAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetPrevalentResource(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& resourceDomain)
        : m_arguments(sessionID, resourceDomain)
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
class SetPrevalentResourceForDebugMode {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetPrevalentResourceForDebugMode; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetPrevalentResourceForDebugModeReply; }
    using AsyncReply = SetPrevalentResourceForDebugModeAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetPrevalentResourceForDebugMode(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& resourceDomain)
        : m_arguments(sessionID, resourceDomain)
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
class IsResourceLoadStatisticsEphemeral {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_IsResourceLoadStatisticsEphemeral; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_IsResourceLoadStatisticsEphemeralReply; }
    using AsyncReply = IsResourceLoadStatisticsEphemeralAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool isEphemeral);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    explicit IsResourceLoadStatisticsEphemeral(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
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
class HadUserInteraction {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_HadUserInteraction; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_HadUserInteractionReply; }
    using AsyncReply = HadUserInteractionAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool hadUserInteraction);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    HadUserInteraction(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& resourceDomain)
        : m_arguments(sessionID, resourceDomain)
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
class IsRelationshipOnlyInDatabaseOnce {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_IsRelationshipOnlyInDatabaseOnce; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_IsRelationshipOnlyInDatabaseOnceReply; }
    using AsyncReply = IsRelationshipOnlyInDatabaseOnceAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool hadUserInteraction);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    IsRelationshipOnlyInDatabaseOnce(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& subDomain, const PurCFetcher::RegistrableDomain& topDomain)
        : m_arguments(sessionID, subDomain, topDomain)
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
class HasLocalStorage {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_HasLocalStorage; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_HasLocalStorageReply; }
    using AsyncReply = HasLocalStorageAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool hadUserInteraction);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    HasLocalStorage(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& resourceDomain)
        : m_arguments(sessionID, resourceDomain)
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
class GetAllStorageAccessEntries {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_GetAllStorageAccessEntries; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(Vector<String>&&)>&&);
    static void cancelReply(CompletionHandler<void(Vector<String>&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_GetAllStorageAccessEntriesReply; }
    using AsyncReply = GetAllStorageAccessEntriesAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Vector<String>& domains);
    using Reply = std::tuple<Vector<String>&>;
    using ReplyArguments = std::tuple<Vector<String>>;
    explicit GetAllStorageAccessEntries(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
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
class IsRegisteredAsRedirectingTo {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_IsRegisteredAsRedirectingTo; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_IsRegisteredAsRedirectingToReply; }
    using AsyncReply = IsRegisteredAsRedirectingToAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool isRedirectingTo);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    IsRegisteredAsRedirectingTo(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& redirectedFromDomain, const PurCFetcher::RegistrableDomain& redirectedToDomain)
        : m_arguments(sessionID, redirectedFromDomain, redirectedToDomain)
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
class IsRegisteredAsSubFrameUnder {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_IsRegisteredAsSubFrameUnder; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_IsRegisteredAsSubFrameUnderReply; }
    using AsyncReply = IsRegisteredAsSubFrameUnderAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool isSubframeUnder);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    IsRegisteredAsSubFrameUnder(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& subFrameDomain, const PurCFetcher::RegistrableDomain& topFrameDomain)
        : m_arguments(sessionID, subFrameDomain, topFrameDomain)
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
class IsRegisteredAsSubresourceUnder {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_IsRegisteredAsSubresourceUnder; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_IsRegisteredAsSubresourceUnderReply; }
    using AsyncReply = IsRegisteredAsSubresourceUnderAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool isSubresourceUnder);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    IsRegisteredAsSubresourceUnder(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& subresourceDomain, const PurCFetcher::RegistrableDomain& topFrameDomain)
        : m_arguments(sessionID, subresourceDomain, topFrameDomain)
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
class DomainIDExistsInDatabase {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const int&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_DomainIDExistsInDatabase; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_DomainIDExistsInDatabaseReply; }
    using AsyncReply = DomainIDExistsInDatabaseAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool exists);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    DomainIDExistsInDatabase(const PAL::SessionID& sessionID, const int& domainID)
        : m_arguments(sessionID, domainID)
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
class LogFrameNavigation {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&, bool, bool, const Seconds&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_LogFrameNavigation; }
    static const bool isSync = false;

    LogFrameNavigation(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& targetDomain, const PurCFetcher::RegistrableDomain& topFrameDomain, const PurCFetcher::RegistrableDomain& sourceDomain, bool isRedirect, bool isMainFrame, const Seconds& delayAfterMainFrameDocumentLoad, bool wasPotentiallyInitiatedByUser)
        : m_arguments(sessionID, targetDomain, topFrameDomain, sourceDomain, isRedirect, isMainFrame, delayAfterMainFrameDocumentLoad, wasPotentiallyInitiatedByUser)
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
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_LogUserInteraction; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_LogUserInteractionReply; }
    using AsyncReply = LogUserInteractionAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    LogUserInteraction(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& topFrameDomain)
        : m_arguments(sessionID, topFrameDomain)
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
class ResetParametersToDefaultValues {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ResetParametersToDefaultValues; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_ResetParametersToDefaultValuesReply; }
    using AsyncReply = ResetParametersToDefaultValuesAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit ResetParametersToDefaultValues(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
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
class ScheduleClearInMemoryAndPersistent {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const Optional<WallTime>&, PurCFetcher::ShouldGrandfatherStatistics>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ScheduleClearInMemoryAndPersistent; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_ScheduleClearInMemoryAndPersistentReply; }
    using AsyncReply = ScheduleClearInMemoryAndPersistentAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    ScheduleClearInMemoryAndPersistent(const PAL::SessionID& sessionID, const Optional<WallTime>& modifiedSince, PurCFetcher::ShouldGrandfatherStatistics shouldGrandfather)
        : m_arguments(sessionID, modifiedSince, shouldGrandfather)
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
class ScheduleCookieBlockingUpdate {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ScheduleCookieBlockingUpdate; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_ScheduleCookieBlockingUpdateReply; }
    using AsyncReply = ScheduleCookieBlockingUpdateAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit ScheduleCookieBlockingUpdate(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
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
class ScheduleStatisticsAndDataRecordsProcessing {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ScheduleStatisticsAndDataRecordsProcessing; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_ScheduleStatisticsAndDataRecordsProcessingReply; }
    using AsyncReply = ScheduleStatisticsAndDataRecordsProcessingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit ScheduleStatisticsAndDataRecordsProcessing(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
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
class StatisticsDatabaseHasAllTables {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_StatisticsDatabaseHasAllTables; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_StatisticsDatabaseHasAllTablesReply; }
    using AsyncReply = StatisticsDatabaseHasAllTablesAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool hasAllTables);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    explicit StatisticsDatabaseHasAllTables(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
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
class SubmitTelemetry {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SubmitTelemetry; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SubmitTelemetryReply; }
    using AsyncReply = SubmitTelemetryAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit SubmitTelemetry(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
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
class SetCacheMaxAgeCapForPrevalentResources {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const Seconds&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetCacheMaxAgeCapForPrevalentResources; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetCacheMaxAgeCapForPrevalentResourcesReply; }
    using AsyncReply = SetCacheMaxAgeCapForPrevalentResourcesAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetCacheMaxAgeCapForPrevalentResources(const PAL::SessionID& sessionID, const Seconds& seconds)
        : m_arguments(sessionID, seconds)
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
class SetGrandfathered {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetGrandfathered; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetGrandfatheredReply; }
    using AsyncReply = SetGrandfatheredAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetGrandfathered(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& resourceDomain, bool isGrandfathered)
        : m_arguments(sessionID, resourceDomain, isGrandfathered)
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
class SetUseITPDatabase {
public:
    using Arguments = std::tuple<const PAL::SessionID&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetUseITPDatabase; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetUseITPDatabaseReply; }
    using AsyncReply = SetUseITPDatabaseAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetUseITPDatabase(const PAL::SessionID& sessionID, bool value)
        : m_arguments(sessionID, value)
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
class SetGrandfatheringTime {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const Seconds&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetGrandfatheringTime; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetGrandfatheringTimeReply; }
    using AsyncReply = SetGrandfatheringTimeAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetGrandfatheringTime(const PAL::SessionID& sessionID, const Seconds& seconds)
        : m_arguments(sessionID, seconds)
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
class SetMaxStatisticsEntries {
public:
    using Arguments = std::tuple<const PAL::SessionID&, uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetMaxStatisticsEntries; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetMaxStatisticsEntriesReply; }
    using AsyncReply = SetMaxStatisticsEntriesAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetMaxStatisticsEntries(const PAL::SessionID& sessionID, uint64_t maximumEntryCount)
        : m_arguments(sessionID, maximumEntryCount)
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
class SetMinimumTimeBetweenDataRecordsRemoval {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const Seconds&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetMinimumTimeBetweenDataRecordsRemoval; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetMinimumTimeBetweenDataRecordsRemovalReply; }
    using AsyncReply = SetMinimumTimeBetweenDataRecordsRemovalAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetMinimumTimeBetweenDataRecordsRemoval(const PAL::SessionID& sessionID, const Seconds& seconds)
        : m_arguments(sessionID, seconds)
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
class SetPruneEntriesDownTo {
public:
    using Arguments = std::tuple<const PAL::SessionID&, uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetPruneEntriesDownTo; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetPruneEntriesDownToReply; }
    using AsyncReply = SetPruneEntriesDownToAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetPruneEntriesDownTo(const PAL::SessionID& sessionID, uint64_t maximumEntryCount)
        : m_arguments(sessionID, maximumEntryCount)
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
class SetShouldClassifyResourcesBeforeDataRecordsRemoval {
public:
    using Arguments = std::tuple<const PAL::SessionID&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetShouldClassifyResourcesBeforeDataRecordsRemoval; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetShouldClassifyResourcesBeforeDataRecordsRemovalReply; }
    using AsyncReply = SetShouldClassifyResourcesBeforeDataRecordsRemovalAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetShouldClassifyResourcesBeforeDataRecordsRemoval(const PAL::SessionID& sessionID, bool value)
        : m_arguments(sessionID, value)
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
class SetNotifyPagesWhenDataRecordsWereScanned {
public:
    using Arguments = std::tuple<const PAL::SessionID&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetNotifyPagesWhenDataRecordsWereScanned; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetNotifyPagesWhenDataRecordsWereScannedReply; }
    using AsyncReply = SetNotifyPagesWhenDataRecordsWereScannedAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetNotifyPagesWhenDataRecordsWereScanned(const PAL::SessionID& sessionID, bool value)
        : m_arguments(sessionID, value)
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
class SetIsRunningResourceLoadStatisticsTest {
public:
    using Arguments = std::tuple<const PAL::SessionID&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetIsRunningResourceLoadStatisticsTest; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetIsRunningResourceLoadStatisticsTestReply; }
    using AsyncReply = SetIsRunningResourceLoadStatisticsTestAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetIsRunningResourceLoadStatisticsTest(const PAL::SessionID& sessionID, bool value)
        : m_arguments(sessionID, value)
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
class SetNotifyPagesWhenTelemetryWasCaptured {
public:
    using Arguments = std::tuple<const PAL::SessionID&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetNotifyPagesWhenTelemetryWasCaptured; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetNotifyPagesWhenTelemetryWasCapturedReply; }
    using AsyncReply = SetNotifyPagesWhenTelemetryWasCapturedAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetNotifyPagesWhenTelemetryWasCaptured(const PAL::SessionID& sessionID, bool value)
        : m_arguments(sessionID, value)
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
class SetResourceLoadStatisticsDebugMode {
public:
    using Arguments = std::tuple<const PAL::SessionID&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetResourceLoadStatisticsDebugMode; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetResourceLoadStatisticsDebugModeReply; }
    using AsyncReply = SetResourceLoadStatisticsDebugModeAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetResourceLoadStatisticsDebugMode(const PAL::SessionID& sessionID, bool debugMode)
        : m_arguments(sessionID, debugMode)
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
class SetVeryPrevalentResource {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetVeryPrevalentResource; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetVeryPrevalentResourceReply; }
    using AsyncReply = SetVeryPrevalentResourceAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetVeryPrevalentResource(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& resourceDomain)
        : m_arguments(sessionID, resourceDomain)
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
class SetSubframeUnderTopFrameDomain {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetSubframeUnderTopFrameDomain; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetSubframeUnderTopFrameDomainReply; }
    using AsyncReply = SetSubframeUnderTopFrameDomainAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetSubframeUnderTopFrameDomain(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& subFrameDomain, const PurCFetcher::RegistrableDomain& topFrameDomain)
        : m_arguments(sessionID, subFrameDomain, topFrameDomain)
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
class SetSubresourceUnderTopFrameDomain {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetSubresourceUnderTopFrameDomain; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetSubresourceUnderTopFrameDomainReply; }
    using AsyncReply = SetSubresourceUnderTopFrameDomainAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetSubresourceUnderTopFrameDomain(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& subresourceDomain, const PurCFetcher::RegistrableDomain& topFrameDomain)
        : m_arguments(sessionID, subresourceDomain, topFrameDomain)
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
class SetSubresourceUniqueRedirectTo {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetSubresourceUniqueRedirectTo; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetSubresourceUniqueRedirectToReply; }
    using AsyncReply = SetSubresourceUniqueRedirectToAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetSubresourceUniqueRedirectTo(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& subresourceDomain, const PurCFetcher::RegistrableDomain& redirectedToDomain)
        : m_arguments(sessionID, subresourceDomain, redirectedToDomain)
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
class SetSubresourceUniqueRedirectFrom {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetSubresourceUniqueRedirectFrom; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetSubresourceUniqueRedirectFromReply; }
    using AsyncReply = SetSubresourceUniqueRedirectFromAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetSubresourceUniqueRedirectFrom(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& subresourceDomain, const PurCFetcher::RegistrableDomain& redirectedFromDomain)
        : m_arguments(sessionID, subresourceDomain, redirectedFromDomain)
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
class SetTimeToLiveUserInteraction {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const Seconds&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetTimeToLiveUserInteraction; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetTimeToLiveUserInteractionReply; }
    using AsyncReply = SetTimeToLiveUserInteractionAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetTimeToLiveUserInteraction(const PAL::SessionID& sessionID, const Seconds& seconds)
        : m_arguments(sessionID, seconds)
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
class SetTopFrameUniqueRedirectTo {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetTopFrameUniqueRedirectTo; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetTopFrameUniqueRedirectToReply; }
    using AsyncReply = SetTopFrameUniqueRedirectToAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetTopFrameUniqueRedirectTo(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& topFrameDomain, const PurCFetcher::RegistrableDomain& redirectedToDomain)
        : m_arguments(sessionID, topFrameDomain, redirectedToDomain)
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
class SetTopFrameUniqueRedirectFrom {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetTopFrameUniqueRedirectFrom; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetTopFrameUniqueRedirectFromReply; }
    using AsyncReply = SetTopFrameUniqueRedirectFromAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetTopFrameUniqueRedirectFrom(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& topFrameDomain, const PurCFetcher::RegistrableDomain& redirectedFromDomain)
        : m_arguments(sessionID, topFrameDomain, redirectedFromDomain)
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
class ResetCacheMaxAgeCapForPrevalentResources {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ResetCacheMaxAgeCapForPrevalentResources; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_ResetCacheMaxAgeCapForPrevalentResourcesReply; }
    using AsyncReply = ResetCacheMaxAgeCapForPrevalentResourcesAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit ResetCacheMaxAgeCapForPrevalentResources(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
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
class DidCommitCrossSiteLoadWithDataTransfer {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&, const OptionSet<PurCFetcher::CrossSiteNavigationDataTransfer::Flag>&, const PurCFetcher::WebPageProxyIdentifier&, const PurCFetcher::PageIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_DidCommitCrossSiteLoadWithDataTransfer; }
    static const bool isSync = false;

    DidCommitCrossSiteLoadWithDataTransfer(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& fromDomain, const PurCFetcher::RegistrableDomain& toDomain, const OptionSet<PurCFetcher::CrossSiteNavigationDataTransfer::Flag>& navigationDataTransfer, const PurCFetcher::WebPageProxyIdentifier& webPageProxyID, const PurCFetcher::PageIdentifier& webPageID)
        : m_arguments(sessionID, fromDomain, toDomain, navigationDataTransfer, webPageProxyID, webPageID)
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
class SetCrossSiteLoadWithLinkDecorationForTesting {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetCrossSiteLoadWithLinkDecorationForTesting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetCrossSiteLoadWithLinkDecorationForTestingReply; }
    using AsyncReply = SetCrossSiteLoadWithLinkDecorationForTestingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetCrossSiteLoadWithLinkDecorationForTesting(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& fromDomain, const PurCFetcher::RegistrableDomain& toDomain)
        : m_arguments(sessionID, fromDomain, toDomain)
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
class ResetCrossSiteLoadsWithLinkDecorationForTesting {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ResetCrossSiteLoadsWithLinkDecorationForTesting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_ResetCrossSiteLoadsWithLinkDecorationForTestingReply; }
    using AsyncReply = ResetCrossSiteLoadsWithLinkDecorationForTestingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit ResetCrossSiteLoadsWithLinkDecorationForTesting(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
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
class DeleteCookiesForTesting {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_DeleteCookiesForTesting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_DeleteCookiesForTestingReply; }
    using AsyncReply = DeleteCookiesForTestingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    DeleteCookiesForTesting(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& domain, bool includeHttpOnlyCookies)
        : m_arguments(sessionID, domain, includeHttpOnlyCookies)
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
class HasIsolatedSession {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_HasIsolatedSession; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_HasIsolatedSessionReply; }
    using AsyncReply = HasIsolatedSessionAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool hasIsolatedSession);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    HasIsolatedSession(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& domain)
        : m_arguments(sessionID, domain)
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
class SetAppBoundDomainsForResourceLoadStatistics {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const HashSet<PurCFetcher::RegistrableDomain>&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetAppBoundDomainsForResourceLoadStatistics; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetAppBoundDomainsForResourceLoadStatisticsReply; }
    using AsyncReply = SetAppBoundDomainsForResourceLoadStatisticsAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetAppBoundDomainsForResourceLoadStatistics(const PAL::SessionID& sessionID, const HashSet<PurCFetcher::RegistrableDomain>& appBoundDomains)
        : m_arguments(sessionID, appBoundDomains)
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
class SetShouldDowngradeReferrerForTesting {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetShouldDowngradeReferrerForTesting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetShouldDowngradeReferrerForTestingReply; }
    using AsyncReply = SetShouldDowngradeReferrerForTestingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit SetShouldDowngradeReferrerForTesting(bool enabled)
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
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
class SetThirdPartyCookieBlockingMode {
public:
    using Arguments = std::tuple<const PAL::SessionID&, PurCFetcher::ThirdPartyCookieBlockingMode>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetThirdPartyCookieBlockingMode; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetThirdPartyCookieBlockingModeReply; }
    using AsyncReply = SetThirdPartyCookieBlockingModeAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetThirdPartyCookieBlockingMode(const PAL::SessionID& sessionID, PurCFetcher::ThirdPartyCookieBlockingMode blockingMode)
        : m_arguments(sessionID, blockingMode)
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
class SetShouldEnbleSameSiteStrictEnforcementForTesting {
public:
    using Arguments = std::tuple<const PAL::SessionID&, PurCFetcher::SameSiteStrictEnforcementEnabled>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetShouldEnbleSameSiteStrictEnforcementForTesting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetShouldEnbleSameSiteStrictEnforcementForTestingReply; }
    using AsyncReply = SetShouldEnbleSameSiteStrictEnforcementForTestingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetShouldEnbleSameSiteStrictEnforcementForTesting(const PAL::SessionID& sessionID, PurCFetcher::SameSiteStrictEnforcementEnabled enabled)
        : m_arguments(sessionID, enabled)
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
class SetFirstPartyWebsiteDataRemovalModeForTesting {
public:
    using Arguments = std::tuple<const PAL::SessionID&, PurCFetcher::FirstPartyWebsiteDataRemovalMode>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetFirstPartyWebsiteDataRemovalModeForTesting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetFirstPartyWebsiteDataRemovalModeForTestingReply; }
    using AsyncReply = SetFirstPartyWebsiteDataRemovalModeForTestingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetFirstPartyWebsiteDataRemovalModeForTesting(const PAL::SessionID& sessionID, PurCFetcher::FirstPartyWebsiteDataRemovalMode mode)
        : m_arguments(sessionID, mode)
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
class SetToSameSiteStrictCookiesForTesting {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const PurCFetcher::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetToSameSiteStrictCookiesForTesting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetToSameSiteStrictCookiesForTestingReply; }
    using AsyncReply = SetToSameSiteStrictCookiesForTestingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetToSameSiteStrictCookiesForTesting(const PAL::SessionID& sessionID, const PurCFetcher::RegistrableDomain& domain)
        : m_arguments(sessionID, domain)
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

class SetSessionIsControlledByAutomation {
public:
    using Arguments = std::tuple<const PAL::SessionID&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetSessionIsControlledByAutomation; }
    static const bool isSync = false;

    SetSessionIsControlledByAutomation(const PAL::SessionID& sessionID, bool controlled)
        : m_arguments(sessionID, controlled)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterURLSchemeAsSecure {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_RegisterURLSchemeAsSecure; }
    static const bool isSync = false;

    explicit RegisterURLSchemeAsSecure(const String& scheme)
        : m_arguments(scheme)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterURLSchemeAsBypassingContentSecurityPolicy {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_RegisterURLSchemeAsBypassingContentSecurityPolicy; }
    static const bool isSync = false;

    explicit RegisterURLSchemeAsBypassingContentSecurityPolicy(const String& scheme)
        : m_arguments(scheme)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterURLSchemeAsLocal {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_RegisterURLSchemeAsLocal; }
    static const bool isSync = false;

    explicit RegisterURLSchemeAsLocal(const String& scheme)
        : m_arguments(scheme)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterURLSchemeAsNoAccess {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_RegisterURLSchemeAsNoAccess; }
    static const bool isSync = false;

    explicit RegisterURLSchemeAsNoAccess(const String& scheme)
        : m_arguments(scheme)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetCacheStorageParameters {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const String&, const PurCFetcher::SandboxExtension::Handle&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetCacheStorageParameters; }
    static const bool isSync = false;

    SetCacheStorageParameters(const PAL::SessionID& sessionID, const String& cacheStorageDirectory, const PurCFetcher::SandboxExtension::Handle& handle)
        : m_arguments(sessionID, cacheStorageDirectory, handle)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SyncLocalStorage {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SyncLocalStorage; }
    static const bool isSync = true;

    using DelayedReply = SyncLocalStorageDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ClearLegacyPrivateBrowsingLocalStorage {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ClearLegacyPrivateBrowsingLocalStorage; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DumpAdClickAttribution {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_DumpAdClickAttribution; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(String&&)>&&);
    static void cancelReply(CompletionHandler<void(String&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_DumpAdClickAttributionReply; }
    using AsyncReply = DumpAdClickAttributionAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const String& adClickAttributionState);
    using Reply = std::tuple<String&>;
    using ReplyArguments = std::tuple<String>;
    explicit DumpAdClickAttribution(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ClearAdClickAttribution {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ClearAdClickAttribution; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_ClearAdClickAttributionReply; }
    using AsyncReply = ClearAdClickAttributionAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit ClearAdClickAttribution(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetAdClickAttributionOverrideTimerForTesting {
public:
    using Arguments = std::tuple<const PAL::SessionID&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetAdClickAttributionOverrideTimerForTesting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetAdClickAttributionOverrideTimerForTestingReply; }
    using AsyncReply = SetAdClickAttributionOverrideTimerForTestingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetAdClickAttributionOverrideTimerForTesting(const PAL::SessionID& sessionID, bool value)
        : m_arguments(sessionID, value)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetAdClickAttributionConversionURLForTesting {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const URL&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetAdClickAttributionConversionURLForTesting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_SetAdClickAttributionConversionURLForTestingReply; }
    using AsyncReply = SetAdClickAttributionConversionURLForTestingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SetAdClickAttributionConversionURLForTesting(const PAL::SessionID& sessionID, const URL& url)
        : m_arguments(sessionID, url)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class MarkAdClickAttributionsAsExpiredForTesting {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_MarkAdClickAttributionsAsExpiredForTesting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_MarkAdClickAttributionsAsExpiredForTestingReply; }
    using AsyncReply = MarkAdClickAttributionsAsExpiredForTestingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit MarkAdClickAttributionsAsExpiredForTesting(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class GetLocalStorageOriginDetails {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_GetLocalStorageOriginDetails; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(Vector<PurCFetcher::LocalStorageDatabaseTracker::OriginDetails>&&)>&&);
    static void cancelReply(CompletionHandler<void(Vector<PurCFetcher::LocalStorageDatabaseTracker::OriginDetails>&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_GetLocalStorageOriginDetailsReply; }
    using AsyncReply = GetLocalStorageOriginDetailsAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Vector<PurCFetcher::LocalStorageDatabaseTracker::OriginDetails>& details);
    using Reply = std::tuple<Vector<PurCFetcher::LocalStorageDatabaseTracker::OriginDetails>&>;
    using ReplyArguments = std::tuple<Vector<PurCFetcher::LocalStorageDatabaseTracker::OriginDetails>>;
    explicit GetLocalStorageOriginDetails(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetServiceWorkerFetchTimeoutForTesting {
public:
    using Arguments = std::tuple<const Seconds&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_SetServiceWorkerFetchTimeoutForTesting; }
    static const bool isSync = true;

    using DelayedReply = SetServiceWorkerFetchTimeoutForTestingDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit SetServiceWorkerFetchTimeoutForTesting(const Seconds& seconds)
        : m_arguments(seconds)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ResetServiceWorkerFetchTimeoutForTesting {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ResetServiceWorkerFetchTimeoutForTesting; }
    static const bool isSync = true;

    using DelayedReply = ResetServiceWorkerFetchTimeoutForTestingDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ResetQuota {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ResetQuota; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_ResetQuotaReply; }
    using AsyncReply = ResetQuotaAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit ResetQuota(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class HasAppBoundSession {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_HasAppBoundSession; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_HasAppBoundSessionReply; }
    using AsyncReply = HasAppBoundSessionAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool hasAppBoundSession);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    explicit HasAppBoundSession(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ClearAppBoundSession {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ClearAppBoundSession; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_ClearAppBoundSessionReply; }
    using AsyncReply = ClearAppBoundSessionAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit ClearAppBoundSession(const PAL::SessionID& sessionID)
        : m_arguments(sessionID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if PLATFORM(IOS_FAMILY) && !PLATFORM(MACCATALYST)
class DisableServiceWorkerEntitlement {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_DisableServiceWorkerEntitlement; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if PLATFORM(IOS_FAMILY) && !PLATFORM(MACCATALYST)
class ClearServiceWorkerEntitlementOverride {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ClearServiceWorkerEntitlementOverride; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_ClearServiceWorkerEntitlementOverrideReply; }
    using AsyncReply = ClearServiceWorkerEntitlementOverrideAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

class UpdateBundleIdentifier {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_UpdateBundleIdentifier; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_UpdateBundleIdentifierReply; }
    using AsyncReply = UpdateBundleIdentifierAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit UpdateBundleIdentifier(const String& bundleIdentifier)
        : m_arguments(bundleIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ClearBundleIdentifier {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcess_ClearBundleIdentifier; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcess_ClearBundleIdentifierReply; }
    using AsyncReply = ClearBundleIdentifierAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

} // namespace NetworkProcess
} // namespace Messages
