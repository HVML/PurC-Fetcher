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
#include "FormDataReference.h"
#include "MessageNames.h"
#include "NetworkProcessProxyMessagesReplies.h"
#include "UserContentControllerIdentifier.h"
#include "WebPageProxyIdentifier.h"
#include "WebsiteDataFetchOption.h"
#include "WebsiteDataType.h"
#include <WebCore/FrameIdentifier.h>
#include <WebCore/ProcessIdentifier.h>
#include <WebCore/RegistrableDomain.h>
#include <WebCore/SecurityOriginData.h>
#include <wtf/Forward.h>
#include <wtf/HashSet.h>
#include <wtf/OptionSet.h>
#include <wtf/Optional.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace PAL {
class SessionID;
}

namespace WebCore {
class AuthenticationChallenge;
class RegistrableDomain;
class ResourceError;
class ResourceRequest;
class ResourceResponse;
enum class ShouldSample : bool;
struct ClientOrigin;
}

namespace WebKit {
class CallbackID;
struct ResourceLoadInfo;
struct WebsiteData;
}

namespace Messages {
namespace NetworkProcessProxy {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::NetworkProcessProxy;
}

class DidReceiveAuthenticationChallenge {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const WebKit::WebPageProxyIdentifier&, const Optional<WebCore::SecurityOriginData>&, const WebCore::AuthenticationChallenge&, bool, uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_DidReceiveAuthenticationChallenge; }
    static const bool isSync = false;

