/*
 * @file fetcher-session.h
 * @author XueShuming
 * @date 2021/11/17
 * @brief The fetcher session class.
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

#include "Connection.h"
#include "MessageReceiverMap.h"
#include "ProcessLauncher.h"
#include "ProcessIdentifier.h"

#include <wtf/ProcessID.h>
#include <wtf/SystemTracing.h>
#include <wtf/ThreadSafeRefCounted.h>

using namespace PurCFetcher;

class PcFetcherSession : public IPC::Connection::Client {
    WTF_MAKE_NONCOPYABLE(PcFetcherSession);

public:
    PcFetcherSession(uint64_t sessionId,
            IPC::Connection::Identifier connectionIdentifier);

    ~PcFetcherSession();

    IPC::Connection* connection() const
    {
        ASSERT(m_connection);
        return m_connection.get();
    }

    void addMessageReceiver(IPC::ReceiverName, IPC::MessageReceiver&);
    void addMessageReceiver(IPC::ReceiverName, uint64_t destinationID,
            IPC::MessageReceiver&);
    void removeMessageReceiver(IPC::ReceiverName, uint64_t destinationID);
    void removeMessageReceiver(IPC::ReceiverName);

    template <typename T>
    void addMessageReceiver(IPC::ReceiverName messageReceiverName,
            ObjectIdentifier<T> destinationID, IPC::MessageReceiver& receiver)
    {
        addMessageReceiver(messageReceiverName, destinationID.toUInt64(),
                receiver);
    }

    template <typename T>
    void removeMessageReceiver(IPC::ReceiverName messageReceiverName,
            ObjectIdentifier<T> destinationID)
    {
        removeMessageReceiver(messageReceiverName, destinationID.toUInt64());
    }

protected:
    bool dispatchMessage(IPC::Connection&, IPC::Decoder&);
    bool dispatchSyncMessage(IPC::Connection&, IPC::Decoder&,
            std::unique_ptr<IPC::Encoder>&);

    void didReceiveMessage(IPC::Connection&, IPC::Decoder&);
    void didReceiveSyncMessage(IPC::Connection&, IPC::Decoder&,
            std::unique_ptr<IPC::Encoder>&);
    void didClose(IPC::Connection&);
    void didReceiveInvalidMessage(IPC::Connection&, IPC::MessageName);
    const char* connectName(void) { return "PcFetcherSession"; }

private:
    uint64_t m_sessionId;
    RefPtr<IPC::Connection> m_connection;
    IPC::MessageReceiverMap m_messageReceiverMap;
};


#endif // ENABLE(LINK_PURC_FETCHER)

#endif /* not defined PURC_FETCHER_PROCESS_H */
