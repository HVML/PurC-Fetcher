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

#if ENABLE(WEB_RTC)

#include "ArgumentCoders.h"
#include "Connection.h"
#include "MDNSRegisterIdentifier.h"
#include "MessageNames.h"
#include "NetworkMDNSRegisterMessagesReplies.h"
#include <WebCore/DocumentIdentifier.h>
#include <wtf/Forward.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/text/WTFString.h>


namespace Messages {
namespace NetworkMDNSRegister {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::NetworkMDNSRegister;
}

class UnregisterMDNSNames {
public:
    using Arguments = std::tuple<const WebCore::DocumentIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkMDNSRegister_UnregisterMDNSNames; }
    static const bool isSync = false;

    explicit UnregisterMDNSNames(const WebCore::DocumentIdentifier& documentIdentifier)
        : m_arguments(documentIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterMDNSName {
public:
    using Arguments = std::tuple<const WebKit::MDNSRegisterIdentifier&, const WebCore::DocumentIdentifier&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::NetworkMDNSRegister_RegisterMDNSName; }
    static const bool isSync = false;

    RegisterMDNSName(const WebKit::MDNSRegisterIdentifier& requestIdentifier, const WebCore::DocumentIdentifier& documentIdentifier, const String& ipAddress)
        : m_arguments(requestIdentifier, documentIdentifier, ipAddress)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

} // namespace NetworkMDNSRegister
} // namespace Messages

#endif // ENABLE(WEB_RTC)
