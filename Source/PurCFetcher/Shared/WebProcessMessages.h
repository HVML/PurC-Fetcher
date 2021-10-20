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

#include "ArgumentCoders.h"
#include "Connection.h"
#include "GamepadData.h"
#include "MessageNames.h"
#include "SandboxExtension.h"
#include "WebPageProxyIdentifier.h"
#include "WebProcessMessagesReplies.h"
#include "WebsiteDataType.h"
#include <WebCore/PageIdentifier.h>
#include <WebCore/PluginData.h>
#include <WebCore/RegistrableDomain.h>
#include <WebCore/SecurityOriginData.h>
#include <wtf/Forward.h>
#include <wtf/HashMap.h>
#include <wtf/HashSet.h>
#include <wtf/OptionSet.h>
#include <wtf/Optional.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/Vector.h>
#include <wtf/WallTime.h>
#include <wtf/text/WTFString.h>

namespace IPC {
class DataReference;
}

namespace WebCore {
class RegistrableDomain;
enum class ThirdPartyCookieBlockingMode : uint8_t;
struct BackForwardItemIdentifier;
struct MockMediaDevice;
struct PrewarmInformation;
struct ScreenProperties;
}

namespace WebKit {
class GamepadData;
class MediaDeviceSandboxExtensions;
class UserData;
enum class CacheModel : uint8_t;
struct ServiceWorkerInitializationData;
struct TextCheckerState;
struct UserMessage;
struct WebPageCreationParameters;
struct WebPreferencesStore;
struct WebProcessCreationParameters;
struct WebProcessDataStoreParameters;
struct WebsiteData;
}

namespace Messages {
namespace WebProcess {

static inline IPC::ReceiverName messageReceiverName()
{
    return IPC::ReceiverName::WebProcess;
}

class InitializeWebProcess {
public:
    using Arguments = std::tuple<const WebKit::WebProcessCreationParameters&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_InitializeWebProcess; }
    static const bool isSync = false;

    explicit InitializeWebProcess(const WebKit::WebProcessCreationParameters& processCreationParameters)
        : m_arguments(processCreationParameters)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetWebsiteDataStoreParameters {
public:
    using Arguments = std::tuple<const WebKit::WebProcessDataStoreParameters&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetWebsiteDataStoreParameters; }
    static const bool isSync = false;

    explicit SetWebsiteDataStoreParameters(const WebKit::WebProcessDataStoreParameters& parameters)
        : m_arguments(parameters)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class CreateWebPage {
public:
    using Arguments = std::tuple<const WebCore::PageIdentifier&, const WebKit::WebPageCreationParameters&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_CreateWebPage; }
    static const bool isSync = false;

