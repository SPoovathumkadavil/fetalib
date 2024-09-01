# ---- Declare library ----

file(GLOB_RECURSE fetalib_fetalib_sources CONFIGURE_DEPENDS "src/fetalib/*.cpp")
add_library(
    fetalib_fetalib
    ${fetalib_fetalib_sources}
)
add_library(fetalib::fetalib ALIAS fetalib_fetalib)

include(GenerateExportHeader)
generate_export_header(
    fetalib_fetalib
    BASE_NAME fetalib
    EXPORT_FILE_NAME export/fetalib/fetalib_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(fetalib_fetalib PUBLIC FETALIB_STATIC_DEFINE)
endif()

set_target_properties(
    fetalib_fetalib PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME fetalib
    OUTPUT_NAME fetalib
)

target_include_directories(
    fetalib_fetalib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    fetalib_fetalib SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(fetalib_fetalib PUBLIC cxx_std_17)

# ---- external libs ----

include(cmake/external-lib.cmake)
