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

#include "config.h"

#include "NetworkConnectionToWebProcess.h"

#include "ArgumentCoders.h"
#include "ClientOrigin.h"
#include "Cookie.h"
#include "Decoder.h"
#include "DownloadID.h"
#include "FrameIdentifier.h"
#include "HandleMessage.h"
#include "MessagePortIdentifier.h"
#include "MessageWithMessagePorts.h"
#include "NavigatingToAppBoundDomain.h"
#include "NetworkConnectionToWebProcessMessages.h"
#include "NetworkLoadMetrics.h"
#include "NetworkResourceLoadParameters.h"
#include "PageIdentifier.h"
#if ENABLE(SERVICE_WORKER)
#include "RegistrableDomain.h"
#endif
#include "ResourceError.h"
#include "ResourceRequest.h"
#include "ResourceResponse.h"
#include "SameSiteInfo.h"
#include "ShouldRelaxThirdPartyCookieBlocking.h"
#include "WebCoreArgumentCoders.h"
#include "WebPageProxyIdentifier.h"
#include "WebSocketIdentifier.h"
#include <CookieJar.h>
#include <NetworkLoadInformation.h>
#include <NetworkStorageSession.h>
#if HAVE(COOKIE_CHANGE_LISTENER_API)
#include <wtf/HashSet.h>
#endif
#include <wtf/Optional.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace Messages {

