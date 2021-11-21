include(GlobalCommon)
include(target/PurCFetcher)

# sync_req
PURCFETCHER_EXECUTABLE_DECLARE(sync_req)

list(APPEND sync_req_PRIVATE_INCLUDE_DIRECTORIES
    "${CMAKE_BINARY_DIR}"
    "${PURCFETCHER_DIR}"
    "${PURCFETCHER_DIR}/include"
    "${PURCFETCHER_DIR}/ipc"
    "${PURCFETCHER_DIR}/auxiliary"
    "${PURCFETCHER_DIR}/auxiliary/soup"
    "${PURCFETCHER_DIR}/network"
    "${PURCFETCHER_DIR}/network/soup"
    "${PURCFETCHER_DIR}/network/WebStorage"
    "${PurCFetcher_DERIVED_SOURCES_DIR}"
    "${MESSAGES_DERIVED_SOURCES_DIR}"
    "${GIO_UNIX_INCLUDE_DIRS}"
    "${GLIB_INCLUDE_DIRS}"
    "${PURC_INCLUDE_DIRS}"
)

PURCFETCHER_EXECUTABLE(sync_req)

set(sync_req_SOURCES
    sync_req.cpp
)

set(sync_req_LIBRARIES
    PurCFetcher::fetcher_capi
    ${PURC_LIBRARIES}
    -lpthread
)

#PURCFETCHER_COMPUTE_SOURCES(sync_req)
PURCFETCHER_FRAMEWORK(sync_req)

# async_req
PURCFETCHER_EXECUTABLE_DECLARE(async_req)

list(APPEND async_req_PRIVATE_INCLUDE_DIRECTORIES
    "${CMAKE_BINARY_DIR}"
    "${PURCFETCHER_DIR}"
    "${PURCFETCHER_DIR}/include"
    "${PURCFETCHER_DIR}/ipc"
    "${PURCFETCHER_DIR}/auxiliary"
    "${PURCFETCHER_DIR}/auxiliary/soup"
    "${PURCFETCHER_DIR}/network"
    "${PURCFETCHER_DIR}/network/soup"
    "${PURCFETCHER_DIR}/network/WebStorage"
    "${PurCFetcher_DERIVED_SOURCES_DIR}"
    "${MESSAGES_DERIVED_SOURCES_DIR}"
    "${GIO_UNIX_INCLUDE_DIRS}"
    "${GLIB_INCLUDE_DIRS}"
    "${PURC_INCLUDE_DIRS}"
)

PURCFETCHER_EXECUTABLE(async_req)

set(async_req_SOURCES
    async_req.cpp
)

set(async_req_LIBRARIES
    PurCFetcher::fetcher_capi
    ${PURC_LIBRARIES}
    -lpthread
)

#PURCFETCHER_COMPUTE_SOURCES(async_req)
PURCFETCHER_FRAMEWORK(async_req)

