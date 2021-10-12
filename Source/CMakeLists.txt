# -----------------------------------------------------------------------------
# Add components directories
# -----------------------------------------------------------------------------
add_subdirectory(WTF)

add_subdirectory(PurCFetcher)

if (ENABLE_API_TESTS)
    add_subdirectory(third-party/gtest)
#    add_subdirectory(test)
endif ()

#add_subdirectory(samples)

PURCFETCHER_INCLUDE_CONFIG_FILES_IF_EXISTS()
