/*
 * @file fetcher-session.cpp
 * @author XueShuming
 * @date 2021/11/17
 * @brief The impl for fetcher session.
 *
 * Copyright (C) 2021 FMSoft <https://www.fmsoft.cn>
 *
 * This file is a part of PurC (short for Purring Cat), an HVML interpreter.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "config.h"
#include "fetcher-session.h"
#include "NetworkResourceLoadParameters.h"
#include "NetworkConnectionToWebProcessMessages.h"
#include "WebResourceLoaderMessages.h"

#include <wtf/RunLoop.h>

using namespace PurCFetcher;

PcFetcherSession::PcFetcherSession(uint64_t sessionId,
        IPC::Connection::Identifier identifier)
    : m_sessionId(sessionId)
    , m_connection(IPC::Connection::createClientConnection(identifier, *this))
{
    m_connection->open();
}

PcFetcherSession::~PcFetcherSession()
{
}

void PcFetcherSession::close()
{
    m_connection->invalidate();
}

purc_variant_t PcFetcherSession::requestAsync(
        const char* url,
        enum pcfetcher_request_method method,
        purc_variant_t params,
        uint32_t timeout,
        response_handler handler,
        void* ctxt)
{
    std::unique_ptr<WTF::URL> wurl = makeUnique<URL>(URL(), url);;
    ResourceRequest request;
    request.setURL(*wurl);
    request.setHTTPMethod("GET");

    NetworkResourceLoadParameters loadParameters;
    loadParameters.identifier = (uint64_t)ProcessIdentifier::generate().toUInt64();
    loadParameters.request = request;
    loadParameters.webPageProxyID = WebPageProxyIdentifier::generate();
    loadParameters.webPageID = PageIdentifier::generate();
    loadParameters.webFrameID = FrameIdentifier::generate();
    loadParameters.parentPID = getpid();

    fprintf(stderr, "................................before send request\n");
    m_connection->send(Messages::NetworkConnectionToWebProcess::ScheduleResourceLoad(
                loadParameters), 0);
    fprintf(stderr, "................................after send request\n");
    fprintf(stderr, "................................before wait\n");

    m_connection->waitForAndDispatchImmediately<Messages::WebResourceLoader::DidFinishResourceLoad>(0, 5_s, IPC::WaitForOption::InterruptWaitingIfSyncMessageArrives);
    fprintf(stderr, "................................after wait\n");

    UNUSED_PARAM(url);
    UNUSED_PARAM(method);
    UNUSED_PARAM(params);
    UNUSED_PARAM(timeout);
    UNUSED_PARAM(handler);
    UNUSED_PARAM(ctxt);
    return PURC_VARIANT_INVALID;
}

purc_rwstream_t PcFetcherSession::requestSync(
        const char* url,
        enum pcfetcher_request_method method,
        purc_variant_t params,
        uint32_t timeout,
        struct pcfetcher_resp_header *resp_header)
{
    UNUSED_PARAM(url);
    UNUSED_PARAM(method);
    UNUSED_PARAM(params);
    UNUSED_PARAM(timeout);
    UNUSED_PARAM(resp_header);
    return NULL;
}

void PcFetcherSession::addMessageReceiver(
        IPC::ReceiverName messageReceiverName,
        IPC::MessageReceiver& messageReceiver)
{
    m_messageReceiverMap.addMessageReceiver(messageReceiverName,
            messageReceiver);
}

void PcFetcherSession::addMessageReceiver(IPC::ReceiverName messageReceiverName,
        uint64_t destinationID, IPC::MessageReceiver& messageReceiver)
{
    m_messageReceiverMap.addMessageReceiver(messageReceiverName, destinationID,
            messageReceiver);
}

void PcFetcherSession::removeMessageReceiver(
        IPC::ReceiverName messageReceiverName, uint64_t destinationID)
{
    m_messageReceiverMap.removeMessageReceiver(messageReceiverName,
            destinationID);
}

void PcFetcherSession::removeMessageReceiver(
        IPC::ReceiverName messageReceiverName)
{
    m_messageReceiverMap.removeMessageReceiver(messageReceiverName);
}

bool PcFetcherSession::dispatchMessage(
        IPC::Connection& connection, IPC::Decoder& decoder)
{
    return m_messageReceiverMap.dispatchMessage(connection, decoder);
}

bool PcFetcherSession::dispatchSyncMessage(IPC::Connection& connection,
        IPC::Decoder& decoder, std::unique_ptr<IPC::Encoder>& replyEncoder)
{
    return m_messageReceiverMap.dispatchSyncMessage(connection, decoder,
            replyEncoder);
}

void PcFetcherSession::didClose(IPC::Connection&)
{
}

void PcFetcherSession::didReceiveInvalidMessage(IPC::Connection&,
        IPC::MessageName)
{
}

void PcFetcherSession::didReceiveMessage(IPC::Connection& connection,
        IPC::Decoder& decoder)
{
    dispatchMessage(connection, decoder);
    if (decoder.messageName() == Messages::WebResourceLoader::DidReceiveResponse::name()) {
        IPC::handleMessage<Messages::WebResourceLoader::DidReceiveResponse>(decoder, this, &PcFetcherSession::didReceiveResponse);
        return;
    }
    if (decoder.messageName() == Messages::WebResourceLoader::DidReceiveSharedBuffer::name()) {
        IPC::handleMessage<Messages::WebResourceLoader::DidReceiveSharedBuffer>(decoder, this, &PcFetcherSession::didReceiveSharedBuffer);
        return;
    }
    if (decoder.messageName() == Messages::WebResourceLoader::DidFinishResourceLoad::name()) {
        IPC::handleMessage<Messages::WebResourceLoader::DidFinishResourceLoad>(decoder, this, &PcFetcherSession::didFinishResourceLoad);
        return;
    }
}

void PcFetcherSession::didReceiveSyncMessage(IPC::Connection& connection,
        IPC::Decoder& decoder, std::unique_ptr<IPC::Encoder>& replyEncoder)
{
    didReceiveSyncMessage(connection, decoder, replyEncoder);
}

void PcFetcherSession::didReceiveResponse(const PurCFetcher::ResourceResponse& response, bool needsContinueDidReceiveResponseMessage)
{
    UNUSED_PARAM(response);
    UNUSED_PARAM(needsContinueDidReceiveResponseMessage);
    fprintf(stderr, "%s:%d:%s   url=%s\n", __FILE__, __LINE__, __func__, response.url().string().characters8());
    fprintf(stderr, "%s:%d:%s   code=%d\n", __FILE__, __LINE__, __func__, response.httpStatusCode());
    fprintf(stderr, "%s:%d:%s   mime=%s\n", __FILE__, __LINE__, __func__, response.mimeType().characters8());
    fprintf(stderr, "%s:%d:%s   content=%Ld\n", __FILE__, __LINE__, __func__, response.expectedContentLength());
}

void PcFetcherSession::didReceiveSharedBuffer(IPC::SharedBufferDataReference&& data, int64_t encodedDataLength)
{
    UNUSED_PARAM(data);
    UNUSED_PARAM(encodedDataLength);
    fprintf(stderr, "%s:%d:%s   encodedDataLength=%ld\n", __FILE__, __LINE__, __func__, encodedDataLength);
//    fprintf(stderr, "%s:%d:%s   data.size=%ld\n", __FILE__, __LINE__, __func__, data.size());
//    fprintf(stderr, "%s:%d:%s   data=%s\n", __FILE__, __LINE__, __func__, data.data());
}

void PcFetcherSession::didFinishResourceLoad(const NetworkLoadMetrics& networkLoadMetrics)
{
    UNUSED_PARAM(networkLoadMetrics);
    fprintf(stderr, "%s:%d:%s  complete=%d\n", __FILE__, __LINE__, __func__, networkLoadMetrics.isComplete());
}



