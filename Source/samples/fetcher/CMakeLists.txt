include(GlobalCommon)
include(target/PurCFetcher)

# fetcher
PURCFETCHER_EXECUTABLE_DECLARE(purcfetcher)

list(APPEND purcfetcher_PRIVATE_INCLUDE_DIRECTORIES
    ${PURCFETCHER_DIR}/include
    ${PurCFetcher_DERIVED_SOURCES_DIR}
)

PURCFETCHER_EXECUTABLE(purcfetcher)

set(purcfetcher_SOURCES
    fetcher.cpp
)

set(purcfetcher_LIBRARIES
    PurCFetcher::PurCFetcher
    pthread
)

#PURCFETCHER_COMPUTE_SOURCES(purcfetcher)
PURCFETCHER_FRAMEWORK(purcfetcher)

install(TARGETS purcfetcher DESTINATION "${LIBEXEC_INSTALL_DIR}")
