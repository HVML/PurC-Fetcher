set_property(DIRECTORY . PROPERTY FOLDER "PurCFetcher")

add_definitions(-Wno-conversion)

include(GlobalCommon)
include(MyMacros.cmake)

configure_file(include/purc_fetcher_version.h.in ${PurCFetcher_DERIVED_SOURCES_DIR}/purc_fetcher_version.h)
configure_file(include/purc_fetcher_features.h.in ${PurCFetcher_DERIVED_SOURCES_DIR}/purc_fetcher_features.h)

set(PurCFetcher_PRIVATE_INCLUDE_DIRECTORIES
    "${CMAKE_BINARY_DIR}"
    "${PURCFETCHER_DIR}"
    "${PURCFETCHER_DIR}/include"
    "${PURCFETCHER_DIR}/ipc"
    "${PURCFETCHER_DIR}/auxiliary"
    "${PURCFETCHER_DIR}/auxiliary/crypto"
    "${PURCFETCHER_DIR}/network"
    "${PURCFETCHER_DIR}/network/Downloads"
    "${PURCFETCHER_DIR}/network/WebStorage"
    "${PURCFETCHER_DIR}/network/cache"
    "${PURCFETCHER_DIR}/network/Cookies"
    "${PURCFETCHER_DIR}/messages"
    "${PurCFetcher_DERIVED_SOURCES_DIR}"
    "${FORWARDING_HEADERS_DIR}"
    "${CMAKE_BINARY_DIR}/src/purc_fetcher"
)

# TODO: List the headers should be installed to system here.
set(PurCFetcher_INSTALLED_HEADERS
    "${PurCFetcher_DERIVED_SOURCES_DIR}/purc_fetcher_version.h"
    "${PurCFetcher_DERIVED_SOURCES_DIR}/purc_fetcher_features.h"
    "${PURCFETCHER_DIR}/include/purc_fetcher_macros.h"
    "${PURCFETCHER_DIR}/include/purc_fetcher_errors.h"
    "${PURCFETCHER_DIR}/include/purc_fetcher_utils.h"
    "${PURCFETCHER_DIR}/include/purc_fetcher_ports.h"
    "${PURCFETCHER_DIR}/include/purc_fetcher.h"
)

# TODO: List the directorires which do not contain any platform-dependent C source files
set(PurCFetcher_PLATFORM_INDEPENDENT_DIRS
    "instance"
    "utils"
)

# This macro will append all C/CXX source files in ${PurCFetcher_PLATFORM_INDEPENDENT_DIRS}
# to ${PurCFetcher_SOURCES}
APPEND_ALL_SOURCE_FILES_IN_DIRLIST(PurCFetcher_SOURCES "${PurCFetcher_PLATFORM_INDEPENDENT_DIRS}")

# TODO: List the source files individually.
list(APPEND PurCFetcher_SOURCES
    "${PURCFETCHER_DIR}/ports/vasprintf.c"
)

# Put the CXX source files which can be unified in `Sources.txt`
list(APPEND PurCFetcher_UNIFIED_SOURCE_LIST_FILES
    "Sources.txt"
)

set(PurCFetcher_LIBRARIES)

# TODO: the source files and libraries for optional features.
if (ENABLE_LSQL)
    list(APPEND PurCFetcher_LIBRARIES SQLite::SQLite3)
endif ()

if (ENABLE_XML)
    list(APPEND PurCFetcher_LIBRARIES LibXml2::LibXml2)
endif ()

list(APPEND PurCFetcher_LIBRARIES PurCFetcher::WTF)

set(PurCFetcher_INTERFACE_LIBRARIES PurCFetcher)
set(PurCFetcher_INTERFACE_INCLUDE_DIRECTORIES ${PurCFetcher_PRIVATE_FRAMEWORK_HEADERS_DIR})

PURCFETCHER_FRAMEWORK_DECLARE(PurCFetcher)
PURCFETCHER_INCLUDE_CONFIG_FILES_IF_EXISTS()

PURCFETCHER_WRAP_SOURCELIST(${PurCFetcher_SOURCES})

PURCFETCHER_COPY_FILES(PurCFetcher_CopyPrivateHeaders
    DESTINATION ${PurCFetcher_PRIVATE_FRAMEWORK_HEADERS_DIR}/purc_fetcher
    FILES ${PurCFetcher_PRIVATE_FRAMEWORK_HEADERS}
    FLATTENED
)
list(APPEND PurCFetcher_INTERFACE_DEPENDENCIES PurCFetcher_CopyPrivateHeaders)
# PurCFetcher_CopyPrivateHeaders needs to have a direct or indirect
# dependency of PurCFetcher target for CMake Visual Studio generator to
# eliminate duplicated custom commands. Otherwise, CSSPropertyNames.h
# will be generated in both projects.
if (NOT INTERNAL_BUILD)
    add_dependencies(PurCFetcher_CopyPrivateHeaders PurCFetcher)
endif ()

PURCFETCHER_COMPUTE_SOURCES(PurCFetcher)
PURCFETCHER_FRAMEWORK(PurCFetcher)
PURCFETCHER_FRAMEWORK_TARGET(PurCFetcher)

# The -ftree-sra optimization (implicit with -O2) causes crashes when
# allocating large chunks of memory using bmalloc on Intel 32bit.
# See https://bugs.webkit.org/show_bug.cgi?id=146440
string(TOLOWER ${CMAKE_HOST_SYSTEM_PROCESSOR} LOWERCASE_CMAKE_HOST_SYSTEM_PROCESSOR)
if (CMAKE_COMPILER_IS_GNUCXX AND "${LOWERCASE_CMAKE_HOST_SYSTEM_PROCESSOR}" MATCHES "(i[3-6]86|x86)$")
    PURCFETCHER_ADD_TARGET_CXX_FLAGS(PurCFetcher -fno-tree-sra)
endif ()

if (${PurCFetcher_LIBRARY_TYPE} MATCHES "SHARED")
    set_target_properties(PurCFetcher PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
    install(TARGETS PurCFetcher DESTINATION "${LIB_INSTALL_DIR}")
endif ()

if (MSVC)
    set_target_properties(PurCFetcher PROPERTIES
        COMPILE_PDB_NAME ${PurCFetcher_OUTPUT_NAME}
    )
endif ()

# add_subdirectory(res)

install(TARGETS PurCFetcher
        DESTINATION "${LIB_INSTALL_DIR}/"
)

install(FILES ${PurCFetcher_INSTALLED_HEADERS}
        DESTINATION "${HEADER_INSTALL_DIR}/purc_fetcher"
)