namespace NetworkConnectionToWebProcess {

void PerformSynchronousLoad::send(std::unique_ptr<IPC::Encoder>&& encoder, IPC::Connection& connection, const PurCFetcher::ResourceError& error, const PurCFetcher::ResourceResponse& response, const Vector<char>& data)
{
    *encoder << error;
    *encoder << response;
    *encoder << data;
    connection.sendSyncReply(WTFMove(encoder));
}

void TestProcessIncomingSyncMessagesWhenWaitingForSyncReply::send(std::unique_ptr<IPC::Encoder>&& encoder, IPC::Connection& connection, bool handled)
{
    *encoder << handled;
    connection.sendSyncReply(WTFMove(encoder));
}

void CookiesForDOM::send(std::unique_ptr<IPC::Encoder>&& encoder, IPC::Connection& connection, const String& cookieString, bool didAccessSecureCookies)
{
    *encoder << cookieString;
    *encoder << didAccessSecureCookies;
    connection.sendSyncReply(WTFMove(encoder));
}

void CookieRequestHeaderFieldValue::send(std::unique_ptr<IPC::Encoder>&& encoder, IPC::Connection& connection, const String& cookieString, bool didAccessSecureCookies)
{
    *encoder << cookieString;
    *encoder << didAccessSecureCookies;
    connection.sendSyncReply(WTFMove(encoder));
}

void GetRawCookies::send(std::unique_ptr<IPC::Encoder>&& encoder, IPC::Connection& connection, const Vector<PurCFetcher::Cookie>& cookies)
{
    *encoder << cookies;
    connection.sendSyncReply(WTFMove(encoder));
}

void DomCookiesForHost::send(std::unique_ptr<IPC::Encoder>&& encoder, IPC::Connection& connection, const Vector<PurCFetcher::Cookie>& cookies)
{
    *encoder << cookies;
    connection.sendSyncReply(WTFMove(encoder));
}

void GetNetworkLoadInformationResponse::send(std::unique_ptr<IPC::Encoder>&& encoder, IPC::Connection& connection, const PurCFetcher::ResourceResponse& response)
{
    *encoder << response;
    connection.sendSyncReply(WTFMove(encoder));
}

void GetNetworkLoadIntermediateInformation::send(std::unique_ptr<IPC::Encoder>&& encoder, IPC::Connection& connection, const Vector<PurCFetcher::NetworkTransactionInformation>& transactions)
{
    *encoder << transactions;
    connection.sendSyncReply(WTFMove(encoder));
}

void TakeNetworkLoadInformationMetrics::send(std::unique_ptr<IPC::Encoder>&& encoder, IPC::Connection& connection, const PurCFetcher::NetworkLoadMetrics& networkMetrics)
{
    *encoder << networkMetrics;
    connection.sendSyncReply(WTFMove(encoder));
}

#if ENABLE(SERVICE_WORKER)

void EstablishSWContextConnection::callReply(IPC::Decoder& decoder, CompletionHandler<void()>&& completionHandler)
{
    UNUSED_PARAM(decoder);
    UNUSED_PARAM(completionHandler);
    completionHandler();
}

void EstablishSWContextConnection::cancelReply(CompletionHandler<void()>&& completionHandler)
{
    completionHandler();
}

void EstablishSWContextConnection::send(std::unique_ptr<IPC::Encoder>&& encoder, IPC::Connection& connection)
{
    connection.sendSyncReply(WTFMove(encoder));
}

#endif

void TakeAllMessagesForPort::callReply(IPC::Decoder& decoder, CompletionHandler<void(Vector<PurCFetcher::MessageWithMessagePorts>&&, uint64_t&&)>&& completionHandler)
{
    UNUSED_PARAM(decoder);
    UNUSED_PARAM(completionHandler);
    Optional<Vector<PurCFetcher::MessageWithMessagePorts>> messages;
    decoder >> messages;
    if (!messages) {
        ASSERT_NOT_REACHED();
        cancelReply(WTFMove(completionHandler));
        return;
    }
    Optional<uint64_t> messageBatchIdentifier;
    decoder >> messageBatchIdentifier;
    if (!messageBatchIdentifier) {
        ASSERT_NOT_REACHED();
        cancelReply(WTFMove(completionHandler));
        return;
    }
    completionHandler(WTFMove(*messages), WTFMove(*messageBatchIdentifier));
}

void TakeAllMessagesForPort::cancelReply(CompletionHandler<void(Vector<PurCFetcher::MessageWithMessagePorts>&&, uint64_t&&)>&& completionHandler)
{
    completionHandler(IPC::AsyncReplyError<Vector<PurCFetcher::MessageWithMessagePorts>>::create(), IPC::AsyncReplyError<uint64_t>::create());
}

void TakeAllMessagesForPort::send(std::unique_ptr<IPC::Encoder>&& encoder, IPC::Connection& connection, const Vector<PurCFetcher::MessageWithMessagePorts>& messages, uint64_t messageBatchIdentifier)
{
    *encoder << messages;
    *encoder << messageBatchIdentifier;
    connection.sendSyncReply(WTFMove(encoder));
}

void CheckRemotePortForActivity::callReply(IPC::Decoder& decoder, CompletionHandler<void(bool&&)>&& completionHandler)
{
    UNUSED_PARAM(decoder);
    UNUSED_PARAM(completionHandler);
    Optional<bool> hasActivity;
    decoder >> hasActivity;
    if (!hasActivity) {
        ASSERT_NOT_REACHED();
        cancelReply(WTFMove(completionHandler));
        return;
    }
    completionHandler(WTFMove(*hasActivity));
}

void CheckRemotePortForActivity::cancelReply(CompletionHandler<void(bool&&)>&& completionHandler)
{
    completionHandler(IPC::AsyncReplyError<bool>::create());
}

void CheckRemotePortForActivity::send(std::unique_ptr<IPC::Encoder>&& encoder, IPC::Connection& connection, bool hasActivity)
{
    *encoder << hasActivity;
    connection.sendSyncReply(WTFMove(encoder));
}

} // namespace NetworkConnectionToWebProcess

} // namespace Messages

