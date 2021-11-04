include(GlobalCommon)
include(target/PurCFetcher)

# control
PURCFETCHER_EXECUTABLE_DECLARE(control)

list(APPEND control_PRIVATE_INCLUDE_DIRECTORIES
    "${PURCFETCHER_DIR}/include"
    "${PURCFETCHER_DIR}/ipc"
    "${PurCFetcher_DERIVED_SOURCES_DIR}"
    "${MESSAGES_DERIVED_SOURCES_DIR}"
    "${GIO_UNIX_INCLUDE_DIRS}"
    "${GLIB_INCLUDE_DIRS}"
)

PURCFETCHER_EXECUTABLE(control)

set(control_SOURCES
    control.cpp
)

set(control_LIBRARIES
    PurCFetcher::PurCFetcher
    pthread
)

#PURCFETCHER_COMPUTE_SOURCES(control)
PURCFETCHER_FRAMEWORK(control)
