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

#include <stdint.h>

#include <wtf/EnumTraits.h>

namespace IPC {

enum class ReceiverName : uint8_t {
    WebPage = 1
    , IPC = 2
    , AsyncReply = 3
    , Invalid = 4
};

enum class MessageName : uint16_t {
    WebPage_LoadURL = 1
    , WrappedAsyncMessageForTesting = 2
    , SyncMessageReply = 3
    , InitializeConnection = 4
    , LegacySessionState = 5
};

ReceiverName receiverName(MessageName);
const char* description(MessageName);
bool isValidMessageName(MessageName);

} // namespace IPC

namespace WTF {

template<>
class HasCustomIsValidEnum<IPC::MessageName> : public std::true_type { };
template<typename E, typename T, typename = std::enable_if_t<std::is_same<E, IPC::MessageName>::value>>
bool isValidEnum(T messageName)
{
    static_assert(sizeof(T) == sizeof(IPC::MessageName), "isValidEnum<MessageName> should only be called with 16-bit types");
    return IPC::isValidMessageName(static_cast<IPC::MessageName>(messageName));
};

} // namespace WTF