namespace PurCFetcher {

void NetworkConnectionToWebProcess::didReceiveNetworkConnectionToWebProcessMessage(IPC::Connection& connection, IPC::Decoder& decoder)
{
    auto protectedThis = makeRef(*this);
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::ScheduleResourceLoad::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::ScheduleResourceLoad>(decoder, this, &NetworkConnectionToWebProcess::scheduleResourceLoad);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::LoadPing::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::LoadPing>(decoder, this, &NetworkConnectionToWebProcess::loadPing);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::RemoveLoadIdentifier::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::RemoveLoadIdentifier>(decoder, this, &NetworkConnectionToWebProcess::removeLoadIdentifier);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::PageLoadCompleted::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::PageLoadCompleted>(decoder, this, &NetworkConnectionToWebProcess::pageLoadCompleted);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::BrowsingContextRemoved::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::BrowsingContextRemoved>(decoder, this, &NetworkConnectionToWebProcess::browsingContextRemoved);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::PrefetchDNS::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::PrefetchDNS>(decoder, this, &NetworkConnectionToWebProcess::prefetchDNS);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::PreconnectTo::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::PreconnectTo>(decoder, this, &NetworkConnectionToWebProcess::preconnectTo);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::StartDownload::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::StartDownload>(decoder, this, &NetworkConnectionToWebProcess::startDownload);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::ConvertMainResourceLoadToDownload::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::ConvertMainResourceLoadToDownload>(decoder, this, &NetworkConnectionToWebProcess::convertMainResourceLoadToDownload);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::SetCookiesFromDOM::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::SetCookiesFromDOM>(decoder, this, &NetworkConnectionToWebProcess::setCookiesFromDOM);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::SetRawCookie::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::SetRawCookie>(decoder, this, &NetworkConnectionToWebProcess::setRawCookie);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::DeleteCookie::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::DeleteCookie>(decoder, this, &NetworkConnectionToWebProcess::deleteCookie);
        return;
    }
#if HAVE(COOKIE_CHANGE_LISTENER_API)
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::UnsubscribeFromCookieChangeNotifications::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::UnsubscribeFromCookieChangeNotifications>(decoder, this, &NetworkConnectionToWebProcess::unsubscribeFromCookieChangeNotifications);
        return;
    }
#endif
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::SetCaptureExtraNetworkLoadMetricsEnabled::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::SetCaptureExtraNetworkLoadMetricsEnabled>(decoder, this, &NetworkConnectionToWebProcess::setCaptureExtraNetworkLoadMetricsEnabled);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::CreateSocketStream::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::CreateSocketStream>(decoder, this, &NetworkConnectionToWebProcess::createSocketStream);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::CreateSocketChannel::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::CreateSocketChannel>(decoder, this, &NetworkConnectionToWebProcess::createSocketChannel);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::AddOriginAccessWhitelistEntry::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::AddOriginAccessWhitelistEntry>(decoder, this, &NetworkConnectionToWebProcess::addOriginAccessWhitelistEntry);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::RemoveOriginAccessWhitelistEntry::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::RemoveOriginAccessWhitelistEntry>(decoder, this, &NetworkConnectionToWebProcess::removeOriginAccessWhitelistEntry);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::ResetOriginAccessWhitelists::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::ResetOriginAccessWhitelists>(decoder, this, &NetworkConnectionToWebProcess::resetOriginAccessWhitelists);
        return;
    }
#if ENABLE(SERVICE_WORKER)
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::EstablishSWContextConnection::name()) {
        IPC::handleMessageAsync<Messages::NetworkConnectionToWebProcess::EstablishSWContextConnection>(connection, decoder, this, &NetworkConnectionToWebProcess::establishSWContextConnection);
        return;
    }
#endif
#if ENABLE(SERVICE_WORKER)
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::CloseSWContextConnection::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::CloseSWContextConnection>(decoder, this, &NetworkConnectionToWebProcess::closeSWContextConnection);
        return;
    }
#endif
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::UpdateQuotaBasedOnSpaceUsageForTesting::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::UpdateQuotaBasedOnSpaceUsageForTesting>(decoder, this, &NetworkConnectionToWebProcess::updateQuotaBasedOnSpaceUsageForTesting);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::CreateNewMessagePortChannel::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::CreateNewMessagePortChannel>(decoder, this, &NetworkConnectionToWebProcess::createNewMessagePortChannel);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::EntangleLocalPortInThisProcessToRemote::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::EntangleLocalPortInThisProcessToRemote>(decoder, this, &NetworkConnectionToWebProcess::entangleLocalPortInThisProcessToRemote);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::MessagePortDisentangled::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::MessagePortDisentangled>(decoder, this, &NetworkConnectionToWebProcess::messagePortDisentangled);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::MessagePortClosed::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::MessagePortClosed>(decoder, this, &NetworkConnectionToWebProcess::messagePortClosed);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::TakeAllMessagesForPort::name()) {
        IPC::handleMessageAsync<Messages::NetworkConnectionToWebProcess::TakeAllMessagesForPort>(connection, decoder, this, &NetworkConnectionToWebProcess::takeAllMessagesForPort);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::PostMessageToRemote::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::PostMessageToRemote>(decoder, this, &NetworkConnectionToWebProcess::postMessageToRemote);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::CheckRemotePortForActivity::name()) {
        IPC::handleMessageAsync<Messages::NetworkConnectionToWebProcess::CheckRemotePortForActivity>(connection, decoder, this, &NetworkConnectionToWebProcess::checkRemotePortForActivity);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::DidDeliverMessagePortMessages::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::DidDeliverMessagePortMessages>(decoder, this, &NetworkConnectionToWebProcess::didDeliverMessagePortMessages);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::RegisterURLSchemesAsCORSEnabled::name()) {
        IPC::handleMessage<Messages::NetworkConnectionToWebProcess::RegisterURLSchemesAsCORSEnabled>(decoder, this, &NetworkConnectionToWebProcess::registerURLSchemesAsCORSEnabled);
        return;
    }
    UNUSED_PARAM(connection);
    UNUSED_PARAM(decoder);
    ASSERT_NOT_REACHED();
}

