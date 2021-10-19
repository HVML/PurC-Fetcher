/*
 * Copyright (C) 2018 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "MessagePortChannel.h"
#include "MessagePortChannelProvider.h"
#include "MessagePortIdentifier.h"
#include "ProcessIdentifier.h"
#include <wtf/HashMap.h>

namespace WebCore {

class MessagePortChannelRegistry {
public:
    using CheckProcessLocalPortForActivityCallback = Function<void(const MessagePortIdentifier&, ProcessIdentifier, CompletionHandler<void(MessagePortChannelProvider::HasActivity)>&&)>;
    WTF_EXPORT_DECLARATION explicit MessagePortChannelRegistry(CheckProcessLocalPortForActivityCallback&&);

    WTF_EXPORT_DECLARATION ~MessagePortChannelRegistry();
    
    WTF_EXPORT_DECLARATION void didCreateMessagePortChannel(const MessagePortIdentifier& port1, const MessagePortIdentifier& port2);
    WTF_EXPORT_DECLARATION void didEntangleLocalToRemote(const MessagePortIdentifier& local, const MessagePortIdentifier& remote, ProcessIdentifier);
    WTF_EXPORT_DECLARATION void didDisentangleMessagePort(const MessagePortIdentifier& local);
    WTF_EXPORT_DECLARATION void didCloseMessagePort(const MessagePortIdentifier& local);
    WTF_EXPORT_DECLARATION bool didPostMessageToRemote(MessageWithMessagePorts&&, const MessagePortIdentifier& remoteTarget);
    WTF_EXPORT_DECLARATION void takeAllMessagesForPort(const MessagePortIdentifier&, CompletionHandler<void(Vector<MessageWithMessagePorts>&&, Function<void()>&&)>&&);
    WTF_EXPORT_DECLARATION void checkRemotePortForActivity(const MessagePortIdentifier& remoteTarget, CompletionHandler<void(MessagePortChannelProvider::HasActivity)>&& callback);

    WTF_EXPORT_DECLARATION MessagePortChannel* existingChannelContainingPort(const MessagePortIdentifier&);

    WTF_EXPORT_DECLARATION void messagePortChannelCreated(MessagePortChannel&);
    WTF_EXPORT_DECLARATION void messagePortChannelDestroyed(MessagePortChannel&);

    void checkProcessLocalPortForActivity(const MessagePortIdentifier&, ProcessIdentifier, CompletionHandler<void(MessagePortChannelProvider::HasActivity)>&&);

private:
    HashMap<MessagePortIdentifier, MessagePortChannel*> m_openChannels;
    CheckProcessLocalPortForActivityCallback m_checkProcessLocalPortForActivityCallback;
};

} // namespace WebCore
