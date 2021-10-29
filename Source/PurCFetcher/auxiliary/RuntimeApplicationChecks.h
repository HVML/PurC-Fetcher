/*
 * Copyright (C) 2009-2020 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <wtf/Forward.h>

namespace PurcFetcher {

PURCFETCHER_EXPORT void setPresentingApplicationPID(int);
PURCFETCHER_EXPORT int presentingApplicationPID();

#if PLATFORM(WIN)
inline bool isInWebProcess() { return false; }
#elif !PLATFORM(COCOA)
inline bool isInWebProcess() { return true; }
#endif

#if PLATFORM(COCOA)

bool isInWebProcess();
bool isInNetworkProcess();

PURCFETCHER_EXPORT void setApplicationBundleIdentifier(const String&);
PURCFETCHER_EXPORT void setApplicationBundleIdentifierOverride(const String&);
PURCFETCHER_EXPORT String applicationBundleIdentifier();
PURCFETCHER_EXPORT void clearApplicationBundleIdentifierTestingOverride();

#if PLATFORM(MAC)

namespace MacApplication {

PURCFETCHER_EXPORT bool isAOLInstantMessenger();
PURCFETCHER_EXPORT bool isAdobeInstaller();
PURCFETCHER_EXPORT bool isAperture();
PURCFETCHER_EXPORT bool isAppleMail();
PURCFETCHER_EXPORT bool isIBooks();
PURCFETCHER_EXPORT bool isITunes();
PURCFETCHER_EXPORT bool isMicrosoftMessenger();
PURCFETCHER_EXPORT bool isMicrosoftMyDay();
PURCFETCHER_EXPORT bool isMicrosoftOutlook();
PURCFETCHER_EXPORT bool isMiniBrowser();
bool isQuickenEssentials();
PURCFETCHER_EXPORT bool isSafari();
bool isSolidStateNetworksDownloader();
PURCFETCHER_EXPORT bool isVersions();
PURCFETCHER_EXPORT bool isHRBlock();
PURCFETCHER_EXPORT bool isIAdProducer();
PURCFETCHER_EXPORT bool isEpsonSoftwareUpdater();

} // MacApplication

#endif // PLATFORM(MAC)

#if PLATFORM(IOS_FAMILY)

namespace IOSApplication {

PURCFETCHER_EXPORT bool isMobileMail();
PURCFETCHER_EXPORT bool isMailCompositionService();
PURCFETCHER_EXPORT bool isMobileSafari();
PURCFETCHER_EXPORT bool isSafariViewService();
PURCFETCHER_EXPORT bool isIMDb();
PURCFETCHER_EXPORT bool isWebBookmarksD();
PURCFETCHER_EXPORT bool isDumpRenderTree();
PURCFETCHER_EXPORT bool isMiniBrowser();
bool isMobileStore();
PURCFETCHER_EXPORT bool isJesusCalling();
PURCFETCHER_EXPORT bool isFamilyHealthApp();
bool isSpringBoard();
PURCFETCHER_EXPORT bool isWebProcess();
PURCFETCHER_EXPORT bool isIBooks();
bool isIBooksStorytime();
PURCFETCHER_EXPORT bool isTheSecretSocietyHiddenMystery();
PURCFETCHER_EXPORT bool isCardiogram();
PURCFETCHER_EXPORT bool isNike();
bool isMoviStarPlus();
PURCFETCHER_EXPORT bool isFirefox();
PURCFETCHER_EXPORT bool isAppleApplication();
PURCFETCHER_EXPORT bool isEvernote();
PURCFETCHER_EXPORT bool isEventbrite();
PURCFETCHER_EXPORT bool isDataActivation();
PURCFETCHER_EXPORT bool isNews();
PURCFETCHER_EXPORT bool isStocks();
PURCFETCHER_EXPORT bool isFeedly();
PURCFETCHER_EXPORT bool isPocketCity();
PURCFETCHER_EXPORT bool isEssentialSkeleton();

} // IOSApplication

#endif // PLATFORM(IOS_FAMILY)

#else

inline bool isInNetworkProcess() { return false; }

#endif // PLATFORM(COCOA)

} // namespace PurcFetcher
