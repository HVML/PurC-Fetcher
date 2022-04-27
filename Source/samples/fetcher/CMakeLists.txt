include(GlobalCommon)
include(target/PurCFetcher)

# fetcher
PURCFETCHER_EXECUTABLE_DECLARE(purc_fetcher)

list(APPEND purc_fetcher_PRIVATE_INCLUDE_DIRECTORIES
    ${PURCFETCHER_DIR}/include
    ${PurCFetcher_DERIVED_SOURCES_DIR}
)

PURCFETCHER_EXECUTABLE(purc_fetcher)

set(purc_fetcher_SOURCES
    fetcher.cpp
)

set(purc_fetcher_LIBRARIES
    PurCFetcher::PurCFetcher
    pthread
)

#PURCFETCHER_COMPUTE_SOURCES(purc_fetcher)
PURCFETCHER_FRAMEWORK(purc_fetcher)

install(TARGETS purc_fetcher DESTINATION "${LIBEXEC_INSTALL_DIR}")
