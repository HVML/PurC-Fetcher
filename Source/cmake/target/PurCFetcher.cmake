if (NOT TARGET PurCFetcher::PurCFetcher)
    if (NOT INTERNAL_BUILD)
        message(FATAL_ERROR "PurCFetcher::PurCFetcher target not found")
    endif ()

    # This should be moved to an if block if the Apple Mac/iOS build moves completely to CMake
    # Just assuming Windows for the moment
    add_library(PurCFetcher::PurCFetcher STATIC IMPORTED)
    set_target_properties(PurCFetcher::PurCFetcher PROPERTIES
        IMPORTED_LOCATION ${WEBKIT_LIBRARIES_LINK_DIR}/PurCFetcher${DEBUG_SUFFIX}.lib
    )
    set(PurCFetcher_PRIVATE_FRAMEWORK_HEADERS_DIR "${CMAKE_BINARY_DIR}/../include/private")
    target_include_directories(PurCFetcher::PurCFetcher INTERFACE
        ${PurCFetcher_PRIVATE_FRAMEWORK_HEADERS_DIR}
    )
endif ()
