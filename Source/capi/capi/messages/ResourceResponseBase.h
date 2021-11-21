/*
 * Copyright (C) 2006, 2008, 2016 Apple Inc. All rights reserved.
 * Copyright (C) 2009 Google Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#pragma once

#include "CacheValidation.h"
#include "CertificateInfo.h"
#include "HTTPHeaderMap.h"
#include "NetworkLoadMetrics.h"
#include "ParsedContentRange.h"
#include <wtf/Box.h>
#include <wtf/EnumTraits.h>
#include <wtf/Markable.h>
#include <wtf/URL.h>
#include <wtf/WallTime.h>

namespace PurCFetcher {

class ResourceResponse;

bool isScriptAllowedByNosniff(const ResourceResponse&);

enum class UsedLegacyTLS : bool { No, Yes };
static constexpr unsigned bitWidthOfUsedLegacyTLS = 1;
static_assert(static_cast<unsigned>(UsedLegacyTLS::Yes) <= ((1U << bitWidthOfUsedLegacyTLS) - 1));

// Do not use this class directly, use the class ResourceResponse instead
class ResourceResponseBase {
    WTF_MAKE_FAST_ALLOCATED;
public:
    enum class Type : uint8_t { Basic, Cors, Default, Error, Opaque, Opaqueredirect };
    static constexpr unsigned bitWidthOfType = 3;
    enum class Tainting : uint8_t { Basic, Cors, Opaque, Opaqueredirect };
    static constexpr unsigned bitWidthOfTainting = 2;

    static bool isRedirectionStatusCode(int code) { return code == 301 || code == 302 || code == 303 || code == 307 || code == 308; }

    struct CrossThreadData {
        CrossThreadData(const CrossThreadData&) = delete;
        CrossThreadData& operator=(const CrossThreadData&) = delete;
        CrossThreadData() = default;
        CrossThreadData(CrossThreadData&&) = default;

        URL url;
        String mimeType;
        long long expectedContentLength;
        String textEncodingName;
        int httpStatusCode;
        String httpStatusText;
        String httpVersion;
        HTTPHeaderMap httpHeaderFields;
        Optional<NetworkLoadMetrics> networkLoadMetrics;
        Type type;
        Tainting tainting;
        bool isRedirected;
        bool isRangeRequested;
    };

    CrossThreadData crossThreadData() const;
    static ResourceResponse fromCrossThreadData(CrossThreadData&&);

    bool isNull() const { return m_isNull; }
    PURCFETCHER_EXPORT bool isInHTTPFamily() const;
    PURCFETCHER_EXPORT bool isSuccessful() const;

    PURCFETCHER_EXPORT const URL& url() const;
    PURCFETCHER_EXPORT void setURL(const URL&);

    PURCFETCHER_EXPORT const String& mimeType() const;
    PURCFETCHER_EXPORT void setMimeType(const String& mimeType);

    PURCFETCHER_EXPORT long long expectedContentLength() const;
    PURCFETCHER_EXPORT void setExpectedContentLength(long long expectedContentLength);

    PURCFETCHER_EXPORT const String& textEncodingName() const;
    PURCFETCHER_EXPORT void setTextEncodingName(const String& name);

    PURCFETCHER_EXPORT int httpStatusCode() const;
    PURCFETCHER_EXPORT void setHTTPStatusCode(int);
    PURCFETCHER_EXPORT bool isRedirection() const;

    PURCFETCHER_EXPORT const String& httpStatusText() const;
    PURCFETCHER_EXPORT void setHTTPStatusText(const String&);

    PURCFETCHER_EXPORT const String& httpVersion() const;
    PURCFETCHER_EXPORT void setHTTPVersion(const String&);
    PURCFETCHER_EXPORT bool isHTTP09() const;

    PURCFETCHER_EXPORT const HTTPHeaderMap& httpHeaderFields() const;
    void setHTTPHeaderFields(HTTPHeaderMap&&);

    enum class SanitizationType { Redirection, RemoveCookies, CrossOriginSafe };
    PURCFETCHER_EXPORT void sanitizeHTTPHeaderFields(SanitizationType);

    String httpHeaderField(const String& name) const;
    PURCFETCHER_EXPORT String httpHeaderField(HTTPHeaderName) const;
    PURCFETCHER_EXPORT void setHTTPHeaderField(const String& name, const String& value);
    PURCFETCHER_EXPORT void setHTTPHeaderField(HTTPHeaderName, const String& value);

    PURCFETCHER_EXPORT void addHTTPHeaderField(HTTPHeaderName, const String& value);
    PURCFETCHER_EXPORT void addHTTPHeaderField(const String& name, const String& value);

    // Instead of passing a string literal to any of these functions, just use a HTTPHeaderName instead.
    template<size_t length> String httpHeaderField(const char (&)[length]) const = delete;
    template<size_t length> void setHTTPHeaderField(const char (&)[length], const String&) = delete;
    template<size_t length> void addHTTPHeaderField(const char (&)[length], const String&) = delete;

    bool isMultipart() const { return mimeType() == "multipart/x-mixed-replace"; }

    PURCFETCHER_EXPORT bool isAttachment() const;
    PURCFETCHER_EXPORT bool isAttachmentWithFilename() const;
    PURCFETCHER_EXPORT String suggestedFilename() const;
    PURCFETCHER_EXPORT static String sanitizeSuggestedFilename(const String&);

    PURCFETCHER_EXPORT void includeCertificateInfo() const;
    const Optional<CertificateInfo>& certificateInfo() const { return m_certificateInfo; };
    bool usedLegacyTLS() const { return m_usedLegacyTLS == UsedLegacyTLS::Yes; }
    void setUsedLegacyTLS(UsedLegacyTLS used) { m_usedLegacyTLS = used; }
    
    // These functions return parsed values of the corresponding response headers.
    PURCFETCHER_EXPORT bool cacheControlContainsNoCache() const;
    PURCFETCHER_EXPORT bool cacheControlContainsNoStore() const;
    PURCFETCHER_EXPORT bool cacheControlContainsMustRevalidate() const;
    PURCFETCHER_EXPORT bool cacheControlContainsImmutable() const;
    PURCFETCHER_EXPORT bool hasCacheValidatorFields() const;
    PURCFETCHER_EXPORT Optional<Seconds> cacheControlMaxAge() const;
    PURCFETCHER_EXPORT Optional<Seconds> cacheControlStaleWhileRevalidate() const;
    PURCFETCHER_EXPORT Optional<WallTime> date() const;
    PURCFETCHER_EXPORT Optional<Seconds> age() const;
    PURCFETCHER_EXPORT Optional<WallTime> expires() const;
    PURCFETCHER_EXPORT Optional<WallTime> lastModified() const;
    const ParsedContentRange& contentRange() const;

    enum class Source : uint8_t { Unknown, Network, DiskCache, DiskCacheAfterValidation, MemoryCache, MemoryCacheAfterValidation, ServiceWorker, ApplicationCache, DOMCache, InspectorOverride };
    static constexpr unsigned bitWidthOfSource = 4;
    static_assert(static_cast<unsigned>(Source::InspectorOverride) <= ((1U << bitWidthOfSource) - 1));

    PURCFETCHER_EXPORT Source source() const;
    void setSource(Source source)
    {
        ASSERT(source != Source::Unknown);
        m_source = source;
    }

    // FIXME: This should be eliminated from ResourceResponse.
    // Network loading metrics should be delivered via didFinishLoad
    // and should not be part of the ResourceResponse.
    const NetworkLoadMetrics* deprecatedNetworkLoadMetricsOrNull() const
    {
        if (m_networkLoadMetrics)
            return m_networkLoadMetrics.get();
        return nullptr;
    }
    void setDeprecatedNetworkLoadMetrics(Box<NetworkLoadMetrics>&& metrics)
    {
        m_networkLoadMetrics = WTFMove(metrics);
    }

    // The ResourceResponse subclass may "shadow" this method to provide platform-specific memory usage information
    unsigned memoryUsage() const
    {
        // average size, mostly due to URL and Header Map strings
        return 1280;
    }

    PURCFETCHER_EXPORT void setType(Type);
    Type type() const { return m_type; }

    void setRedirected(bool isRedirected) { m_isRedirected = isRedirected; }
    bool isRedirected() const { return m_isRedirected; }

    void setTainting(Tainting tainting) { m_tainting = tainting; }
    Tainting tainting() const { return m_tainting; }

    enum class PerformExposeAllHeadersCheck : uint8_t { Yes, No };
    static ResourceResponse filter(const ResourceResponse&, PerformExposeAllHeadersCheck);

    PURCFETCHER_EXPORT static ResourceResponse syntheticRedirectResponse(const URL& fromURL, const URL& toURL);

    static bool compare(const ResourceResponse&, const ResourceResponse&);

    template<class Encoder> void encode(Encoder&) const;
    template<class Decoder> static WARN_UNUSED_RETURN bool decode(Decoder&, ResourceResponseBase&);

    bool isRangeRequested() const { return m_isRangeRequested; }
    void setAsRangeRequested() { m_isRangeRequested = true; }

    bool containsInvalidHTTPHeaders() const;

protected:
    enum InitLevel {
        Uninitialized,
        CommonFieldsOnly,
        AllFields
    };

    PURCFETCHER_EXPORT ResourceResponseBase();
    PURCFETCHER_EXPORT ResourceResponseBase(const URL&, const String& mimeType, long long expectedLength, const String& textEncodingName);

    PURCFETCHER_EXPORT void lazyInit(InitLevel) const;

    // The ResourceResponse subclass should shadow these functions to lazily initialize platform specific fields
    void platformLazyInit(InitLevel) { }
    CertificateInfo platformCertificateInfo() const { return CertificateInfo(); };
    String platformSuggestedFileName() const { return String(); }

    static bool platformCompare(const ResourceResponse&, const ResourceResponse&) { return true; }

private:
    void parseCacheControlDirectives() const;
    void updateHeaderParsedState(HTTPHeaderName);
    void sanitizeHTTPHeaderFieldsAccordingToTainting();

protected:
    URL m_url;
    AtomString m_mimeType;
    long long m_expectedContentLength { 0 };
    AtomString m_textEncodingName;
    AtomString m_httpStatusText;
    AtomString m_httpVersion;
    HTTPHeaderMap m_httpHeaderFields;
    Box<NetworkLoadMetrics> m_networkLoadMetrics;

    mutable Optional<CertificateInfo> m_certificateInfo;

private:
    mutable Markable<Seconds, Seconds::MarkableTraits> m_age;
    mutable Markable<WallTime, WallTime::MarkableTraits> m_date;
    mutable Markable<WallTime, WallTime::MarkableTraits> m_expires;
    mutable Markable<WallTime, WallTime::MarkableTraits> m_lastModified;
    mutable ParsedContentRange m_contentRange;
    mutable CacheControlDirectives m_cacheControlDirectives;

    mutable bool m_haveParsedCacheControlHeader : 1;
    mutable bool m_haveParsedAgeHeader : 1;
    mutable bool m_haveParsedDateHeader : 1;
    mutable bool m_haveParsedExpiresHeader : 1;
    mutable bool m_haveParsedLastModifiedHeader : 1;
    mutable bool m_haveParsedContentRangeHeader : 1;
    bool m_isRedirected : 1;
    bool m_isRangeRequested : 1;
protected:
    bool m_isNull : 1;
    unsigned m_initLevel : 3; // Controlled by ResourceResponse.
    mutable UsedLegacyTLS m_usedLegacyTLS : bitWidthOfUsedLegacyTLS;
private:
#if 0
    Tainting m_tainting : bitWidthOfTainting;
    Source m_source : bitWidthOfSource;
    Type m_type : bitWidthOfType;
#else
    Tainting m_tainting;
    Source m_source;
    Type m_type;
#endif
protected:
    short m_httpStatusCode { 0 };
};

inline bool operator==(const ResourceResponse& a, const ResourceResponse& b) { return ResourceResponseBase::compare(a, b); }
inline bool operator!=(const ResourceResponse& a, const ResourceResponse& b) { return !(a == b); }

template<class Encoder>
void ResourceResponseBase::encode(Encoder& encoder) const
{
    encoder << m_isNull;
    if (m_isNull)
        return;
    lazyInit(AllFields);

    encoder << m_url;
    encoder << m_mimeType;
    encoder << static_cast<int64_t>(m_expectedContentLength);
    encoder << m_textEncodingName;
    encoder << m_httpStatusText;
    encoder << m_httpVersion;
    encoder << m_httpHeaderFields;

    // We don't want to put the networkLoadMetrics info
    // into the disk cache, because we will never use the old info.
    if constexpr (Encoder::isIPCEncoder)
        encoder << m_networkLoadMetrics;

    encoder << m_httpStatusCode;
    encoder << m_certificateInfo;
    encoder << m_source;
    encoder << m_type;
    encoder << m_tainting;
    encoder << m_isRedirected;
    UsedLegacyTLS usedLegacyTLS = m_usedLegacyTLS;
    encoder << usedLegacyTLS;
    encoder << m_isRangeRequested;
}

template<class Decoder>
bool ResourceResponseBase::decode(Decoder& decoder, ResourceResponseBase& response)
{
    ASSERT(response.m_isNull);
    Optional<bool> responseIsNull;
    decoder >> responseIsNull;
    if (!responseIsNull)
        return false;
    if (*responseIsNull)
        return true;

    response.m_isNull = false;

    Optional<URL> url;
    decoder >> url;
    if (!url)
        return false;
    response.m_url = WTFMove(*url);

    Optional<String> mimeType;
    decoder >> mimeType;
    if (!mimeType)
        return false;
    response.m_mimeType = WTFMove(*mimeType);

    Optional<int64_t> expectedContentLength;
    decoder >> expectedContentLength;
    if (!expectedContentLength)
        return false;
    response.m_expectedContentLength = *expectedContentLength;

    Optional<AtomString> textEncodingName;
    decoder >> textEncodingName;
    if (!textEncodingName)
        return false;
    response.m_textEncodingName = WTFMove(*textEncodingName);

    Optional<AtomString> httpStatusText;
    decoder >> httpStatusText;
    if (!httpStatusText)
        return false;
    response.m_httpStatusText = WTFMove(*httpStatusText);

    Optional<AtomString> httpVersion;
    decoder >> httpVersion;
    if (!httpVersion)
        return false;
    response.m_httpVersion = WTFMove(*httpVersion);

    Optional<HTTPHeaderMap> httpHeaderFields;
    decoder >> httpHeaderFields;
    if (!httpHeaderFields)
        return false;
    response.m_httpHeaderFields = WTFMove(*httpHeaderFields);

    // The networkLoadMetrics info is only send over IPC and not stored in disk cache.
    if constexpr (Decoder::isIPCDecoder) {
        Optional<Box<NetworkLoadMetrics>> networkLoadMetrics;
        decoder >> networkLoadMetrics;
        if (!networkLoadMetrics)
            return false;
        response.m_networkLoadMetrics = WTFMove(*networkLoadMetrics);
    }

    Optional<short> httpStatusCode;
    decoder >> httpStatusCode;
    if (!httpStatusCode)
        return false;
    response.m_httpStatusCode = WTFMove(*httpStatusCode);

    Optional<Optional<CertificateInfo>> certificateInfo;
    decoder >> certificateInfo;
    if (!certificateInfo)
        return false;
    response.m_certificateInfo = WTFMove(*certificateInfo);

    Optional<Source> source;
    decoder >> source;
    if (!source)
        return false;
    response.m_source = WTFMove(*source);

    Optional<Type> type;
    decoder >> type;
    if (!type)
        return false;
    response.m_type = WTFMove(*type);

    Optional<Tainting> tainting;
    decoder >> tainting;
    if (!tainting)
        return false;
    response.m_tainting = WTFMove(*tainting);

    Optional<bool> isRedirected;
    decoder >> isRedirected;
    if (!isRedirected)
        return false;
    response.m_isRedirected = WTFMove(*isRedirected);

    Optional<UsedLegacyTLS> usedLegacyTLS;
    decoder >> usedLegacyTLS;
    if (!usedLegacyTLS)
        return false;
    response.m_usedLegacyTLS = WTFMove(*usedLegacyTLS);

    Optional<bool> isRangeRequested;
    decoder >> isRangeRequested;
    if (!isRangeRequested)
        return false;
    response.m_isRangeRequested = WTFMove(*isRangeRequested);

    return true;
}

} // namespace PurCFetcher

namespace WTF {

template<> struct EnumTraits<PurCFetcher::ResourceResponseBase::Type> {
    using values = EnumValues<
        PurCFetcher::ResourceResponseBase::Type,
        PurCFetcher::ResourceResponseBase::Type::Basic,
        PurCFetcher::ResourceResponseBase::Type::Cors,
        PurCFetcher::ResourceResponseBase::Type::Default,
        PurCFetcher::ResourceResponseBase::Type::Error,
        PurCFetcher::ResourceResponseBase::Type::Opaque,
        PurCFetcher::ResourceResponseBase::Type::Opaqueredirect
    >;
};

template<> struct EnumTraits<PurCFetcher::ResourceResponseBase::Tainting> {
    using values = EnumValues<
        PurCFetcher::ResourceResponseBase::Tainting,
        PurCFetcher::ResourceResponseBase::Tainting::Basic,
        PurCFetcher::ResourceResponseBase::Tainting::Cors,
        PurCFetcher::ResourceResponseBase::Tainting::Opaque,
        PurCFetcher::ResourceResponseBase::Tainting::Opaqueredirect
    >;
};


template<> struct EnumTraits<PurCFetcher::ResourceResponseBase::Source> {
    using values = EnumValues<
        PurCFetcher::ResourceResponseBase::Source,
        PurCFetcher::ResourceResponseBase::Source::Unknown,
        PurCFetcher::ResourceResponseBase::Source::Network,
        PurCFetcher::ResourceResponseBase::Source::DiskCache,
        PurCFetcher::ResourceResponseBase::Source::DiskCacheAfterValidation,
        PurCFetcher::ResourceResponseBase::Source::MemoryCache,
        PurCFetcher::ResourceResponseBase::Source::MemoryCacheAfterValidation,
        PurCFetcher::ResourceResponseBase::Source::ServiceWorker,
        PurCFetcher::ResourceResponseBase::Source::ApplicationCache,
        PurCFetcher::ResourceResponseBase::Source::DOMCache,
        PurCFetcher::ResourceResponseBase::Source::InspectorOverride
    >;
};

} // namespace WTF
