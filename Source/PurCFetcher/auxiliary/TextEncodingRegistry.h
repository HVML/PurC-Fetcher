/*
 * Copyright (C) 2006-2017 Apple Inc. All rights reserved.
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

#include <memory>
#include <wtf/Forward.h>

namespace PurCFetcher {

class TextCodec;
class TextEncoding;

// Use TextResourceDecoder::decode to decode resources, since it handles BOMs.
// Use TextEncoding::encode to encode, since it takes care of normalization.
PURCFETCHER_EXPORT std::unique_ptr<TextCodec> newTextCodec(const TextEncoding&);

// Only TextEncoding should use the following functions directly.
const char* atomCanonicalTextEncodingName(const char* alias);
const char* atomCanonicalTextEncodingName(const String&);
bool noExtendedTextEncodingNameUsed();
bool isJapaneseEncoding(const char* canonicalEncodingName);
bool shouldShowBackslashAsCurrencySymbolIn(const char* canonicalEncodingName);

PURCFETCHER_EXPORT String defaultTextEncodingNameForSystemLanguage();

} // namespace PurCFetcher
