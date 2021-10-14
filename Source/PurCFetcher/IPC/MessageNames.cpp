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
#include "MessageNames.h"
#include <wtf/Assertions.h>

namespace IPC {

const char* description(MessageName name)
{
    switch (name) {
    case MessageName::WebPage_LoadURL:
        return "WebPage::LoadURL";
    case MessageName::WrappedAsyncMessageForTesting:
        return "IPC::WrappedAsyncMessageForTesting";
    case MessageName::SyncMessageReply:
        return "IPC::SyncMessageReply";
    case MessageName::InitializeConnection:
        return "IPC::InitializeConnection";
    case MessageName::LegacySessionState:
        return "IPC::LegacySessionState";
    }
    ASSERT_NOT_REACHED();
    return "<invalid message name>";
}

ReceiverName receiverName(MessageName messageName)
{
    switch (messageName) {
    case MessageName::WebPage_LoadURL:
        return ReceiverName::WebPage;
    case MessageName::WrappedAsyncMessageForTesting:
    case MessageName::SyncMessageReply:
    case MessageName::InitializeConnection:
    case MessageName::LegacySessionState:
        return ReceiverName::IPC;
    }
    ASSERT_NOT_REACHED();
    return ReceiverName::Invalid;
}

bool isValidMessageName(MessageName messageName)
{
    if (messageName == IPC::MessageName::WebPage_LoadURL)
        return true;
    if (messageName == IPC::MessageName::WrappedAsyncMessageForTesting)
        return true;
    if (messageName == IPC::MessageName::SyncMessageReply)
        return true;
    if (messageName == IPC::MessageName::InitializeConnection)
        return true;
    if (messageName == IPC::MessageName::LegacySessionState)
        return true;
    return false;
};

} // namespace IPC
