/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
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

#include "config.h"
#include "WebsitePoliciesData.h"

#include "ArgumentCoders.h"
#include "WebProcess.h"
#include <PurCFetcher/Frame.h>
#include <PurCFetcher/Page.h>

namespace PurCFetcher {

void WebsitePoliciesData::encode(IPC::Encoder& encoder) const
{
    encoder << contentBlockersEnabled;
    encoder << autoplayPolicy;
#if ENABLE(DEVICE_ORIENTATION)
    encoder << deviceOrientationAndMotionAccessState;
#endif
    encoder << allowedAutoplayQuirks;
    encoder << customHeaderFields;
    encoder << popUpPolicy;
    encoder << customUserAgent;
    encoder << customUserAgentAsSiteSpecificQuirks;
    encoder << customNavigatorPlatform;
    encoder << metaViewportPolicy;
    encoder << mediaSourcePolicy;
    encoder << simulatedMouseEventsDispatchPolicy;
    encoder << legacyOverflowScrollingTouchPolicy;
    encoder << allowContentChangeObserverQuirk;
    encoder << allowsContentJavaScript;
    encoder << mouseEventPolicy;
    encoder << idempotentModeAutosizingOnlyHonorsPercentages;
}

Optional<WebsitePoliciesData> WebsitePoliciesData::decode(IPC::Decoder& decoder)
{
    Optional<bool> contentBlockersEnabled;
    decoder >> contentBlockersEnabled;
    if (!contentBlockersEnabled)
        return WTF::nullopt;
    
    Optional<WebsiteAutoplayPolicy> autoplayPolicy;
    decoder >> autoplayPolicy;
    if (!autoplayPolicy)
        return WTF::nullopt;

#if ENABLE(DEVICE_ORIENTATION)
    Optional<PurCFetcher::DeviceOrientationOrMotionPermissionState> deviceOrientationAndMotionAccessState;
    decoder >> deviceOrientationAndMotionAccessState;
    if (!deviceOrientationAndMotionAccessState)
        return WTF::nullopt;
#endif
    
    Optional<OptionSet<WebsiteAutoplayQuirk>> allowedAutoplayQuirks;
    decoder >> allowedAutoplayQuirks;
    if (!allowedAutoplayQuirks)
        return WTF::nullopt;
    
    Optional<Vector<PurCFetcher::CustomHeaderFields>> customHeaderFields;
    decoder >> customHeaderFields;
    if (!customHeaderFields)
        return WTF::nullopt;

    Optional<WebsitePopUpPolicy> popUpPolicy;
    decoder >> popUpPolicy;
    if (!popUpPolicy)
        return WTF::nullopt;

    Optional<String> customUserAgent;
    decoder >> customUserAgent;
    if (!customUserAgent)
        return WTF::nullopt;

    Optional<String> customUserAgentAsSiteSpecificQuirks;
    decoder >> customUserAgentAsSiteSpecificQuirks;
    if (!customUserAgentAsSiteSpecificQuirks)
        return WTF::nullopt;

    Optional<String> customNavigatorPlatform;
    decoder >> customNavigatorPlatform;
    if (!customNavigatorPlatform)
        return WTF::nullopt;

    Optional<WebsiteMetaViewportPolicy> metaViewportPolicy;
    decoder >> metaViewportPolicy;
    if (!metaViewportPolicy)
        return WTF::nullopt;

    Optional<WebsiteMediaSourcePolicy> mediaSourcePolicy;
    decoder >> mediaSourcePolicy;
    if (!mediaSourcePolicy)
        return WTF::nullopt;
    
    Optional<WebsiteSimulatedMouseEventsDispatchPolicy> simulatedMouseEventsDispatchPolicy;
    decoder >> simulatedMouseEventsDispatchPolicy;
    if (!simulatedMouseEventsDispatchPolicy)
        return WTF::nullopt;

    Optional<WebsiteLegacyOverflowScrollingTouchPolicy> legacyOverflowScrollingTouchPolicy;
    decoder >> legacyOverflowScrollingTouchPolicy;
    if (!legacyOverflowScrollingTouchPolicy)
        return WTF::nullopt;

    Optional<bool> allowContentChangeObserverQuirk;
    decoder >> allowContentChangeObserverQuirk;
    if (!allowContentChangeObserverQuirk)
        return WTF::nullopt;

    Optional<PurCFetcher::AllowsContentJavaScript> allowsContentJavaScript;
    decoder >> allowsContentJavaScript;
    if (!allowsContentJavaScript)
        return WTF::nullopt;

    Optional<PurCFetcher::MouseEventPolicy> mouseEventPolicy;
    decoder >> mouseEventPolicy;
    if (!mouseEventPolicy)
        return WTF::nullopt;

    Optional<bool> idempotentModeAutosizingOnlyHonorsPercentages;
    decoder >> idempotentModeAutosizingOnlyHonorsPercentages;
    if (!idempotentModeAutosizingOnlyHonorsPercentages)
        return WTF::nullopt;

    return { {
        WTFMove(*contentBlockersEnabled),
        WTFMove(*allowedAutoplayQuirks),
        WTFMove(*autoplayPolicy),
#if ENABLE(DEVICE_ORIENTATION)
        WTFMove(*deviceOrientationAndMotionAccessState),
#endif
        WTFMove(*customHeaderFields),
        WTFMove(*popUpPolicy),
        WTFMove(*customUserAgent),
        WTFMove(*customUserAgentAsSiteSpecificQuirks),
        WTFMove(*customNavigatorPlatform),
        WTFMove(*metaViewportPolicy),
        WTFMove(*mediaSourcePolicy),
        WTFMove(*simulatedMouseEventsDispatchPolicy),
        WTFMove(*legacyOverflowScrollingTouchPolicy),
        WTFMove(*allowContentChangeObserverQuirk),
        WTFMove(*allowsContentJavaScript),
        WTFMove(*mouseEventPolicy),
        WTFMove(*idempotentModeAutosizingOnlyHonorsPercentages),
    } };
}

void WebsitePoliciesData::applyToDocumentLoader(WebsitePoliciesData&& websitePolicies, PurCFetcher::DocumentLoader& documentLoader)
{
    documentLoader.setCustomHeaderFields(WTFMove(websitePolicies.customHeaderFields));
    documentLoader.setCustomUserAgent(websitePolicies.customUserAgent);
    documentLoader.setCustomUserAgentAsSiteSpecificQuirks(websitePolicies.customUserAgentAsSiteSpecificQuirks);
    documentLoader.setCustomNavigatorPlatform(websitePolicies.customNavigatorPlatform);

#if ENABLE(DEVICE_ORIENTATION)
    documentLoader.setDeviceOrientationAndMotionAccessState(websitePolicies.deviceOrientationAndMotionAccessState);
#endif

    // Only setUserContentExtensionsEnabled if it hasn't already been disabled by reloading without content blockers.
    if (documentLoader.userContentExtensionsEnabled())
        documentLoader.setUserContentExtensionsEnabled(websitePolicies.contentBlockersEnabled);

    OptionSet<PurCFetcher::AutoplayQuirk> quirks;
    const auto& allowedQuirks = websitePolicies.allowedAutoplayQuirks;
    
    if (allowedQuirks.contains(WebsiteAutoplayQuirk::InheritedUserGestures))
        quirks.add(PurCFetcher::AutoplayQuirk::InheritedUserGestures);
    
    if (allowedQuirks.contains(WebsiteAutoplayQuirk::SynthesizedPauseEvents))
        quirks.add(PurCFetcher::AutoplayQuirk::SynthesizedPauseEvents);
    
    if (allowedQuirks.contains(WebsiteAutoplayQuirk::ArbitraryUserGestures))
        quirks.add(PurCFetcher::AutoplayQuirk::ArbitraryUserGestures);

    if (allowedQuirks.contains(WebsiteAutoplayQuirk::PerDocumentAutoplayBehavior))
        quirks.add(PurCFetcher::AutoplayQuirk::PerDocumentAutoplayBehavior);

    documentLoader.setAllowedAutoplayQuirks(quirks);

    switch (websitePolicies.autoplayPolicy) {
    case WebsiteAutoplayPolicy::Default:
        documentLoader.setAutoplayPolicy(PurCFetcher::AutoplayPolicy::Default);
        break;
    case WebsiteAutoplayPolicy::Allow:
        documentLoader.setAutoplayPolicy(PurCFetcher::AutoplayPolicy::Allow);
        break;
    case WebsiteAutoplayPolicy::AllowWithoutSound:
        documentLoader.setAutoplayPolicy(PurCFetcher::AutoplayPolicy::AllowWithoutSound);
        break;
    case WebsiteAutoplayPolicy::Deny:
        documentLoader.setAutoplayPolicy(PurCFetcher::AutoplayPolicy::Deny);
        break;
    }

    switch (websitePolicies.popUpPolicy) {
    case WebsitePopUpPolicy::Default:
        documentLoader.setPopUpPolicy(PurCFetcher::PopUpPolicy::Default);
        break;
    case WebsitePopUpPolicy::Allow:
        documentLoader.setPopUpPolicy(PurCFetcher::PopUpPolicy::Allow);
        break;
    case WebsitePopUpPolicy::Block:
        documentLoader.setPopUpPolicy(PurCFetcher::PopUpPolicy::Block);
        break;
    }

    switch (websitePolicies.metaViewportPolicy) {
    case WebsiteMetaViewportPolicy::Default:
        documentLoader.setMetaViewportPolicy(PurCFetcher::MetaViewportPolicy::Default);
        break;
    case WebsiteMetaViewportPolicy::Respect:
        documentLoader.setMetaViewportPolicy(PurCFetcher::MetaViewportPolicy::Respect);
        break;
    case WebsiteMetaViewportPolicy::Ignore:
        documentLoader.setMetaViewportPolicy(PurCFetcher::MetaViewportPolicy::Ignore);
        break;
    }

    switch (websitePolicies.mediaSourcePolicy) {
    case WebsiteMediaSourcePolicy::Default:
        documentLoader.setMediaSourcePolicy(PurCFetcher::MediaSourcePolicy::Default);
        break;
    case WebsiteMediaSourcePolicy::Disable:
        documentLoader.setMediaSourcePolicy(PurCFetcher::MediaSourcePolicy::Disable);
        break;
    case WebsiteMediaSourcePolicy::Enable:
        documentLoader.setMediaSourcePolicy(PurCFetcher::MediaSourcePolicy::Enable);
        break;
    }

    switch (websitePolicies.simulatedMouseEventsDispatchPolicy) {
    case WebsiteSimulatedMouseEventsDispatchPolicy::Default:
        documentLoader.setSimulatedMouseEventsDispatchPolicy(PurCFetcher::SimulatedMouseEventsDispatchPolicy::Default);
        break;
    case WebsiteSimulatedMouseEventsDispatchPolicy::Allow:
        documentLoader.setSimulatedMouseEventsDispatchPolicy(PurCFetcher::SimulatedMouseEventsDispatchPolicy::Allow);
        break;
    case WebsiteSimulatedMouseEventsDispatchPolicy::Deny:
        documentLoader.setSimulatedMouseEventsDispatchPolicy(PurCFetcher::SimulatedMouseEventsDispatchPolicy::Deny);
        break;
    }

    switch (websitePolicies.legacyOverflowScrollingTouchPolicy) {
    case WebsiteLegacyOverflowScrollingTouchPolicy::Default:
        documentLoader.setLegacyOverflowScrollingTouchPolicy(PurCFetcher::LegacyOverflowScrollingTouchPolicy::Default);
        break;
    case WebsiteLegacyOverflowScrollingTouchPolicy::Disable:
        documentLoader.setLegacyOverflowScrollingTouchPolicy(PurCFetcher::LegacyOverflowScrollingTouchPolicy::Disable);
        break;
    case WebsiteLegacyOverflowScrollingTouchPolicy::Enable:
        documentLoader.setLegacyOverflowScrollingTouchPolicy(PurCFetcher::LegacyOverflowScrollingTouchPolicy::Enable);
        break;
    }

    switch (websitePolicies.mouseEventPolicy) {
    case PurCFetcher::MouseEventPolicy::Default:
        documentLoader.setMouseEventPolicy(PurCFetcher::MouseEventPolicy::Default);
        break;
#if ENABLE(IOS_TOUCH_EVENTS)
    case PurCFetcher::MouseEventPolicy::SynthesizeTouchEvents:
        documentLoader.setMouseEventPolicy(PurCFetcher::MouseEventPolicy::SynthesizeTouchEvents);
        break;
#endif
    }

    documentLoader.setAllowContentChangeObserverQuirk(websitePolicies.allowContentChangeObserverQuirk);
    documentLoader.setIdempotentModeAutosizingOnlyHonorsPercentages(websitePolicies.idempotentModeAutosizingOnlyHonorsPercentages);

    auto* frame = documentLoader.frame();
    if (!frame)
        return;

    documentLoader.applyPoliciesToSettings();
}

}

