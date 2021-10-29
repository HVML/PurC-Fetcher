/*
 * Copyright (C) 2015 Apple Inc. All rights reserved.
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

#ifndef DownloadID_h
#define DownloadID_h

#include "ArgumentCoder.h"
#include "WebCoreArgumentCoders.h"
#include "Decoder.h"
#include "Encoder.h"
#include <wtf/HashTraits.h>

namespace PurcFetcher {

class DownloadID {
public:
    DownloadID()
    {
    }

    explicit DownloadID(uint64_t downloadID)
        : m_downloadID(downloadID)
    {
    }

    bool operator==(DownloadID other) const { return m_downloadID == other.m_downloadID; }
    bool operator!=(DownloadID other) const { return m_downloadID != other.m_downloadID; }

    explicit operator bool() const { return downloadID(); }

    uint64_t downloadID() const { return m_downloadID; }
private:
    uint64_t m_downloadID { 0 };
};

}

namespace IPC {
    
template<> struct ArgumentCoder<PurcFetcher::DownloadID> {
    static void encode(Encoder& encoder, const PurcFetcher::DownloadID& downloadID)
    {
        encoder << downloadID.downloadID();
    }
    static WARN_UNUSED_RETURN bool decode(Decoder& decoder, PurcFetcher::DownloadID& downloadID)
    {
        uint64_t id;
        if (!decoder.decode(id))
            return false;

        downloadID = PurcFetcher::DownloadID(id);
        
        return true;
    }
};

}

namespace WTF {
    
struct DownloadIDHash {
    static unsigned hash(const PurcFetcher::DownloadID& d) { return intHash(d.downloadID()); }
    static bool equal(const PurcFetcher::DownloadID& a, const PurcFetcher::DownloadID& b) { return a.downloadID() == b.downloadID(); }
    static const bool safeToCompareToEmptyOrDeleted = true;
};
template<> struct HashTraits<PurcFetcher::DownloadID> : GenericHashTraits<PurcFetcher::DownloadID> {
    static PurcFetcher::DownloadID emptyValue() { return { }; }
    
    static void constructDeletedValue(PurcFetcher::DownloadID& slot) { slot = PurcFetcher::DownloadID(std::numeric_limits<uint64_t>::max()); }
    static bool isDeletedValue(const PurcFetcher::DownloadID& slot) { return slot.downloadID() == std::numeric_limits<uint64_t>::max(); }
};
template<> struct DefaultHash<PurcFetcher::DownloadID> {
    typedef DownloadIDHash Hash;
};

}
#endif /* DownloadID_h */