    CreateWebPage(const WebCore::PageIdentifier& newPageID, const WebKit::WebPageCreationParameters& pageCreationParameters)
        : m_arguments(newPageID, pageCreationParameters)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class PrewarmGlobally {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_PrewarmGlobally; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class PrewarmWithDomainInformation {
public:
    using Arguments = std::tuple<const WebCore::PrewarmInformation&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_PrewarmWithDomainInformation; }
    static const bool isSync = false;

    explicit PrewarmWithDomainInformation(const WebCore::PrewarmInformation& prewarmInformation)
        : m_arguments(prewarmInformation)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetCacheModel {
public:
    using Arguments = std::tuple<WebKit::CacheModel>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetCacheModel; }
    static const bool isSync = false;

    explicit SetCacheModel(WebKit::CacheModel cacheModel)
        : m_arguments(cacheModel)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterURLSchemeAsEmptyDocument {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_RegisterURLSchemeAsEmptyDocument; }
    static const bool isSync = false;

    explicit RegisterURLSchemeAsEmptyDocument(const String& scheme)
        : m_arguments(scheme)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterURLSchemeAsSecure {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_RegisterURLSchemeAsSecure; }
    static const bool isSync = false;

    explicit RegisterURLSchemeAsSecure(const String& scheme)
        : m_arguments(scheme)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterURLSchemeAsBypassingContentSecurityPolicy {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_RegisterURLSchemeAsBypassingContentSecurityPolicy; }
    static const bool isSync = false;

    explicit RegisterURLSchemeAsBypassingContentSecurityPolicy(const String& scheme)
        : m_arguments(scheme)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetDomainRelaxationForbiddenForURLScheme {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetDomainRelaxationForbiddenForURLScheme; }
    static const bool isSync = false;

    explicit SetDomainRelaxationForbiddenForURLScheme(const String& scheme)
        : m_arguments(scheme)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterURLSchemeAsLocal {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_RegisterURLSchemeAsLocal; }
    static const bool isSync = false;

    explicit RegisterURLSchemeAsLocal(const String& scheme)
        : m_arguments(scheme)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterURLSchemeAsNoAccess {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_RegisterURLSchemeAsNoAccess; }
    static const bool isSync = false;

    explicit RegisterURLSchemeAsNoAccess(const String& scheme)
        : m_arguments(scheme)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterURLSchemeAsDisplayIsolated {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_RegisterURLSchemeAsDisplayIsolated; }
    static const bool isSync = false;

    explicit RegisterURLSchemeAsDisplayIsolated(const String& scheme)
        : m_arguments(scheme)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterURLSchemeAsCORSEnabled {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_RegisterURLSchemeAsCORSEnabled; }
    static const bool isSync = false;

    explicit RegisterURLSchemeAsCORSEnabled(const String& scheme)
        : m_arguments(scheme)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterURLSchemeAsCachePartitioned {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_RegisterURLSchemeAsCachePartitioned; }
    static const bool isSync = false;

    explicit RegisterURLSchemeAsCachePartitioned(const String& scheme)
        : m_arguments(scheme)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RegisterURLSchemeAsCanDisplayOnlyIfCanRequest {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_RegisterURLSchemeAsCanDisplayOnlyIfCanRequest; }
    static const bool isSync = false;

    explicit RegisterURLSchemeAsCanDisplayOnlyIfCanRequest(const String& scheme)
        : m_arguments(scheme)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetDefaultRequestTimeoutInterval {
public:
    using Arguments = std::tuple<double>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetDefaultRequestTimeoutInterval; }
    static const bool isSync = false;

    explicit SetDefaultRequestTimeoutInterval(double timeoutInterval)
        : m_arguments(timeoutInterval)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetAlwaysUsesComplexTextCodePath {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetAlwaysUsesComplexTextCodePath; }
    static const bool isSync = false;

    explicit SetAlwaysUsesComplexTextCodePath(bool alwaysUseComplexText)
        : m_arguments(alwaysUseComplexText)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetShouldUseFontSmoothing {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetShouldUseFontSmoothing; }
    static const bool isSync = false;

    explicit SetShouldUseFontSmoothing(bool useFontSmoothing)
        : m_arguments(useFontSmoothing)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetResourceLoadStatisticsEnabled {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetResourceLoadStatisticsEnabled; }
    static const bool isSync = false;

    explicit SetResourceLoadStatisticsEnabled(bool resourceLoadStatisticsEnabled)
        : m_arguments(resourceLoadStatisticsEnabled)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ClearResourceLoadStatistics {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_ClearResourceLoadStatistics; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class UserPreferredLanguagesChanged {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_UserPreferredLanguagesChanged; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class FullKeyboardAccessModeChanged {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_FullKeyboardAccessModeChanged; }
    static const bool isSync = false;

    explicit FullKeyboardAccessModeChanged(bool fullKeyboardAccessEnabled)
        : m_arguments(fullKeyboardAccessEnabled)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DidAddPlugInAutoStartOriginHash {
public:
    using Arguments = std::tuple<uint32_t, const WallTime&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_DidAddPlugInAutoStartOriginHash; }
    static const bool isSync = false;

    DidAddPlugInAutoStartOriginHash(uint32_t hash, const WallTime& expirationTime)
        : m_arguments(hash, expirationTime)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ResetPlugInAutoStartOriginHashes {
public:
    using Arguments = std::tuple<const HashMap<uint32_t,WallTime>&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_ResetPlugInAutoStartOriginHashes; }
    static const bool isSync = false;

    explicit ResetPlugInAutoStartOriginHashes(const HashMap<uint32_t,WallTime>& hashes)
        : m_arguments(hashes)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetPluginLoadClientPolicy {
public:
    using Arguments = std::tuple<const WebCore::PluginLoadClientPolicy&, const String&, const String&, const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetPluginLoadClientPolicy; }
    static const bool isSync = false;

    SetPluginLoadClientPolicy(const WebCore::PluginLoadClientPolicy& policy, const String& host, const String& bundleIdentifier, const String& versionString)
        : m_arguments(policy, host, bundleIdentifier, versionString)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ResetPluginLoadClientPolicies {
public:
    using Arguments = std::tuple<const HashMap<String, HashMap<String, HashMap<String, WebCore::PluginLoadClientPolicy>>>&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_ResetPluginLoadClientPolicies; }
    static const bool isSync = false;

    explicit ResetPluginLoadClientPolicies(const HashMap<String, HashMap<String, HashMap<String, WebCore::PluginLoadClientPolicy>>>& pluginLoadClientPolicies)
        : m_arguments(pluginLoadClientPolicies)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ClearPluginClientPolicies {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_ClearPluginClientPolicies; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class RefreshPlugins {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_RefreshPlugins; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class StartMemorySampler {
public:
    using Arguments = std::tuple<const WebKit::SandboxExtension::Handle&, const String&, double>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_StartMemorySampler; }
    static const bool isSync = false;

    StartMemorySampler(const WebKit::SandboxExtension::Handle& sampleLogFileHandle, const String& sampleLogFilePath, double interval)
        : m_arguments(sampleLogFileHandle, sampleLogFilePath, interval)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class StopMemorySampler {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_StopMemorySampler; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetTextCheckerState {
public:
    using Arguments = std::tuple<const WebKit::TextCheckerState&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetTextCheckerState; }
    static const bool isSync = false;

    explicit SetTextCheckerState(const WebKit::TextCheckerState& textCheckerState)
        : m_arguments(textCheckerState)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetEnhancedAccessibility {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetEnhancedAccessibility; }
    static const bool isSync = false;

    explicit SetEnhancedAccessibility(bool flag)
        : m_arguments(flag)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class GarbageCollectJavaScriptObjects {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_GarbageCollectJavaScriptObjects; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetJavaScriptGarbageCollectorTimerEnabled {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetJavaScriptGarbageCollectorTimerEnabled; }
    static const bool isSync = false;

    explicit SetJavaScriptGarbageCollectorTimerEnabled(bool enable)
        : m_arguments(enable)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetInjectedBundleParameter {
public:
    using Arguments = std::tuple<const String&, const IPC::DataReference&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetInjectedBundleParameter; }
    static const bool isSync = false;

    SetInjectedBundleParameter(const String& parameter, const IPC::DataReference& value)
        : m_arguments(parameter, value)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetInjectedBundleParameters {
public:
    using Arguments = std::tuple<const IPC::DataReference&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetInjectedBundleParameters; }
    static const bool isSync = false;

    explicit SetInjectedBundleParameters(const IPC::DataReference& parameters)
        : m_arguments(parameters)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class HandleInjectedBundleMessage {
public:
    using Arguments = std::tuple<const String&, const WebKit::UserData&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_HandleInjectedBundleMessage; }
    static const bool isSync = false;

    HandleInjectedBundleMessage(const String& messageName, const WebKit::UserData& messageBody)
        : m_arguments(messageName, messageBody)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class FetchWebsiteData {
public:
    using Arguments = std::tuple<const OptionSet<WebKit::WebsiteDataType>&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_FetchWebsiteData; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(WebKit::WebsiteData&&)>&&);
    static void cancelReply(CompletionHandler<void(WebKit::WebsiteData&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebProcess_FetchWebsiteDataReply; }
    using AsyncReply = FetchWebsiteDataAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const WebKit::WebsiteData& websiteData);
    using Reply = std::tuple<WebKit::WebsiteData&>;
    using ReplyArguments = std::tuple<WebKit::WebsiteData>;
    explicit FetchWebsiteData(const OptionSet<WebKit::WebsiteDataType>& websiteDataTypes)
        : m_arguments(websiteDataTypes)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DeleteWebsiteData {
public:
    using Arguments = std::tuple<const OptionSet<WebKit::WebsiteDataType>&, const WallTime&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_DeleteWebsiteData; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebProcess_DeleteWebsiteDataReply; }
    using AsyncReply = DeleteWebsiteDataAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    DeleteWebsiteData(const OptionSet<WebKit::WebsiteDataType>& websiteDataTypes, const WallTime& modifiedSince)
        : m_arguments(websiteDataTypes, modifiedSince)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DeleteWebsiteDataForOrigins {
public:
    using Arguments = std::tuple<const OptionSet<WebKit::WebsiteDataType>&, const Vector<WebCore::SecurityOriginData>&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_DeleteWebsiteDataForOrigins; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebProcess_DeleteWebsiteDataForOriginsReply; }
    using AsyncReply = DeleteWebsiteDataForOriginsAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    DeleteWebsiteDataForOrigins(const OptionSet<WebKit::WebsiteDataType>& websiteDataTypes, const Vector<WebCore::SecurityOriginData>& origins)
        : m_arguments(websiteDataTypes, origins)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetHiddenPageDOMTimerThrottlingIncreaseLimit {
public:
    using Arguments = std::tuple<const int&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetHiddenPageDOMTimerThrottlingIncreaseLimit; }
    static const bool isSync = false;

    explicit SetHiddenPageDOMTimerThrottlingIncreaseLimit(const int& milliseconds)
        : m_arguments(milliseconds)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if PLATFORM(COCOA)
class SetQOS {
public:
    using Arguments = std::tuple<const int&, const int&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetQOS; }
    static const bool isSync = false;

    SetQOS(const int& latencyQOS, const int& throughputQOS)
        : m_arguments(latencyQOS, throughputQOS)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

class SetMemoryCacheDisabled {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetMemoryCacheDisabled; }
    static const bool isSync = false;

    explicit SetMemoryCacheDisabled(bool disabled)
        : m_arguments(disabled)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if ENABLE(SERVICE_CONTROLS)
class SetEnabledServices {
public:
    using Arguments = std::tuple<bool, bool, bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetEnabledServices; }
    static const bool isSync = false;

    SetEnabledServices(bool hasImageServices, bool hasSelectionServices, bool hasRichContentServices)
        : m_arguments(hasImageServices, hasSelectionServices, hasRichContentServices)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

class EnsureAutomationSessionProxy {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_EnsureAutomationSessionProxy; }
    static const bool isSync = false;

    explicit EnsureAutomationSessionProxy(const String& sessionIdentifier)
        : m_arguments(sessionIdentifier)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class DestroyAutomationSessionProxy {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_DestroyAutomationSessionProxy; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class PrepareToSuspend {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_PrepareToSuspend; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebProcess_PrepareToSuspendReply; }
    using AsyncReply = PrepareToSuspendAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit PrepareToSuspend(bool isSuspensionImminent)
        : m_arguments(isSuspensionImminent)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ProcessDidResume {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_ProcessDidResume; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class MainThreadPing {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_MainThreadPing; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class BackgroundResponsivenessPing {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_BackgroundResponsivenessPing; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if ENABLE(GAMEPAD)
class SetInitialGamepads {
public:
    using Arguments = std::tuple<const Vector<WebKit::GamepadData>&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetInitialGamepads; }
    static const bool isSync = false;

    explicit SetInitialGamepads(const Vector<WebKit::GamepadData>& gamepadDatas)
        : m_arguments(gamepadDatas)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if ENABLE(GAMEPAD)
class GamepadConnected {
public:
    using Arguments = std::tuple<const WebKit::GamepadData&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_GamepadConnected; }
    static const bool isSync = false;

    explicit GamepadConnected(const WebKit::GamepadData& gamepadData)
        : m_arguments(gamepadData)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if ENABLE(GAMEPAD)
class GamepadDisconnected {
public:
    using Arguments = std::tuple<const unsigned&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_GamepadDisconnected; }
    static const bool isSync = false;

    explicit GamepadDisconnected(const unsigned& index)
        : m_arguments(index)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if ENABLE(SERVICE_WORKER)
class EstablishWorkerContextConnectionToNetworkProcess {
public:
    using Arguments = std::tuple<uint64_t, const WebKit::WebPageProxyIdentifier&, const WebCore::PageIdentifier&, const WebKit::WebPreferencesStore&, const WebCore::RegistrableDomain&, const WebKit::ServiceWorkerInitializationData&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_EstablishWorkerContextConnectionToNetworkProcess; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebProcess_EstablishWorkerContextConnectionToNetworkProcessReply; }
    using AsyncReply = EstablishWorkerContextConnectionToNetworkProcessAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    EstablishWorkerContextConnectionToNetworkProcess(uint64_t pageGroupID, const WebKit::WebPageProxyIdentifier& webPageProxyID, const WebCore::PageIdentifier& pageID, const WebKit::WebPreferencesStore& store, const WebCore::RegistrableDomain& domain, const WebKit::ServiceWorkerInitializationData& initializationData)
        : m_arguments(pageGroupID, webPageProxyID, pageID, store, domain, initializationData)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

class SetHasSuspendedPageProxy {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetHasSuspendedPageProxy; }
    static const bool isSync = false;

    explicit SetHasSuspendedPageProxy(bool hasSuspendedPageProxy)
        : m_arguments(hasSuspendedPageProxy)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetIsInProcessCache {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetIsInProcessCache; }
    static const bool isSync = false;

    explicit SetIsInProcessCache(bool isInProcessCache)
        : m_arguments(isInProcessCache)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class MarkIsNoLongerPrewarmed {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_MarkIsNoLongerPrewarmed; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class GetActivePagesOriginsForTesting {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_GetActivePagesOriginsForTesting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(Vector<String>&&)>&&);
    static void cancelReply(CompletionHandler<void(Vector<String>&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebProcess_GetActivePagesOriginsForTestingReply; }
    using AsyncReply = GetActivePagesOriginsForTestingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, const Vector<String>& activeOrigins);
    using Reply = std::tuple<Vector<String>&>;
    using ReplyArguments = std::tuple<Vector<String>>;
    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if PLATFORM(COCOA)
class SetScreenProperties {
public:
    using Arguments = std::tuple<const WebCore::ScreenProperties&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetScreenProperties; }
    static const bool isSync = false;

    explicit SetScreenProperties(const WebCore::ScreenProperties& screenProperties)
        : m_arguments(screenProperties)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if PLATFORM(MAC) && ENABLE(WEBPROCESS_WINDOWSERVER_BLOCKING)
class ScrollerStylePreferenceChanged {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_ScrollerStylePreferenceChanged; }
    static const bool isSync = false;

    explicit ScrollerStylePreferenceChanged(bool useOvelayScrollbars)
        : m_arguments(useOvelayScrollbars)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if PLATFORM(MAC) && ENABLE(WEBPROCESS_WINDOWSERVER_BLOCKING)
class DisplayConfigurationChanged {
public:
    using Arguments = std::tuple<const CGDirectDisplayID&, const CGDisplayChangeSummaryFlags&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_DisplayConfigurationChanged; }
    static const bool isSync = false;

    DisplayConfigurationChanged(const CGDirectDisplayID& displayID, const CGDisplayChangeSummaryFlags& flags)
        : m_arguments(displayID, flags)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if PLATFORM(IOS_FAMILY) && !PLATFORM(MACCATALYST)
class BacklightLevelDidChange {
public:
    using Arguments = std::tuple<float>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_BacklightLevelDidChange; }
    static const bool isSync = false;

    explicit BacklightLevelDidChange(float backlightLevel)
        : m_arguments(backlightLevel)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

class IsJITEnabled {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_IsJITEnabled; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void(bool&&)>&&);
    static void cancelReply(CompletionHandler<void(bool&&)>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebProcess_IsJITEnabledReply; }
    using AsyncReply = IsJITEnabledAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&, bool enabled);
    using Reply = std::tuple<bool&>;
    using ReplyArguments = std::tuple<bool>;
    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if PLATFORM(COCOA)
class SetMediaMIMETypes {
public:
    using Arguments = std::tuple<const Vector<String>&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetMediaMIMETypes; }
    static const bool isSync = false;

    explicit SetMediaMIMETypes(const Vector<String>& types)
        : m_arguments(types)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if (PLATFORM(COCOA) && ENABLE(REMOTE_INSPECTOR))
class EnableRemoteWebInspector {
public:
    using Arguments = std::tuple<const WebKit::SandboxExtension::Handle&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_EnableRemoteWebInspector; }
    static const bool isSync = false;

    explicit EnableRemoteWebInspector(const WebKit::SandboxExtension::Handle& handle)
        : m_arguments(handle)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if ENABLE(MEDIA_STREAM)
class AddMockMediaDevice {
public:
    using Arguments = std::tuple<const WebCore::MockMediaDevice&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_AddMockMediaDevice; }
    static const bool isSync = false;

    explicit AddMockMediaDevice(const WebCore::MockMediaDevice& device)
        : m_arguments(device)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if ENABLE(MEDIA_STREAM)
class ClearMockMediaDevices {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_ClearMockMediaDevices; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if ENABLE(MEDIA_STREAM)
class RemoveMockMediaDevice {
public:
    using Arguments = std::tuple<const String&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_RemoveMockMediaDevice; }
    static const bool isSync = false;

    explicit RemoveMockMediaDevice(const String& persistentId)
        : m_arguments(persistentId)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if ENABLE(MEDIA_STREAM)
class ResetMockMediaDevices {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_ResetMockMediaDevices; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if (ENABLE(MEDIA_STREAM) && ENABLE(SANDBOX_EXTENSIONS))
class GrantUserMediaDeviceSandboxExtensions {
public:
    using Arguments = std::tuple<const WebKit::MediaDeviceSandboxExtensions&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_GrantUserMediaDeviceSandboxExtensions; }
    static const bool isSync = false;

    explicit GrantUserMediaDeviceSandboxExtensions(const WebKit::MediaDeviceSandboxExtensions& sandboxExtensions)
        : m_arguments(sandboxExtensions)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if (ENABLE(MEDIA_STREAM) && ENABLE(SANDBOX_EXTENSIONS))
class RevokeUserMediaDeviceSandboxExtensions {
public:
    using Arguments = std::tuple<const Vector<String>&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_RevokeUserMediaDeviceSandboxExtensions; }
    static const bool isSync = false;

    explicit RevokeUserMediaDeviceSandboxExtensions(const Vector<String>& sandboxExtensionIDs)
        : m_arguments(sandboxExtensionIDs)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

class ClearCurrentModifierStateForTesting {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_ClearCurrentModifierStateForTesting; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class SetBackForwardCacheCapacity {
public:
    using Arguments = std::tuple<const unsigned&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetBackForwardCacheCapacity; }
    static const bool isSync = false;

    explicit SetBackForwardCacheCapacity(const unsigned& capacity)
        : m_arguments(capacity)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

class ClearCachedPage {
public:
    using Arguments = std::tuple<const WebCore::BackForwardItemIdentifier&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_ClearCachedPage; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebProcess_ClearCachedPageReply; }
    using AsyncReply = ClearCachedPageAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit ClearCachedPage(const WebCore::BackForwardItemIdentifier& backForwardItemID)
        : m_arguments(backForwardItemID)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};

#if PLATFORM(GTK) || PLATFORM(WPE)
class SendMessageToWebExtension {
public:
    using Arguments = std::tuple<const WebKit::UserMessage&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SendMessageToWebExtension; }
    static const bool isSync = false;

    explicit SendMessageToWebExtension(const WebKit::UserMessage& userMessage)
        : m_arguments(userMessage)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
class SeedResourceLoadStatisticsForTesting {
public:
    using Arguments = std::tuple<const WebCore::RegistrableDomain&, const WebCore::RegistrableDomain&, bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SeedResourceLoadStatisticsForTesting; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebProcess_SeedResourceLoadStatisticsForTestingReply; }
    using AsyncReply = SeedResourceLoadStatisticsForTestingAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    SeedResourceLoadStatisticsForTesting(const WebCore::RegistrableDomain& firstPartyDomain, const WebCore::RegistrableDomain& thirdPartyDomain, bool shouldScheduleNotification)
        : m_arguments(firstPartyDomain, thirdPartyDomain, shouldScheduleNotification)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
class SetThirdPartyCookieBlockingMode {
public:
    using Arguments = std::tuple<WebCore::ThirdPartyCookieBlockingMode>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetThirdPartyCookieBlockingMode; }
    static const bool isSync = false;

    static void callReply(IPC::Decoder&, CompletionHandler<void()>&&);
    static void cancelReply(CompletionHandler<void()>&&);
    static IPC::MessageName asyncMessageReplyName() { return IPC::MessageName::WebProcess_SetThirdPartyCookieBlockingModeReply; }
    using AsyncReply = SetThirdPartyCookieBlockingModeAsyncReply;
    static void send(std::unique_ptr<IPC::Encoder>&&, IPC::Connection&);
    using Reply = std::tuple<>;
    using ReplyArguments = std::tuple<>;
    explicit SetThirdPartyCookieBlockingMode(WebCore::ThirdPartyCookieBlockingMode blockingMode)
        : m_arguments(blockingMode)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if ENABLE(RESOURCE_LOAD_STATISTICS)
class SetDomainsWithUserInteraction {
public:
    using Arguments = std::tuple<const HashSet<WebCore::RegistrableDomain>&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetDomainsWithUserInteraction; }
    static const bool isSync = false;

    explicit SetDomainsWithUserInteraction(const HashSet<WebCore::RegistrableDomain>& domains)
        : m_arguments(domains)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if PLATFORM(IOS)
class GrantAccessToAssetServices {
public:
    using Arguments = std::tuple<const WebKit::SandboxExtension::Handle&, const WebKit::SandboxExtension::Handle&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_GrantAccessToAssetServices; }
    static const bool isSync = false;

    GrantAccessToAssetServices(const WebKit::SandboxExtension::Handle& mobileAssetHandle, const WebKit::SandboxExtension::Handle& mobileAssetV2Handle)
        : m_arguments(mobileAssetHandle, mobileAssetV2Handle)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if PLATFORM(IOS)
class RevokeAccessToAssetServices {
public:
    using Arguments = std::tuple<>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_RevokeAccessToAssetServices; }
    static const bool isSync = false;

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if PLATFORM(COCOA)
class UnblockServicesRequiredByAccessibility {
public:
    using Arguments = std::tuple<const WebKit::SandboxExtension::HandleArray&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_UnblockServicesRequiredByAccessibility; }
    static const bool isSync = false;

    explicit UnblockServicesRequiredByAccessibility(const WebKit::SandboxExtension::HandleArray& handleArray)
        : m_arguments(handleArray)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if (PLATFORM(COCOA) && ENABLE(CFPREFS_DIRECT_MODE))
class NotifyPreferencesChanged {
public:
    using Arguments = std::tuple<const String&, const String&, const Optional<String>&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_NotifyPreferencesChanged; }
    static const bool isSync = false;

    NotifyPreferencesChanged(const String& domain, const String& key, const Optional<String>& encodedValue)
        : m_arguments(domain, key, encodedValue)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if (PLATFORM(COCOA) && ENABLE(CFPREFS_DIRECT_MODE))
class UnblockPreferenceService {
public:
    using Arguments = std::tuple<const WebKit::SandboxExtension::HandleArray&>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_UnblockPreferenceService; }
    static const bool isSync = false;

    explicit UnblockPreferenceService(const WebKit::SandboxExtension::HandleArray& handleArray)
        : m_arguments(handleArray)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

#if PLATFORM(GTK) && !USE(GTK4)
class SetUseSystemAppearanceForScrollbars {
public:
    using Arguments = std::tuple<bool>;

    static IPC::MessageName name() { return IPC::MessageName::WebProcess_SetUseSystemAppearanceForScrollbars; }
    static const bool isSync = false;

    explicit SetUseSystemAppearanceForScrollbars(bool useSystemAppearanceForScrollbars)
        : m_arguments(useSystemAppearanceForScrollbars)
    {
    }

    const Arguments& arguments() const
    {
        return m_arguments;
    }

private:
    Arguments m_arguments;
};
#endif

} // namespace WebProcess
} // namespace Messages
