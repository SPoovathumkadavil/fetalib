# ---- Declare library ----

file(GLOB_RECURSE fetalib_common_sources CONFIGURE_DEPENDS "src/fetalib/common/*.cpp")
add_library(
    fetalib_common
    ${fetalib_common_sources}
)
add_library(fetalib::common ALIAS fetalib_common)

file(GLOB_RECURSE fetalib_files_sources CONFIGURE_DEPENDS "src/fetalib/files/*.cpp")
add_library(
    fetalib_files
    ${fetalib_files_sources}
)
add_library(fetalib::files ALIAS fetalib_files)

file(GLOB_RECURSE fetalib_cli_sources CONFIGURE_DEPENDS "src/fetalib/cli/*.cpp")
add_library(
    fetalib_cli
    ${fetalib_cli_sources}
)
add_library(fetalib::cli ALIAS fetalib_cli)

include(GenerateExportHeader)
generate_export_header(
    fetalib_common
    BASE_NAME fetalib
    EXPORT_FILE_NAME export/fetalib/common/fetalib_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)
generate_export_header(
    fetalib_files
    BASE_NAME fetalib
    EXPORT_FILE_NAME export/fetalib/files/fetalib_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)
generate_export_header(
    fetalib_cli
    BASE_NAME fetalib
    EXPORT_FILE_NAME export/fetalib/cli/fetalib_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(fetalib_common PUBLIC FETALIB_STATIC_DEFINE)
  target_compile_definitions(fetalib_files PUBLIC FETALIB_STATIC_DEFINE)
  target_compile_definitions(fetalib_cli PUBLIC FETALIB_STATIC_DEFINE)
endif()

set_target_properties(
    fetalib_common PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME common
    OUTPUT_NAME common
)
set_target_properties(
    fetalib_files PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME files
    OUTPUT_NAME files
)
set_target_properties(
    fetalib_cli PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME cli
    OUTPUT_NAME cli
)

include(GNUInstallDirs)
set(fetalib_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/cmake/fetalib"
    CACHE STRING "Installation destination for CMake files")

target_link_libraries(fetalib_files
    fetalib_common
)
target_link_libraries(fetalib_cli
    fetalib_common
)

target_include_directories(
    fetalib_common ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)
target_include_directories(
    fetalib_files ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)
target_include_directories(
    fetalib_cli ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    fetalib_common SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)
target_include_directories(
    fetalib_files SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)
target_include_directories(
    fetalib_cli SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(fetalib_common PUBLIC cxx_std_17)
target_compile_features(fetalib_files PUBLIC cxx_std_17)
target_compile_features(fetalib_cli PUBLIC cxx_std_17)

# ---- external libs ----

include(cmake/external-lib.cmake)
