set(PurCFetcher_OUTPUT_NAME purc_fetcher)

list(APPEND PurCFetcher_PRIVATE_INCLUDE_DIRECTORIES
)

list(APPEND PurCFetcher_UNIFIED_SOURCE_LIST_FILES
)

list(APPEND PurCFetcher_SOURCES
)

list(APPEND PurCFetcher_LIBRARIES
    -lpthread
)

if (ENABLE_SOCKET_STREAM)
    list(APPEND PurCFetcher_SYSTEM_INCLUDE_DIRECTORIES
    )

    list(APPEND PurCFetcher_LIBRARIES
    )
endif ()

configure_file(ports/linux/purc_fetcher.pc.in ${PurCFetcher_PKGCONFIG_FILE} @ONLY)
install(FILES "${PurCFetcher_PKGCONFIG_FILE}"
        DESTINATION "${LIB_INSTALL_DIR}/pkgconfig"
)

