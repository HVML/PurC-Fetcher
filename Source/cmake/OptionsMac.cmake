# FIXME: These should line up with versions in Version.xcconfig
set(PURCFETCHER_MAC_VERSION 0.0.1)
set(MACOSX_FRAMEWORK_BUNDLE_VERSION 0.0.1)

find_package(GLIB 2.44.0 COMPONENTS gio gio-unix gobject)
find_package(LibXml2 2.8.0)
find_package(LibXslt 1.1.7)
find_package(LibSoup 2.54.0)
find_package(CURL 7.60.0)
find_package(OpenSSL 1.1.1)
find_package(SQLite3 3.10.0)
find_package(LibGcrypt 1.6.0 REQUIRED)
find_package(LibGpgError REQUIRED)

PURCFETCHER_OPTION_BEGIN()
# Private options shared with other PurCFetcher ports. Add options here only if
# we need a value different from the default defined in GlobalFeatures.cmake.

PURCFETCHER_OPTION_DEFAULT_PORT_VALUE(ENABLE_XML PUBLIC OFF)
PURCFETCHER_OPTION_DEFAULT_PORT_VALUE(ENABLE_HTTP PUBLIC OFF)
PURCFETCHER_OPTION_DEFAULT_PORT_VALUE(ENABLE_LSQL PUBLIC OFF)
PURCFETCHER_OPTION_DEFAULT_PORT_VALUE(ENABLE_RSQL PUBLIC OFF)
PURCFETCHER_OPTION_DEFAULT_PORT_VALUE(ENABLE_HIBUS PUBLIC OFF)
PURCFETCHER_OPTION_DEFAULT_PORT_VALUE(ENABLE_SSL PUBLIC OFF)


PURCFETCHER_OPTION_END()

set(PurCFetcher_PKGCONFIG_FILE ${CMAKE_BINARY_DIR}/src/purc_fetcher/purc_fetcher.pc)

set(PurCFetcher_LIBRARY_TYPE SHARED)
set(PurCFetcherTestSupport_LIBRARY_TYPE SHARED)

