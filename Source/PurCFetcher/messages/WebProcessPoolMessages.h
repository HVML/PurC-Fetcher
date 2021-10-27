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
#include "WebProcessPoolMessagesReplies.h"
#include <wtf/Forward.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/text/WTFString.h>

namespace WebKit {
class UserData;
}

namespace Messages {
namespace WebProcessPool {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::WebProcessPool;
}

class HandleMessage {
public:
    using Arguments = std::tuple<const String&, const WebKit::UserData&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcessPool_HandleMessage; }
    static const bool isSync = false;

    HandleMessage(const String& messageName, const WebKit::UserData& messageBody)
        : m_arguments(messageName, messageBody)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class HandleSynchronousMessage {
public:
    using Arguments = std::tuple<const String&, const WebKit::UserData&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcessPool_HandleSynchronousMessage; }
    static const bool isSync = true;

    using DelayedReply = HandleSynchronousMessageDelayedReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const WebKit::UserData& returnData);
    using Reply = std::tuple<WebKit::UserData&>;
    using ReplyArguments = std::tuple<WebKit::UserData>;
    HandleSynchronousMessage(const String& messageName, const WebKit::UserData& messageBody)
        : m_arguments(messageName, messageBody)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if ENABLE(GAMEPAD)
class StartedUsingGamepads {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcessPool_StartedUsingGamepads; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if ENABLE(GAMEPAD)
class StoppedUsingGamepads {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcessPool_StoppedUsingGamepads; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

class ReportWebContentCPUTime {
public:
    using Arguments = std::tuple<const Seconds&, uint64_t>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcessPool_ReportWebContentCPUTime; }
    static const bool isSync = false;

    ReportWebContentCPUTime(const Seconds& cpuTime, uint64_t activityState)
        : m_arguments(cpuTime, activityState)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

} // namespace WebProcessPool
} // namespace Messages
