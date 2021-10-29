/*
 * Copyright (C) 2010-2016 Apple Inc. All rights reserved.
 * Copyright (C) 2020 Beijing FMSoft Technologies Co., Ltd.
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

#include <wtf/Forward.h>

namespace PurcFetcher {
class ResourceError;
class ResourceRequest;
class ResourceResponse;
}

namespace WebKit {

PurcFetcher::ResourceError cancelledError(const PurcFetcher::ResourceRequest&);
PurcFetcher::ResourceError blockedError(const PurcFetcher::ResourceRequest&);
PurcFetcher::ResourceError blockedByContentBlockerError(const PurcFetcher::ResourceRequest&);
PurcFetcher::ResourceError cannotShowURLError(const PurcFetcher::ResourceRequest&);
PurcFetcher::ResourceError wasBlockedByRestrictionsError(const PurcFetcher::ResourceRequest&);
PurcFetcher::ResourceError interruptedForPolicyChangeError(const PurcFetcher::ResourceRequest&);
PurcFetcher::ResourceError failedCustomProtocolSyncLoad(const PurcFetcher::ResourceRequest&);
#if ENABLE(CONTENT_FILTERING)
PurcFetcher::ResourceError blockedByContentFilterError(const PurcFetcher::ResourceRequest&);
#endif
PurcFetcher::ResourceError cannotShowMIMETypeError(const PurcFetcher::ResourceResponse&);
PurcFetcher::ResourceError fileDoesNotExistError(const PurcFetcher::ResourceResponse&);
PurcFetcher::ResourceError pluginWillHandleLoadError(const PurcFetcher::ResourceResponse&);
PurcFetcher::ResourceError internalError(const URL&);

#if USE(SOUP)
PurcFetcher::ResourceError downloadNetworkError(const URL&, const WTF::String&);
PurcFetcher::ResourceError downloadCancelledByUserError(const PurcFetcher::ResourceResponse&);
PurcFetcher::ResourceError downloadDestinationError(const PurcFetcher::ResourceResponse&, const WTF::String&);
#endif

#if PLATFORM(GTK) || PLATFORM(HBD)
PurcFetcher::ResourceError printError(const URL&, const WTF::String&);
PurcFetcher::ResourceError printerNotFoundError(const URL&);
PurcFetcher::ResourceError invalidPageRangeToPrint(const URL&);
#endif

} // namespace WebKit
