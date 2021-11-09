set_property(DIRECTORY . PROPERTY FOLDER "fetcher_capi")
include(MyMacros.cmake)

set(fetcher_capi_PRIVATE_INCLUDE_DIRECTORIES
    "${BMALLOC_DIR}"
    "${PURC_INCLUDE_DIRS}"
    "${CMAKE_SOURCE_DIR}/Source/capi/capi"
)

set(fetcher_capi_SOURCES
    capi/fetcher-msg.c
)

set(fetcher_capi_PUBLIC_HEADERS
    capi/fetcher-msg.h
)

set(fetcher_capi_LIBRARIES
    ${CMAKE_DL_LIBS}
)

set(fetcher_capi_MSG_IN_FILES
    msg/msg-init
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

