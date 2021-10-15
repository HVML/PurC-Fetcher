set(PurCFetcher_OUTPUT_NAME purc_fetcher)

list(APPEND PurCFetcher_PRIVATE_INCLUDE_DIRECTORIES
    "${PURCFETCHER_DIR}/IPC/glib"
    "${PURCFETCHER_DIR}/IPC/unix"
    "${PURCFETCHER_DIR}/Shared/soup"
)

list(APPEND PurCFetcher_UNIFIED_SOURCE_LIST_FILES
    "SourcesLinux.txt"
)

list(APPEND PurCFetcher_SOURCES
)

list(APPEND PurCFetcher_LIBRARIES
    -lpthread
)

list(APPEND PurCFetcher_SYSTEM_INCLUDE_DIRECTORIES
    ${LIBSOUP_INCLUDE_DIRS}
)

list(APPEND PurCFetcher_LIBRARIES
    ${GLIB_LIBRARIES}
)

if (ENABLE_SOCKET_STREAM)
    list(APPEND PurCFetcher_SYSTEM_INCLUDE_DIRECTORIES
        ${GLIB_INCLUDE_DIRS}
    )

    list(APPEND PurCFetcher_LIBRARIES
        ${GLIB_GIO_LIBRARIES}
        ${GLIB_LIBRARIES}
    )
endif ()

configure_file(ports/linux/purc_fetcher.pc.in ${PurCFetcher_PKGCONFIG_FILE} @ONLY)
install(FILES "${PurCFetcher_PKGCONFIG_FILE}"
        DESTINATION "${LIB_INSTALL_DIR}/pkgconfig"
)

