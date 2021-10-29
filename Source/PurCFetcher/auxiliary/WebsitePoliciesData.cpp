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
#include <PurcFetcher/Frame.h>
#include <PurcFetcher/Page.h>

namespace WebKit {

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
    Optional<PurcFetcher::DeviceOrientationOrMotionPermissionState> deviceOrientationAndMotionAccessState;
    decoder >> deviceOrientationAndMotionAccessState;
    if (!deviceOrientationAndMotionAccessState)
        return WTF::nullopt;
#endif
    
    Optional<OptionSet<WebsiteAutoplayQuirk>> allowedAutoplayQuirks;
    decoder >> allowedAutoplayQuirks;
    if (!allowedAutoplayQuirks)
        return WTF::nullopt;
    
    Optional<Vector<PurcFetcher::CustomHeaderFields>> customHeaderFields;
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

    Optional<PurcFetcher::AllowsContentJavaScript> allowsContentJavaScript;
    decoder >> allowsContentJavaScript;
    if (!allowsContentJavaScript)
        return WTF::nullopt;

    Optional<PurcFetcher::MouseEventPolicy> mouseEventPolicy;
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

void WebsitePoliciesData::applyToDocumentLoader(WebsitePoliciesData&& websitePolicies, PurcFetcher::DocumentLoader& documentLoader)
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

    OptionSet<PurcFetcher::AutoplayQuirk> quirks;
    const auto& allowedQuirks = websitePolicies.allowedAutoplayQuirks;
    
    if (allowedQuirks.contains(WebsiteAutoplayQuirk::InheritedUserGestures))
        quirks.add(PurcFetcher::AutoplayQuirk::InheritedUserGestures);
    
    if (allowedQuirks.contains(WebsiteAutoplayQuirk::SynthesizedPauseEvents))
        quirks.add(PurcFetcher::AutoplayQuirk::SynthesizedPauseEvents);
    
    if (allowedQuirks.contains(WebsiteAutoplayQuirk::ArbitraryUserGestures))
        quirks.add(PurcFetcher::AutoplayQuirk::ArbitraryUserGestures);

    if (allowedQuirks.contains(WebsiteAutoplayQuirk::PerDocumentAutoplayBehavior))
        quirks.add(PurcFetcher::AutoplayQuirk::PerDocumentAutoplayBehavior);

    documentLoader.setAllowedAutoplayQuirks(quirks);

    switch (websitePolicies.autoplayPolicy) {
    case WebsiteAutoplayPolicy::Default:
        documentLoader.setAutoplayPolicy(PurcFetcher::AutoplayPolicy::Default);
        break;
    case WebsiteAutoplayPolicy::Allow:
        documentLoader.setAutoplayPolicy(PurcFetcher::AutoplayPolicy::Allow);
        break;
    case WebsiteAutoplayPolicy::AllowWithoutSound:
        documentLoader.setAutoplayPolicy(PurcFetcher::AutoplayPolicy::AllowWithoutSound);
        break;
    case WebsiteAutoplayPolicy::Deny:
        documentLoader.setAutoplayPolicy(PurcFetcher::AutoplayPolicy::Deny);
        break;
    }

    switch (websitePolicies.popUpPolicy) {
    case WebsitePopUpPolicy::Default:
        documentLoader.setPopUpPolicy(PurcFetcher::PopUpPolicy::Default);
        break;
    case WebsitePopUpPolicy::Allow:
        documentLoader.setPopUpPolicy(PurcFetcher::PopUpPolicy::Allow);
        break;
    case WebsitePopUpPolicy::Block:
        documentLoader.setPopUpPolicy(PurcFetcher::PopUpPolicy::Block);
        break;
    }

    switch (websitePolicies.metaViewportPolicy) {
    case WebsiteMetaViewportPolicy::Default:
        documentLoader.setMetaViewportPolicy(PurcFetcher::MetaViewportPolicy::Default);
        break;
    case WebsiteMetaViewportPolicy::Respect:
        documentLoader.setMetaViewportPolicy(PurcFetcher::MetaViewportPolicy::Respect);
        break;
    case WebsiteMetaViewportPolicy::Ignore:
        documentLoader.setMetaViewportPolicy(PurcFetcher::MetaViewportPolicy::Ignore);
        break;
    }

    switch (websitePolicies.mediaSourcePolicy) {
    case WebsiteMediaSourcePolicy::Default:
        documentLoader.setMediaSourcePolicy(PurcFetcher::MediaSourcePolicy::Default);
        break;
    case WebsiteMediaSourcePolicy::Disable:
        documentLoader.setMediaSourcePolicy(PurcFetcher::MediaSourcePolicy::Disable);
        break;
    case WebsiteMediaSourcePolicy::Enable:
        documentLoader.setMediaSourcePolicy(PurcFetcher::MediaSourcePolicy::Enable);
        break;
    }

    switch (websitePolicies.simulatedMouseEventsDispatchPolicy) {
    case WebsiteSimulatedMouseEventsDispatchPolicy::Default:
        documentLoader.setSimulatedMouseEventsDispatchPolicy(PurcFetcher::SimulatedMouseEventsDispatchPolicy::Default);
        break;
    case WebsiteSimulatedMouseEventsDispatchPolicy::Allow:
        documentLoader.setSimulatedMouseEventsDispatchPolicy(PurcFetcher::SimulatedMouseEventsDispatchPolicy::Allow);
        break;
    case WebsiteSimulatedMouseEventsDispatchPolicy::Deny:
        documentLoader.setSimulatedMouseEventsDispatchPolicy(PurcFetcher::SimulatedMouseEventsDispatchPolicy::Deny);
        break;
    }

    switch (websitePolicies.legacyOverflowScrollingTouchPolicy) {
    case WebsiteLegacyOverflowScrollingTouchPolicy::Default:
        documentLoader.setLegacyOverflowScrollingTouchPolicy(PurcFetcher::LegacyOverflowScrollingTouchPolicy::Default);
        break;
    case WebsiteLegacyOverflowScrollingTouchPolicy::Disable:
        documentLoader.setLegacyOverflowScrollingTouchPolicy(PurcFetcher::LegacyOverflowScrollingTouchPolicy::Disable);
        break;
    case WebsiteLegacyOverflowScrollingTouchPolicy::Enable:
        documentLoader.setLegacyOverflowScrollingTouchPolicy(PurcFetcher::LegacyOverflowScrollingTouchPolicy::Enable);
        break;
    }

    switch (websitePolicies.mouseEventPolicy) {
    case PurcFetcher::MouseEventPolicy::Default:
        documentLoader.setMouseEventPolicy(PurcFetcher::MouseEventPolicy::Default);
        break;
#if ENABLE(IOS_TOUCH_EVENTS)
    case PurcFetcher::MouseEventPolicy::SynthesizeTouchEvents:
        documentLoader.setMouseEventPolicy(PurcFetcher::MouseEventPolicy::SynthesizeTouchEvents);
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

