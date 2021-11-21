/*
 * @file fetcher-process.h
 * @author XueShuming
 * @date 2021/11/17
 * @brief The fetcher process class.
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

#ifndef PURC_FETCHER_PROCESS_H
#define PURC_FETCHER_PROCESS_H

#if ENABLE(LINK_PURC_FETCHER)

#include "fetcher-remote.h"
#include "fetcher-session.h"

#include "Connection.h"
#include "MessageReceiverMap.h"
#include "ProcessLauncher.h"
#include "ProcessIdentifier.h"

#include <wtf/ProcessID.h>
#include <wtf/SystemTracing.h>
#include <wtf/ThreadSafeRefCounted.h>

using namespace PurCFetcher;

class PcFetcherProcess : ProcessLauncher::Client, public IPC::Connection::Client {
    WTF_MAKE_NONCOPYABLE(PcFetcherProcess);

public:
    PcFetcherProcess(struct pcfetcher* fetcher,
            bool alwaysRunsAtBackgroundPriority = false);

    virtual ~PcFetcherProcess();

    void connect();
    void terminate();

    void initFetcherProcess();

    template<typename T> bool send(T&& message, uint64_t destinationID, OptionSet<IPC::SendOption> sendOptions = { });
    template<typename T> bool sendSync(T&& message, typename T::Reply&&, uint64_t destinationID, Seconds timeout = 1_s, OptionSet<IPC::SendSyncOption> sendSyncOptions = { });

    enum class ShouldStartProcessThrottlerActivity : bool { No, Yes };
    template<typename T, typename C> void sendWithAsyncReply(T&&, C&&, uint64_t destinationID = 0, OptionSet<IPC::SendOption> = { }, ShouldStartProcessThrottlerActivity = ShouldStartProcessThrottlerActivity::Yes);

    template<typename T, typename U>
    bool send(T&& message, ObjectIdentifier<U> destinationID, OptionSet<IPC::SendOption> sendOptions = { })
    {
        return send<T>(WTFMove(message), destinationID.toUInt64(), sendOptions);
    }

    template<typename T, typename U>
    bool sendSync(T&& message, typename T::Reply&& reply, ObjectIdentifier<U> destinationID, Seconds timeout = 1_s, OptionSet<IPC::SendSyncOption> sendSyncOptions = { })
    {
        return sendSync<T>(WTFMove(message), WTFMove(reply), destinationID.toUInt64(), timeout, sendSyncOptions);
    }

    IPC::Connection* connection() const
    {
        ASSERT(m_connection);
        return m_connection.get();
    }

    bool hasConnection(const IPC::Connection& connection) const
    {
        return m_connection == &connection;
    }

    enum class State {
        Launching,
        Running,
        Terminated,
    };
    State state() const;
    bool isLaunching() const { return state() == State::Launching; }
    bool wasTerminated() const;

    ProcessID processIdentifier() const { return m_processLauncher ? m_processLauncher->processIdentifier() : 0; }

    bool canSendMessage() const { return state() != State::Terminated;}
    bool sendMessage(std::unique_ptr<IPC::Encoder>, OptionSet<IPC::SendOption>, Optional<std::pair<CompletionHandler<void(IPC::Decoder*)>, uint64_t>>&& asyncReplyInfo = WTF::nullopt, ShouldStartProcessThrottlerActivity = ShouldStartProcessThrottlerActivity::Yes);

    void shutDownProcess();

    PurCFetcher::ProcessIdentifier coreProcessIdentifier() const { return m_processIdentifier; }

    void setProcessSuppressionEnabled(bool);

    PcFetcherSession* createSession(void);

    purc_variant_t requestAsync(
        const char* url,
        enum pcfetcher_request_method method,
        purc_variant_t params,
        uint32_t timeout,
        response_handler handler,
        void* ctxt);

    purc_rwstream_t requestSync(
        const char* url,
        enum pcfetcher_request_method method,
        purc_variant_t params,
        uint32_t timeout,
        struct pcfetcher_resp_header *resp_header);

    int checkResponse(uint32_t timeout_ms);

protected:
    // ProcessLauncher::Client
    void didFinishLaunching(ProcessLauncher*, IPC::Connection::Identifier) override;

    bool dispatchMessage(IPC::Connection&, IPC::Decoder&);
    bool dispatchSyncMessage(IPC::Connection&, IPC::Decoder&, std::unique_ptr<IPC::Encoder>&);

    void getLaunchOptions(ProcessLauncher::LaunchOptions&);

    struct PendingMessage {
        std::unique_ptr<IPC::Encoder> encoder;
        OptionSet<IPC::SendOption> sendOptions;
        Optional<std::pair<CompletionHandler<void(IPC::Decoder*)>, uint64_t>> asyncReplyInfo;
    };

    virtual bool shouldSendPendingMessage(const PendingMessage&) { return true; }

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&);
    void didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&, std::unique_ptr<IPC::Encoder>&);
    void didClose(IPC::Connection&);
    void didReceiveInvalidMessage(IPC::Connection&, IPC::MessageName);
    const char* connectionName(void) { return "PcFetcherProcess"; }

private:
    struct pcfetcher* m_fetcher;

    Vector<PendingMessage> m_pendingMessages;
    RefPtr<ProcessLauncher> m_processLauncher;
    RefPtr<IPC::Connection> m_connection;
    bool m_alwaysRunsAtBackgroundPriority { false };
    PurCFetcher::ProcessIdentifier m_processIdentifier { PurCFetcher::ProcessIdentifier::generate() };
};

template<typename T>
bool PcFetcherProcess::send(T&& message, uint64_t destinationID, OptionSet<IPC::SendOption> sendOptions)
{
    COMPILE_ASSERT(!T::isSync, AsyncMessageExpected);

    auto encoder = makeUnique<IPC::Encoder>(T::name(), destinationID);
    encoder->encode(message.arguments());

    return sendMessage(WTFMove(encoder), sendOptions);
}

template<typename U>
bool PcFetcherProcess::sendSync(U&& message, typename U::Reply&& reply, uint64_t destinationID, Seconds timeout, OptionSet<IPC::SendSyncOption> sendSyncOptions)
{
    COMPILE_ASSERT(U::isSync, SyncMessageExpected);

    if (!m_connection)
        return false;

    TraceScope scope(SyncMessageStart, SyncMessageEnd);

    return connection()->sendSync(std::forward<U>(message), WTFMove(reply), destinationID, timeout, sendSyncOptions);
}

template<typename T, typename C>
void PcFetcherProcess::sendWithAsyncReply(T&& message, C&& completionHandler, uint64_t destinationID, OptionSet<IPC::SendOption> sendOptions, ShouldStartProcessThrottlerActivity shouldStartProcessThrottlerActivity)
{
    COMPILE_ASSERT(!T::isSync, AsyncMessageExpected);

    auto encoder = makeUnique<IPC::Encoder>(T::name(), destinationID);
    uint64_t listenerID = IPC::nextAsyncReplyHandlerID();
    encoder->encode(listenerID);
    encoder->encode(message.arguments());
    sendMessage(WTFMove(encoder), sendOptions, {{ [completionHandler = WTFMove(completionHandler)] (IPC::Decoder* decoder) mutable {
        if (decoder && decoder->isValid())
            T::callReply(*decoder, WTFMove(completionHandler));
        else
            T::cancelReply(WTFMove(completionHandler));
    }, listenerID }}, shouldStartProcessThrottlerActivity);
}


#endif // ENABLE(LINK_PURC_FETCHER)

#endif /* not defined PURC_FETCHER_PROCESS_H */