void NetworkConnectionToWebProcess::didReceiveSyncNetworkConnectionToWebProcessMessage(IPC::Connection& connection, IPC::Decoder& decoder, std::unique_ptr<IPC::Encoder>& replyEncoder)
{
    auto protectedThis = makeRef(*this);
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::PerformSynchronousLoad::name()) {
        IPC::handleMessageSynchronous<Messages::NetworkConnectionToWebProcess::PerformSynchronousLoad>(connection, decoder, replyEncoder, this, &NetworkConnectionToWebProcess::performSynchronousLoad);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::TestProcessIncomingSyncMessagesWhenWaitingForSyncReply::name()) {
        IPC::handleMessageSynchronous<Messages::NetworkConnectionToWebProcess::TestProcessIncomingSyncMessagesWhenWaitingForSyncReply>(connection, decoder, replyEncoder, this, &NetworkConnectionToWebProcess::testProcessIncomingSyncMessagesWhenWaitingForSyncReply);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::CookiesForDOM::name()) {
        IPC::handleMessageSynchronous<Messages::NetworkConnectionToWebProcess::CookiesForDOM>(connection, decoder, replyEncoder, this, &NetworkConnectionToWebProcess::cookiesForDOM);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::CookieRequestHeaderFieldValue::name()) {
        IPC::handleMessageSynchronous<Messages::NetworkConnectionToWebProcess::CookieRequestHeaderFieldValue>(connection, decoder, replyEncoder, this, &NetworkConnectionToWebProcess::cookieRequestHeaderFieldValue);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::GetRawCookies::name()) {
        IPC::handleMessageSynchronous<Messages::NetworkConnectionToWebProcess::GetRawCookies>(connection, decoder, replyEncoder, this, &NetworkConnectionToWebProcess::getRawCookies);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::DomCookiesForHost::name()) {
        IPC::handleMessageSynchronous<Messages::NetworkConnectionToWebProcess::DomCookiesForHost>(connection, decoder, replyEncoder, this, &NetworkConnectionToWebProcess::domCookiesForHost);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::GetNetworkLoadInformationResponse::name()) {
        IPC::handleMessageSynchronous<Messages::NetworkConnectionToWebProcess::GetNetworkLoadInformationResponse>(connection, decoder, replyEncoder, this, &NetworkConnectionToWebProcess::getNetworkLoadInformationResponse);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::GetNetworkLoadIntermediateInformation::name()) {
        IPC::handleMessageSynchronous<Messages::NetworkConnectionToWebProcess::GetNetworkLoadIntermediateInformation>(connection, decoder, replyEncoder, this, &NetworkConnectionToWebProcess::getNetworkLoadIntermediateInformation);
        return;
    }
    if (decoder.messageName() == Messages::NetworkConnectionToWebProcess::TakeNetworkLoadInformationMetrics::name()) {
        IPC::handleMessageSynchronous<Messages::NetworkConnectionToWebProcess::TakeNetworkLoadInformationMetrics>(connection, decoder, replyEncoder, this, &NetworkConnectionToWebProcess::takeNetworkLoadInformationMetrics);
        return;
    }
    UNUSED_PARAM(connection);
    UNUSED_PARAM(decoder);
    UNUSED_PARAM(replyEncoder);
    ASSERT_NOT_REACHED();
}

} // namespace PurCFetcher

