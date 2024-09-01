if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/fetalib-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package fetalib)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT fetalib_Development
)

install(
    TARGETS fetalib_fetalib
    EXPORT fetalibTargets
    RUNTIME #
    COMPONENT fetalib_Runtime
    LIBRARY #
    COMPONENT fetalib_Runtime
    NAMELINK_COMPONENT fetalib_Development
    ARCHIVE #
    COMPONENT fetalib_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    fetalib_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE fetalib_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(fetalib_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${fetalib_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT fetalib_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${fetalib_INSTALL_CMAKEDIR}"
    COMPONENT fetalib_Development
)

install(
    EXPORT fetalibTargets
    NAMESPACE fetalib::
    DESTINATION "${fetalib_INSTALL_CMAKEDIR}"
    COMPONENT fetalib_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
