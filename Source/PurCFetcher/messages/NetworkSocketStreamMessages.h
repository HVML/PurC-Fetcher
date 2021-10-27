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
#include "NetworkSocketStreamMessagesReplies.h"
#include "CookieRequestHeaderFieldProxy.h"
#include <wtf/Forward.h>
#include <wtf/Optional.h>
#include <wtf/ThreadSafeRefCounted.h>

namespace IPC {
class DataReference;
}

namespace Messages {
namespace NetworkSocketStream {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::NetworkSocketStream;
}

class SendData {
public:
    using Arguments = std::tuple<const IPC::DataReference&, uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkSocketStream_SendData; }
    static const bool isSync = false;

    SendData(const IPC::DataReference& data, uint64_t identifier)
        : m_arguments(data, identifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SendHandshake {
public:
    using Arguments = std::tuple<const IPC::DataReference&, const Optional<WebCore::CookieRequestHeaderFieldProxy>&, uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkSocketStream_SendHandshake; }
    static const bool isSync = false;

    SendHandshake(const IPC::DataReference& data, const Optional<WebCore::CookieRequestHeaderFieldProxy>& headerFieldProxy, uint64_t identifier)
        : m_arguments(data, headerFieldProxy, identifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class Close {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkSocketStream_Close; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

} // namespace NetworkSocketStream
} // namespace Messages
