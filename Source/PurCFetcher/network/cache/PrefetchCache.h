/*
 * Copyright (C) 2019 Igalia S.L.
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

#include "ResourceRequest.h"
#include "ResourceResponse.h"
#include "SharedBuffer.h"
#include "Timer.h"
#include <wtf/Deque.h>
#include <wtf/HashMap.h>
#include <wtf/URLHash.h>
#include <wtf/text/WTFString.h>

namespace PurCFetcher {

class PrefetchCache {
    WTF_MAKE_NONCOPYABLE(PrefetchCache);
    WTF_MAKE_FAST_ALLOCATED;
public:
    PrefetchCache();
    ~PrefetchCache();

    void clear();

    struct Entry {
        WTF_MAKE_STRUCT_FAST_ALLOCATED;
        Entry(PurCFetcher::ResourceResponse&&, RefPtr<PurCFetcher::SharedBuffer>&&);
        Entry(PurCFetcher::ResourceResponse&&, PurCFetcher::ResourceRequest&&);

        Ref<PurCFetcher::SharedBuffer> releaseBuffer() { return buffer.releaseNonNull(); }

        PurCFetcher::ResourceResponse response;
        RefPtr<PurCFetcher::SharedBuffer> buffer;
        PurCFetcher::ResourceRequest redirectRequest;
    };

    std::unique_ptr<Entry> take(const URL&);
    void store(const URL&, PurCFetcher::ResourceResponse&&, RefPtr<PurCFetcher::SharedBuffer>&&);
    void storeRedirect(const URL&, PurCFetcher::ResourceResponse&&, PurCFetcher::ResourceRequest&&);

private:
    void clearExpiredEntries();

    using PrefetchEntriesMap = HashMap<URL, std::unique_ptr<Entry>>;
    std::unique_ptr<PrefetchEntriesMap> m_sessionPrefetches;

    using SessionPrefetchExpirationList = Deque<std::tuple<URL, WallTime>>;
    SessionPrefetchExpirationList m_sessionExpirationList;

    PurCFetcher::Timer m_expirationTimer;
};

} // namespace PurCFetcher
