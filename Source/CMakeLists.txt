# -----------------------------------------------------------------------------
# Add components directories
# -----------------------------------------------------------------------------
if (NOT USE_SYSTEM_MALLOC)
    add_subdirectory(bmalloc)
endif ()

add_subdirectory(WTF)

add_subdirectory(PurCFetcher)

add_subdirectory(capi)

if (ENABLE_API_TESTS)
    add_subdirectory(third-party/gtest)
#    add_subdirectory(test)
endif ()

add_subdirectory(third-party/xdgmime)
add_subdirectory(samples)

PURCFETCHER_INCLUDE_CONFIG_FILES_IF_EXISTS()
