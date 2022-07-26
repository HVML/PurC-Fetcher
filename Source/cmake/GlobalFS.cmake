if (NOT WTF_DIR)
    set(WTF_DIR "${CMAKE_SOURCE_DIR}/Source/WTF")
endif ()
if (NOT PURCFETCHER_DIR)
    set(PURCFETCHER_DIR "${CMAKE_SOURCE_DIR}/Source/PurCFetcher")
endif ()
if (NOT THIRDPARTY_DIR)
    set(THIRDPARTY_DIR "${CMAKE_SOURCE_DIR}/Source/third-party")
endif ()
if (NOT TOOLS_DIR)
    set(TOOLS_DIR "${CMAKE_SOURCE_DIR}/tools")
endif ()
if (NOT CAPI_DIR)
    set(CAPI_DIR "${CMAKE_SOURCE_DIR}/Source/capi")
endif ()

set(DERIVED_SOURCES_DIR "${CMAKE_BINARY_DIR}/DerivedSources")
set(WTF_DERIVED_SOURCES_DIR "${CMAKE_BINARY_DIR}/DerivedSources/WTF")
set(MESSAGES_DERIVED_SOURCES_DIR "${CMAKE_BINARY_DIR}/DerivedSources/messages")
set(PurCFetcher_DERIVED_SOURCES_DIR "${CMAKE_BINARY_DIR}/DerivedSources/PurCFetcher")
set(PurCFetcherTestRunner_DERIVED_SOURCES_DIR "${CMAKE_BINARY_DIR}/DerivedSources/PurCFetcherTestRunner")

set(FORWARDING_HEADERS_DIR ${DERIVED_SOURCES_DIR}/ForwardingHeaders)

set(bmalloc_FRAMEWORK_HEADERS_DIR ${FORWARDING_HEADERS_DIR})
set(fetcher_capi_FRAMEWORK_HEADERS_DIR ${FORWARDING_HEADERS_DIR})
set(WTF_FRAMEWORK_HEADERS_DIR ${FORWARDING_HEADERS_DIR})
set(PurCFetcher_FRAMEWORK_HEADERS_DIR ${FORWARDING_HEADERS_DIR})
set(PurCFetcher_PRIVATE_FRAMEWORK_HEADERS_DIR ${FORWARDING_HEADERS_DIR})

set(WTF_SCRIPTS_DIR "${WTF_DIR}/Scripts")
