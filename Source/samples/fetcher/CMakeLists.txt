include(GlobalCommon)
include(target/PurCFetcher)

# fetcher
PURCFETCHER_EXECUTABLE_DECLARE(fetcher)

list(APPEND fetcher_PRIVATE_INCLUDE_DIRECTORIES
    ${PURCFETCHER_DIR}/include
    ${PurCFetcher_DERIVED_SOURCES_DIR}
)

PURCFETCHER_EXECUTABLE(fetcher)

set(fetcher_SOURCES
    fetcher.cpp
)

set(fetcher_LIBRARIES
    PurCFetcher::PurCFetcher
    pthread
)

#PURCFETCHER_COMPUTE_SOURCES(fetcher)
PURCFETCHER_FRAMEWORK(fetcher)

