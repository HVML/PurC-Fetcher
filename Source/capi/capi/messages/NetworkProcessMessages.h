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
#include "MessageNames.h"
#include <ProcessIdentifier.h>
#include <wtf/Optional.h>

namespace PAL {
class SessionID;
}

namespace PurCFetcher {
enum class HTTPCookieAcceptPolicy : uint8_t;
struct NetworkProcessCreationParameters;
}

namespace Messages {
namespace NetworkProcess {

using CreateNetworkConnectionToWebProcessDelayedReply = CompletionHandler<void(const Optional<IPC::Attachment>& connectionIdentifier, PurCFetcher::HTTPCookieAcceptPolicy cookieAcceptPolicy)>;

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

} // namespace NetworkProcess
} // namespace Messages