    DidReceiveAuthenticationChallenge(const PAL::SessionID& sessionID, const WebKit::WebPageProxyIdentifier& pageID, const Optional<WebCore::SecurityOriginData>& topOrigin, const WebCore::AuthenticationChallenge& challenge, bool negotiatedLegacyTLS, uint64_t challengeID)
        : m_arguments(sessionID, pageID, topOrigin, challenge, negotiatedLegacyTLS, challengeID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class NegotiatedLegacyTLS {
public:
    using Arguments = std::tuple<const WebKit::WebPageProxyIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_NegotiatedLegacyTLS; }
    static const bool isSync = false;

    explicit NegotiatedLegacyTLS(const WebKit::WebPageProxyIdentifier& pageID)
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

class DidNegotiateModernTLS {
public:
    using Arguments = std::tuple<const WebKit::WebPageProxyIdentifier&, const WebCore::AuthenticationChallenge&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_DidNegotiateModernTLS; }
    static const bool isSync = false;

    DidNegotiateModernTLS(const WebKit::WebPageProxyIdentifier& pageID, const WebCore::AuthenticationChallenge& challenge)
        : m_arguments(pageID, challenge)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidFetchWebsiteData {
public:
    using Arguments = std::tuple<const WebKit::CallbackID&, const WebKit::WebsiteData&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_DidFetchWebsiteData; }
    static const bool isSync = false;

    DidFetchWebsiteData(const WebKit::CallbackID& callbackID, const WebKit::WebsiteData& websiteData)
        : m_arguments(callbackID, websiteData)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidDeleteWebsiteData {
public:
    using Arguments = std::tuple<const WebKit::CallbackID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_DidDeleteWebsiteData; }
    static const bool isSync = false;

    explicit DidDeleteWebsiteData(const WebKit::CallbackID& callbackID)
        : m_arguments(callbackID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidDeleteWebsiteDataForOrigins {
public:
    using Arguments = std::tuple<const WebKit::CallbackID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_DidDeleteWebsiteDataForOrigins; }
    static const bool isSync = false;

    explicit DidDeleteWebsiteDataForOrigins(const WebKit::CallbackID& callbackID)
        : m_arguments(callbackID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidSyncAllCookies {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_DidSyncAllCookies; }
    static const bool isSync = false;

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

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_TestProcessIncomingSyncMessagesWhenWaitingForSyncReply; }
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

class TerminateUnresponsiveServiceWorkerProcesses {
public:
    using Arguments = std::tuple<const WebCore::ProcessIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_TerminateUnresponsiveServiceWorkerProcesses; }
    static const bool isSync = false;

    explicit TerminateUnresponsiveServiceWorkerProcesses(const WebCore::ProcessIdentifier& processIdentifier)
        : m_arguments(processIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetIsHoldingLockedFiles {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_SetIsHoldingLockedFiles; }
    static const bool isSync = false;

    explicit SetIsHoldingLockedFiles(bool isHoldingLockedFiles)
        : m_arguments(isHoldingLockedFiles)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class LogDiagnosticMessage {
public:
    using Arguments = std::tuple<const WebKit::WebPageProxyIdentifier&, const String&, const String&, WebCore::ShouldSample>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_LogDiagnosticMessage; }
    static const bool isSync = false;

    LogDiagnosticMessage(const WebKit::WebPageProxyIdentifier& pageID, const String& message, const String& description, WebCore::ShouldSample shouldSample)
        : m_arguments(pageID, message, description, shouldSample)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class LogDiagnosticMessageWithResult {
public:
    using Arguments = std::tuple<const WebKit::WebPageProxyIdentifier&, const String&, const String&, uint32_t, WebCore::ShouldSample>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_LogDiagnosticMessageWithResult; }
    static const bool isSync = false;

    LogDiagnosticMessageWithResult(const WebKit::WebPageProxyIdentifier& pageID, const String& message, const String& description, uint32_t result, WebCore::ShouldSample shouldSample)
        : m_arguments(pageID, message, description, result, shouldSample)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class LogDiagnosticMessageWithValue {
public:
    using Arguments = std::tuple<const WebKit::WebPageProxyIdentifier&, const String&, const String&, double, const unsigned&, WebCore::ShouldSample>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_LogDiagnosticMessageWithValue; }
    static const bool isSync = false;

    LogDiagnosticMessageWithValue(const WebKit::WebPageProxyIdentifier& pageID, const String& message, const String& description, double value, const unsigned& significantFigures, WebCore::ShouldSample shouldSample)
        : m_arguments(pageID, message, description, value, significantFigures, shouldSample)
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
class LogTestingEvent {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_LogTestingEvent; }
    static const bool isSync = false;

    LogTestingEvent(const PAL::SessionID& sessionID, const String& event)
        : m_arguments(sessionID, event)
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
class NotifyResourceLoadStatisticsProcessed {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_NotifyResourceLoadStatisticsProcessed; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
class NotifyWebsiteDataDeletionForRegistrableDomainsFinished {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_NotifyWebsiteDataDeletionForRegistrableDomainsFinished; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
class NotifyWebsiteDataScanForRegistrableDomainsFinished {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_NotifyWebsiteDataScanForRegistrableDomainsFinished; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
class NotifyResourceLoadStatisticsTelemetryFinished {
public:
    using Arguments = std::tuple<const unsigned&, const unsigned&, const unsigned&, const unsigned&, const unsigned&, const unsigned&, const unsigned&, const unsigned&, const unsigned&, const unsigned&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_NotifyResourceLoadStatisticsTelemetryFinished; }
    static const bool isSync = false;

    NotifyResourceLoadStatisticsTelemetryFinished(const unsigned& numberOfPrevalentResources, const unsigned& numberOfPrevalentResourcesWithUserInteraction, const unsigned& numberOfPrevalentResourcesWithoutUserInteraction, const unsigned& topPrevalentResourceWithUserInteractionDaysSinceUserInteraction, const unsigned& medianDaysSinceUserInteractionPrevalentResourceWithUserInteraction, const unsigned& top3NumberOfPrevalentResourcesWithUI, const unsigned& top3MedianSubFrameWithoutUI, const unsigned& top3MedianSubResourceWithoutUI, const unsigned& top3MedianUniqueRedirectsWithoutUI, const unsigned& top3MedianDataRecordsRemovedWithoutUI)
        : m_arguments(numberOfPrevalentResources, numberOfPrevalentResourcesWithUserInteraction, numberOfPrevalentResourcesWithoutUserInteraction, topPrevalentResourceWithUserInteractionDaysSinceUserInteraction, medianDaysSinceUserInteractionPrevalentResourceWithUserInteraction, top3NumberOfPrevalentResourcesWithUI, top3MedianSubFrameWithoutUI, top3MedianSubResourceWithoutUI, top3MedianUniqueRedirectsWithoutUI, top3MedianDataRecordsRemovedWithoutUI)
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
class RequestStorageAccessConfirm {
public:
    using Arguments = std::tuple<const WebKit::WebPageProxyIdentifier&, const WebCore::FrameIdentifier&, const WebCore::RegistrableDomain&, const WebCore::RegistrableDomain&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_RequestStorageAccessConfirm; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcessProxy_RequestStorageAccessConfirmReply; }
    using AsyncReply = RequestStorageAccessConfirmAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool userDidGrantAccess);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    RequestStorageAccessConfirm(const WebKit::WebPageProxyIdentifier& pageID, const WebCore::FrameIdentifier& frameID, const WebCore::RegistrableDomain& subFrameDomain, const WebCore::RegistrableDomain& topFrameDomain)
        : m_arguments(pageID, frameID, subFrameDomain, topFrameDomain)
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
class DeleteWebsiteDataInUIProcessForRegistrableDomains {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const OptionSet<WebKit::WebsiteDataType>&, const OptionSet<WebKit::WebsiteDataFetchOption>&, const Vector<WebCore::RegistrableDomain>&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_DeleteWebsiteDataInUIProcessForRegistrableDomains; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(HashSet<WebCore::RegistrableDomain>&&)>&&);
    static void cancelReply(CompletionHandler<void(HashSet<WebCore::RegistrableDomain>&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcessProxy_DeleteWebsiteDataInUIProcessForRegistrableDomainsReply; }
    using AsyncReply = DeleteWebsiteDataInUIProcessForRegistrableDomainsAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const HashSet<WebCore::RegistrableDomain>& domainsWithMatchingDataRecords);
    using Reply = std::tuple<HashSet<WebCore::RegistrableDomain>&>;
    using ReplyArguments = std::tuple<HashSet<WebCore::RegistrableDomain>>;
    DeleteWebsiteDataInUIProcessForRegistrableDomains(const PAL::SessionID& sessionID, const OptionSet<WebKit::WebsiteDataType>& dataTypes, const OptionSet<WebKit::WebsiteDataFetchOption>& fetchOptions, const Vector<WebCore::RegistrableDomain>& domains)
        : m_arguments(sessionID, dataTypes, fetchOptions, domains)
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
class DidCommitCrossSiteLoadWithDataTransferFromPrevalentResource {
public:
    using Arguments = std::tuple<const WebKit::WebPageProxyIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_DidCommitCrossSiteLoadWithDataTransferFromPrevalentResource; }
    static const bool isSync = false;

    explicit DidCommitCrossSiteLoadWithDataTransferFromPrevalentResource(const WebKit::WebPageProxyIdentifier& pageID)
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
class SetDomainsWithUserInteraction {
public:
    using Arguments = std::tuple<const HashSet<WebCore::RegistrableDomain>&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_SetDomainsWithUserInteraction; }
    static const bool isSync = false;

    explicit SetDomainsWithUserInteraction(const HashSet<WebCore::RegistrableDomain>& domains)
        : m_arguments(domains)
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

#if ENABLE(CONTENT_EXTENSIONS)
class ContentExtensionRules {
public:
    using Arguments = std::tuple<const WebKit::UserContentControllerIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_ContentExtensionRules; }
    static const bool isSync = false;

    explicit ContentExtensionRules(const WebKit::UserContentControllerIdentifier& identifier)
        : m_arguments(identifier)
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

class RetrieveCacheStorageParameters {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_RetrieveCacheStorageParameters; }
    static const bool isSync = false;

    explicit RetrieveCacheStorageParameters(const PAL::SessionID& sessionID)
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

class TerminateWebProcess {
public:
    using Arguments = std::tuple<const WebCore::ProcessIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_TerminateWebProcess; }
    static const bool isSync = false;

    explicit TerminateWebProcess(const WebCore::ProcessIdentifier& webProcessIdentifier)
        : m_arguments(webProcessIdentifier)
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
class EstablishWorkerContextConnectionToNetworkProcess {
public:
    using Arguments = std::tuple<const WebCore::RegistrableDomain&, const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_EstablishWorkerContextConnectionToNetworkProcess; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcessProxy_EstablishWorkerContextConnectionToNetworkProcessReply; }
    using AsyncReply = EstablishWorkerContextConnectionToNetworkProcessAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    EstablishWorkerContextConnectionToNetworkProcess(const WebCore::RegistrableDomain& registrableDomain, const PAL::SessionID& sessionID)
        : m_arguments(registrableDomain, sessionID)
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
class WorkerContextConnectionNoLongerNeeded {
public:
    using Arguments = std::tuple<const WebCore::ProcessIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_WorkerContextConnectionNoLongerNeeded; }
    static const bool isSync = false;

    explicit WorkerContextConnectionNoLongerNeeded(const WebCore::ProcessIdentifier& identifier)
        : m_arguments(identifier)
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
class RegisterServiceWorkerClientProcess {
public:
    using Arguments = std::tuple<const WebCore::ProcessIdentifier&, const WebCore::ProcessIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_RegisterServiceWorkerClientProcess; }
    static const bool isSync = false;

    RegisterServiceWorkerClientProcess(const WebCore::ProcessIdentifier& webProcessIdentifier, const WebCore::ProcessIdentifier& serviceWorkerProcessIdentifier)
        : m_arguments(webProcessIdentifier, serviceWorkerProcessIdentifier)
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
class UnregisterServiceWorkerClientProcess {
public:
    using Arguments = std::tuple<const WebCore::ProcessIdentifier&, const WebCore::ProcessIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_UnregisterServiceWorkerClientProcess; }
    static const bool isSync = false;

    UnregisterServiceWorkerClientProcess(const WebCore::ProcessIdentifier& webProcessIdentifier, const WebCore::ProcessIdentifier& serviceWorkerProcessIdentifier)
        : m_arguments(webProcessIdentifier, serviceWorkerProcessIdentifier)
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

class SetWebProcessHasUploads {
public:
    using Arguments = std::tuple<const WebCore::ProcessIdentifier&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_SetWebProcessHasUploads; }
    static const bool isSync = false;

    SetWebProcessHasUploads(const WebCore::ProcessIdentifier& processID, bool hasUpload)
        : m_arguments(processID, hasUpload)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class GetAppBoundDomains {
public:
    using Arguments = std::tuple<const PAL::SessionID&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_GetAppBoundDomains; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(HashSet<WebCore::RegistrableDomain>&&)>&&);
    static void cancelReply(CompletionHandler<void(HashSet<WebCore::RegistrableDomain>&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcessProxy_GetAppBoundDomainsReply; }
    using AsyncReply = GetAppBoundDomainsAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const HashSet<WebCore::RegistrableDomain>& appBoundDomains);
    using Reply = std::tuple<HashSet<WebCore::RegistrableDomain>&>;
    using ReplyArguments = std::tuple<HashSet<WebCore::RegistrableDomain>>;
    explicit GetAppBoundDomains(const PAL::SessionID& sessionID)
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

class RequestStorageSpace {
public:
    using Arguments = std::tuple<const PAL::SessionID&, const WebCore::ClientOrigin&, uint64_t, uint64_t, uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_RequestStorageSpace; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(Optional<uint64_t>&&)>&&);
    static void cancelReply(CompletionHandler<void(Optional<uint64_t>&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::NetworkProcessProxy_RequestStorageSpaceReply; }
    using AsyncReply = RequestStorageSpaceAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Optional<uint64_t>& newQuota);
    using Reply = std::tuple<Optional<uint64_t>&>;
    using ReplyArguments = std::tuple<Optional<uint64_t>>;
    RequestStorageSpace(const PAL::SessionID& sessionID, const WebCore::ClientOrigin& origin, uint64_t quota, uint64_t currentSize, uint64_t spaceRequired)
        : m_arguments(sessionID, origin, quota, currentSize, spaceRequired)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ResourceLoadDidSendRequest {
public:
    using Arguments = std::tuple<const WebKit::WebPageProxyIdentifier&, const WebKit::ResourceLoadInfo&, const WebCore::ResourceRequest&, const Optional<IPC::FormDataReference>&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_ResourceLoadDidSendRequest; }
    static const bool isSync = false;

    ResourceLoadDidSendRequest(const WebKit::WebPageProxyIdentifier& pageIdentifier, const WebKit::ResourceLoadInfo& resourceLoadInfo, const WebCore::ResourceRequest& request, const Optional<IPC::FormDataReference>& httpBody)
        : m_arguments(pageIdentifier, resourceLoadInfo, request, httpBody)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ResourceLoadDidPerformHTTPRedirection {
public:
    using Arguments = std::tuple<const WebKit::WebPageProxyIdentifier&, const WebKit::ResourceLoadInfo&, const WebCore::ResourceResponse&, const WebCore::ResourceRequest&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_ResourceLoadDidPerformHTTPRedirection; }
    static const bool isSync = false;

    ResourceLoadDidPerformHTTPRedirection(const WebKit::WebPageProxyIdentifier& pageIdentifier, const WebKit::ResourceLoadInfo& resourceLoadInfo, const WebCore::ResourceResponse& resourceResponse, const WebCore::ResourceRequest& request)
        : m_arguments(pageIdentifier, resourceLoadInfo, resourceResponse, request)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ResourceLoadDidReceiveChallenge {
public:
    using Arguments = std::tuple<const WebKit::WebPageProxyIdentifier&, const WebKit::ResourceLoadInfo&, const WebCore::AuthenticationChallenge&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_ResourceLoadDidReceiveChallenge; }
    static const bool isSync = false;

    ResourceLoadDidReceiveChallenge(const WebKit::WebPageProxyIdentifier& pageIdentifier, const WebKit::ResourceLoadInfo& resourceLoadInfo, const WebCore::AuthenticationChallenge& challenge)
        : m_arguments(pageIdentifier, resourceLoadInfo, challenge)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ResourceLoadDidReceiveResponse {
public:
    using Arguments = std::tuple<const WebKit::WebPageProxyIdentifier&, const WebKit::ResourceLoadInfo&, const WebCore::ResourceResponse&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_ResourceLoadDidReceiveResponse; }
    static const bool isSync = false;

    ResourceLoadDidReceiveResponse(const WebKit::WebPageProxyIdentifier& pageIdentifier, const WebKit::ResourceLoadInfo& resourceLoadInfo, const WebCore::ResourceResponse& response)
        : m_arguments(pageIdentifier, resourceLoadInfo, response)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ResourceLoadDidCompleteWithError {
public:
    using Arguments = std::tuple<const WebKit::WebPageProxyIdentifier&, const WebKit::ResourceLoadInfo&, const WebCore::ResourceResponse&, const WebCore::ResourceError&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkProcessProxy_ResourceLoadDidCompleteWithError; }
    static const bool isSync = false;

    ResourceLoadDidCompleteWithError(const WebKit::WebPageProxyIdentifier& pageIdentifier, const WebKit::ResourceLoadInfo& resourceLoadInfo, const WebCore::ResourceResponse& response, const WebCore::ResourceError& error)
        : m_arguments(pageIdentifier, resourceLoadInfo, response, error)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

} // namespace NetworkProcessProxy
} // namespace Messages
