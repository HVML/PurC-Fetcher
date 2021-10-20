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

#include "Attachment.h"
#include "LocalStorageDatabaseTracker.h"
#include "MessageNames.h"
#include "WebResourceLoadStatisticsStore.h"
#include <wtf/Forward.h>
#include <wtf/Optional.h>
#include <wtf/Vector.h>
#include <wtf/text/WTFString.h>

namespace WebCore {
enum class HTTPCookieAcceptPolicy : uint8_t;
}

namespace Messages {
namespace NetworkProcess {

using CreateNetworkConnectionToWebProcessAsyncReply = CompletionHandler<void(const Optional<IPC::Attachment>& connectionIdentifier, WebCore::HTTPCookieAcceptPolicy cookieAcceptPolicy)>;

using RenameOriginInWebsiteDataAsyncReply = CompletionHandler<void()>;

using SetAllowsAnySSLCertificateForWebSocketDelayedReply = CompletionHandler<void()>;

using SetCacheModelSynchronouslyForTestingDelayedReply = CompletionHandler<void()>;

using ProcessWillSuspendImminentlyForTestingSyncDelayedReply = CompletionHandler<void()>;

using PrepareToSuspendAsyncReply = CompletionHandler<void()>;

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using ClearPrevalentResourceAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using ClearUserInteractionAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using DumpResourceLoadStatisticsAsyncReply = CompletionHandler<void(const String& dumpedStatistics)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using UpdatePrevalentDomainsToBlockCookiesForAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using IsGrandfatheredAsyncReply = CompletionHandler<void(bool isGrandfathered)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using IsPrevalentResourceAsyncReply = CompletionHandler<void(bool isPrevalent)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using IsVeryPrevalentResourceAsyncReply = CompletionHandler<void(bool isVeryPrevalent)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetAgeCapForClientSideCookiesAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetLastSeenAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using MergeStatisticForTestingAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using InsertExpiredStatisticForTestingAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetPrevalentResourceAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetPrevalentResourceForDebugModeAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using IsResourceLoadStatisticsEphemeralAsyncReply = CompletionHandler<void(bool isEphemeral)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using HadUserInteractionAsyncReply = CompletionHandler<void(bool hadUserInteraction)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using IsRelationshipOnlyInDatabaseOnceAsyncReply = CompletionHandler<void(bool hadUserInteraction)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using HasLocalStorageAsyncReply = CompletionHandler<void(bool hadUserInteraction)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using GetAllStorageAccessEntriesAsyncReply = CompletionHandler<void(const Vector<String>& domains)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using IsRegisteredAsRedirectingToAsyncReply = CompletionHandler<void(bool isRedirectingTo)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using IsRegisteredAsSubFrameUnderAsyncReply = CompletionHandler<void(bool isSubframeUnder)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using IsRegisteredAsSubresourceUnderAsyncReply = CompletionHandler<void(bool isSubresourceUnder)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using DomainIDExistsInDatabaseAsyncReply = CompletionHandler<void(bool exists)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using LogUserInteractionAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using ResetParametersToDefaultValuesAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using ScheduleClearInMemoryAndPersistentAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using ScheduleCookieBlockingUpdateAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using ScheduleStatisticsAndDataRecordsProcessingAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using StatisticsDatabaseHasAllTablesAsyncReply = CompletionHandler<void(bool hasAllTables)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SubmitTelemetryAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetCacheMaxAgeCapForPrevalentResourcesAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetGrandfatheredAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetUseITPDatabaseAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using GetResourceLoadStatisticsDataSummaryAsyncReply = CompletionHandler<void(const Vector<WebKit::WebResourceLoadStatisticsStore::ThirdPartyData>& thirdPartyData)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetGrandfatheringTimeAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetMaxStatisticsEntriesAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetMinimumTimeBetweenDataRecordsRemovalAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetPruneEntriesDownToAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetShouldClassifyResourcesBeforeDataRecordsRemovalAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetNotifyPagesWhenDataRecordsWereScannedAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetIsRunningResourceLoadStatisticsTestAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetNotifyPagesWhenTelemetryWasCapturedAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetResourceLoadStatisticsDebugModeAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetVeryPrevalentResourceAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetSubframeUnderTopFrameDomainAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetSubresourceUnderTopFrameDomainAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetSubresourceUniqueRedirectToAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetSubresourceUniqueRedirectFromAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetTimeToLiveUserInteractionAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetTopFrameUniqueRedirectToAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetTopFrameUniqueRedirectFromAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using ResetCacheMaxAgeCapForPrevalentResourcesAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetCrossSiteLoadWithLinkDecorationForTestingAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using ResetCrossSiteLoadsWithLinkDecorationForTestingAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using DeleteCookiesForTestingAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using HasIsolatedSessionAsyncReply = CompletionHandler<void(bool hasIsolatedSession)>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetAppBoundDomainsForResourceLoadStatisticsAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetShouldDowngradeReferrerForTestingAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetThirdPartyCookieBlockingModeAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetShouldEnbleSameSiteStrictEnforcementForTestingAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetFirstPartyWebsiteDataRemovalModeForTestingAsyncReply = CompletionHandler<void()>;
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
using SetToSameSiteStrictCookiesForTestingAsyncReply = CompletionHandler<void()>;
#endif

using SyncLocalStorageDelayedReply = CompletionHandler<void()>;

using DumpAdClickAttributionAsyncReply = CompletionHandler<void(const String& adClickAttributionState)>;

using ClearAdClickAttributionAsyncReply = CompletionHandler<void()>;

using SetAdClickAttributionOverrideTimerForTestingAsyncReply = CompletionHandler<void()>;

using SetAdClickAttributionConversionURLForTestingAsyncReply = CompletionHandler<void()>;

using MarkAdClickAttributionsAsExpiredForTestingAsyncReply = CompletionHandler<void()>;

using GetLocalStorageOriginDetailsAsyncReply = CompletionHandler<void(const Vector<WebKit::LocalStorageDatabaseTracker::OriginDetails>& details)>;

using SetServiceWorkerFetchTimeoutForTestingDelayedReply = CompletionHandler<void()>;

using ResetServiceWorkerFetchTimeoutForTestingDelayedReply = CompletionHandler<void()>;

using ResetQuotaAsyncReply = CompletionHandler<void()>;

using HasAppBoundSessionAsyncReply = CompletionHandler<void(bool hasAppBoundSession)>;

using ClearAppBoundSessionAsyncReply = CompletionHandler<void()>;

#if PLATFORM(IOS_FAMILY) && !PLATFORM(MACCATALYST)
using ClearServiceWorkerEntitlementOverrideAsyncReply = CompletionHandler<void()>;
#endif

using UpdateBundleIdentifierAsyncReply = CompletionHandler<void()>;

using ClearBundleIdentifierAsyncReply = CompletionHandler<void()>;

} // namespace NetworkProcess
} // namespace Messages
