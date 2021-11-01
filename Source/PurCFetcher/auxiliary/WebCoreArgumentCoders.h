/*
 * Copyright (C) 2010-2020 Apple Inc. All rights reserved.
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

#include "ArgumentCoders.h"
#include "NetworkLoadMetrics.h"
#include "StoredCredentialsPolicy.h"
#include <wtf/EnumTraits.h>

namespace PurcFetcher {

class AuthenticationChallenge;
class CertificateInfo;
class ProtectionSpace;
class Credential;
class SecurityOrigin;
class SharedBuffer;
class ResourceError;
class ResourceRequest;
class ResourceResponse;
struct CacheQueryOptions;

#if USE(SOUP)
struct SoupNetworkProxySettings;
#endif

namespace DOMCacheEngine {
struct CacheInfo;
struct Record;
}


} // namespace PurcFetcher

namespace IPC {

template<> struct ArgumentCoder<PurcFetcher::AuthenticationChallenge> {
    static void encode(Encoder&, const PurcFetcher::AuthenticationChallenge&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, PurcFetcher::AuthenticationChallenge&);
};

template<> struct ArgumentCoder<PurcFetcher::ProtectionSpace> {
    static void encode(Encoder&, const PurcFetcher::ProtectionSpace&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, PurcFetcher::ProtectionSpace&);
    static void encodePlatformData(Encoder&, const PurcFetcher::ProtectionSpace&);
    static WARN_UNUSED_RETURN bool decodePlatformData(Decoder&, PurcFetcher::ProtectionSpace&);
};

template<> struct ArgumentCoder<PurcFetcher::Credential> {
    static void encode(Encoder&, const PurcFetcher::Credential&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, PurcFetcher::Credential&);
    static void encodePlatformData(Encoder&, const PurcFetcher::Credential&);
    static WARN_UNUSED_RETURN bool decodePlatformData(Decoder&, PurcFetcher::Credential&);
};

template<> struct ArgumentCoder<PurcFetcher::CertificateInfo> {
    static void encode(Encoder&, const PurcFetcher::CertificateInfo&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, PurcFetcher::CertificateInfo&);
};

template<> struct ArgumentCoder<RefPtr<PurcFetcher::SharedBuffer>> {
    static void encode(Encoder&, const RefPtr<PurcFetcher::SharedBuffer>&);
    static Optional<RefPtr<PurcFetcher::SharedBuffer>> decode(Decoder&);
};

template<> struct ArgumentCoder<Ref<PurcFetcher::SharedBuffer>> {
    static void encode(Encoder&, const Ref<PurcFetcher::SharedBuffer>&);
    static Optional<Ref<PurcFetcher::SharedBuffer>> decode(Decoder&);
};


template<> struct ArgumentCoder<PurcFetcher::ResourceError> {
    static void encode(Encoder&, const PurcFetcher::ResourceError&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, PurcFetcher::ResourceError&);
    static void encodePlatformData(Encoder&, const PurcFetcher::ResourceError&);
    static WARN_UNUSED_RETURN bool decodePlatformData(Decoder&, PurcFetcher::ResourceError&);
};

template<> struct ArgumentCoder<PurcFetcher::ResourceRequest> {
    static void encode(Encoder&, const PurcFetcher::ResourceRequest&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, PurcFetcher::ResourceRequest&);
    static void encodePlatformData(Encoder&, const PurcFetcher::ResourceRequest&);
    static WARN_UNUSED_RETURN bool decodePlatformData(Decoder&, PurcFetcher::ResourceRequest&);
};

template<> struct ArgumentCoder<PurcFetcher::CacheQueryOptions> {
    static void encode(Encoder&, const PurcFetcher::CacheQueryOptions&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, PurcFetcher::CacheQueryOptions&);
};

#if USE(SOUP)
template<> struct ArgumentCoder<PurcFetcher::SoupNetworkProxySettings> {
    static void encode(Encoder&, const PurcFetcher::SoupNetworkProxySettings&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, PurcFetcher::SoupNetworkProxySettings&);
};
#endif

template<> struct ArgumentCoder<Vector<RefPtr<PurcFetcher::SecurityOrigin>>> {
    static void encode(Encoder&, const Vector<RefPtr<PurcFetcher::SecurityOrigin>>&);
    static WARN_UNUSED_RETURN bool decode(Decoder&, Vector<RefPtr<PurcFetcher::SecurityOrigin>>&);
};

template<> struct ArgumentCoder<PurcFetcher::DOMCacheEngine::CacheInfo> {
    static void encode(Encoder&, const PurcFetcher::DOMCacheEngine::CacheInfo&);
    static Optional<PurcFetcher::DOMCacheEngine::CacheInfo> decode(Decoder&);
};

template<> struct ArgumentCoder<PurcFetcher::DOMCacheEngine::Record> {
    static void encode(Encoder&, const PurcFetcher::DOMCacheEngine::Record&);
    static Optional<PurcFetcher::DOMCacheEngine::Record> decode(Decoder&);
};

} // namespace IPC

namespace WTF {

template<> struct EnumTraits<PurcFetcher::NetworkLoadPriority> {
    using values = EnumValues<
        PurcFetcher::NetworkLoadPriority,
        PurcFetcher::NetworkLoadPriority::Low,
        PurcFetcher::NetworkLoadPriority::Medium,
        PurcFetcher::NetworkLoadPriority::High,
        PurcFetcher::NetworkLoadPriority::Unknown
    >;
};

template<> struct EnumTraits<PurcFetcher::StoredCredentialsPolicy> {
    using values = EnumValues<
        PurcFetcher::StoredCredentialsPolicy,
        PurcFetcher::StoredCredentialsPolicy::DoNotUse,
        PurcFetcher::StoredCredentialsPolicy::Use,
        PurcFetcher::StoredCredentialsPolicy::EphemeralStateless
    >;
};

} // namespace WTF
