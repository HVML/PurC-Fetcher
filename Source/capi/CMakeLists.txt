set_property(DIRECTORY . PROPERTY FOLDER "fetcher_capi")

set(fetcher_capi_PRIVATE_INCLUDE_DIRECTORIES
    "${CMAKE_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/capi/ipc"
    "${CMAKE_CURRENT_SOURCE_DIR}/capi/ipc/unix"
    "${CMAKE_CURRENT_SOURCE_DIR}/capi/launcher"
    "${CMAKE_CURRENT_SOURCE_DIR}/capi/messages"
    "${CMAKE_CURRENT_SOURCE_DIR}/capi/messages/soup"
    "${BMALLOC_DIR}"
    "${PURC_INCLUDE_DIRS}"
    "${PURCFETCHER_DIR}"
    "${FORWARDING_HEADERS_DIR}"
    "${FORWARDING_HEADERS_DIR}/capi"
    "${PURC_INCLUDE_DIRS}"
    "${GIO_UNIX_INCLUDE_DIRS}"
    "${GLIB_INCLUDE_DIRS}"
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
    capi/ipc/SharedBuffer.cpp
    capi/ipc/SharedMemory.cpp
    capi/ipc/unix/AttachmentUnix.cpp
    capi/ipc/unix/ConnectionUnix.cpp
    capi/ipc/unix/SharedMemoryUnix.cpp
    capi/ipc/soup/SharedBufferSoup.cpp
    capi/ipc/soup/SharedBufferGlib.cpp

    capi/launcher/ProcessLauncher.cpp

    capi/messages/MessageNames.cpp
    capi/messages/HTTPHeaderMap.cpp
    capi/messages/HTTPHeaderNames.cpp
    capi/messages/FormData.cpp
    capi/messages/NetworkActivityTracker.cpp
    capi/messages/WebsiteDataStoreParameters.cpp
    capi/messages/NetworkSessionCreationParameters.cpp
    capi/messages/NetworkResourceLoadParameters.cpp
    capi/messages/NetworkProcessCreationParameters.cpp
    capi/messages/ResourceRequestBase.cpp
    capi/messages/ResourceResponseBase.cpp
    capi/messages/ResourceErrorBase.cpp
    capi/messages/SecurityOrigin.cpp
    capi/messages/WebCoreArgumentCoders.cpp
    capi/messages/soup/CertificateInfoSoup.cpp
    capi/messages/soup/WebCoreArgumentCodersSoup.cpp
    capi/messages/soup/ResourceErrorSoup.cpp
    capi/messages/soup/URLSoup.cpp

    capi/fetcher.cpp
    capi/fetcher-local.cpp
    capi/fetcher-remote.cpp
    capi/fetcher-process.cpp
    capi/fetcher-session.cpp
)

set(fetcher_capi_PUBLIC_HEADERS
    capi/fetcher.h
)

set(fetcher_capi_LIBRARIES
    ${CMAKE_DL_LIBS}
    PurCFetcher::WTF
    ${GLIB_GIO_LIBRARIES}
    ${GLIB_GOBJECT_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${LIBSOUP_LIBRARIES}
    ${PURC_LIBRARIES}
    rt
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

