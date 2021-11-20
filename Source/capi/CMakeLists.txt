set_property(DIRECTORY . PROPERTY FOLDER "fetcher_capi")
include(MyMacros.cmake)

set(fetcher_capi_PRIVATE_INCLUDE_DIRECTORIES
    "${CMAKE_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/capi/ipc"
    "${CMAKE_CURRENT_SOURCE_DIR}/capi/ipc/unix"
    "${CMAKE_CURRENT_SOURCE_DIR}/capi/launcher"
    "${BMALLOC_DIR}"
    "${PURC_INCLUDE_DIRS}"
    "${PURCFETCHER_DIR}"
    "${PURCFETCHER_DIR}/include"
    "${PURCFETCHER_DIR}/auxiliary"
    "${PURCFETCHER_DIR}/auxiliary/soup"
    "${PURCFETCHER_DIR}/network"
    "${PURCFETCHER_DIR}/network/soup"
    "${PURCFETCHER_DIR}/network/WebStorage"
    "${PURCFETCHER_DIR}/network/Downloads"
    "${PURCFETCHER_DIR}/network/soup"
    "${FORWARDING_HEADERS_DIR}"
    "${FORWARDING_HEADERS_DIR}/capi"
    "${PurCFetcher_DERIVED_SOURCES_DIR}"
    "${PurCFetcher_DERIVED_SOURCES_DIR}"
    "${MESSAGES_DERIVED_SOURCES_DIR}"
    "${GIO_UNIX_INCLUDE_DIRS}"
    "${GLIB_INCLUDE_DIRS}"
    "${PURC_INCLUDE_DIRS}"
    "${LIBSOUP_INCLUDE_DIRS}"
)

set(fetcher_capi_SOURCES
    capi/ipc/ArgumentCoders.cpp
    capi/ipc/Attachment.cpp
    capi/ipc/Connection.cpp
    capi/ipc/DataReference.cpp
    capi/ipc/Decoder.cpp
    capi/ipc/Encoder.cpp
    capi/ipc/MessageReceiverMap.cpp
    capi/ipc/MessageSender.cpp
    capi/ipc/StringReference.cpp
    capi/ipc/unix/AttachmentUnix.cpp
    capi/ipc/unix/ConnectionUnix.cpp

    capi/launcher/ProcessLauncher.cpp
    capi/launcher/ProcessLauncherGLib.cpp
    capi/launcher/ProcessExecutablePathGLib.cpp

    capi/fetcher.cpp
    capi/fetcher-local.cpp
    capi/fetcher-remote.cpp
    capi/fetcher-process.cpp
    capi/fetcher-session.cpp
    capi/fetcher-msg.c
    capi/fetcher-conn.c
)

set(fetcher_capi_PUBLIC_HEADERS
    capi/fetcher.h
    capi/fetcher-msg.h
    capi/fetcher-conn.h
    capi/fetcher-http-header-name.h
    capi/fetcher-network-load-priority.h
    capi/fetcher-resource-response-source.h
    capi/fetcher-resource-response-type.h
    capi/fetcher-resource-response-tainting.h
)

set(fetcher_capi_LIBRARIES
    ${CMAKE_DL_LIBS}
    PurCFetcher::PurCFetcher
    ${PURC_LIBRARIES}
)

set(fetcher_capi_MSG_IN_FILES
    msg/uint8
    msg/second

    msg/common-header
    msg/uncommon-header
    msg/http-header-map
    msg/website-data-store-parameters
    msg/network-load-metrics
    msg/frame-identifier
    msg/page-identifier
    msg/web-page-proxy-identifier
    msg/process-identifier
    msg/session-id
    msg/certificate-info

    msg/soup-network-proxy-settings
    msg/msg-initialize-network-process
    msg/msg-set-on-line-state
    msg/msg-register-url-schemes-as-cors-enabled
    msg/msg-continue-did-receive-response
    msg/msg-remove-load-identifier
    msg/msg-page-load-completed
    msg/msg-did-finish-resource-load
    msg/msg-browsing-context-removed
    msg/msg-create-network-connection-to-web-process
    msg/msg-create-network-connection-to-web-process-reply
    msg/msg-did-receive-response
)
GENERATE_MSG_SOURCES(fetcher_capi_SOURCES "${fetcher_capi_MSG_IN_FILES}")

set(fetcher_capi_INTERFACE_LIBRARIES fetcher_capi)
set(fetcher_capi_INTERFACE_INCLUDE_DIRECTORIES ${fetcher_capi_FRAMEWORK_HEADERS_DIR})
set(fetcher_capi_INTERFACE_DEPENDENCIES fetcher_capi_CopyHeaders)

PURCFETCHER_FRAMEWORK_DECLARE(fetcher_capi)
PURCFETCHER_INCLUDE_CONFIG_FILES_IF_EXISTS()

PURCFETCHER_COPY_FILES(fetcher_capi_CopyHeaders
    DESTINATION ${fetcher_capi_FRAMEWORK_HEADERS_DIR}
    FILES ${fetcher_capi_PUBLIC_HEADERS}
)

PURCFETCHER_WRAP_SOURCELIST(${fetcher_capi_SOURCES})
PURCFETCHER_FRAMEWORK(fetcher_capi)
PURCFETCHER_FRAMEWORK_TARGET(fetcher_capi)

