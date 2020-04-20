cmake_minimum_required(VERSION 3.10)

set(KF5_VERSION "5.67.0") # handled by release scripts
project(KCGroups VERSION ${KF5_VERSION})

include(FeatureSummary)
find_package(ECM 5.67.0 NO_MODULE)
set_package_properties(
    ECM
    PROPERTIES
    TYPE
    REQUIRED
    DESCRIPTION
    "Extra CMake Modules."
    URL
    "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND
                FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMQtDeclareLoggingCategory)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(REQUIRED_QT_VERSION 5.11.0)
find_package(Qt5DBus ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE)

include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMAddQch)

ecm_setup_version(
    PROJECT
    VARIABLE_PREFIX
    KCGROUPS
    VERSION_HEADER
    "${CMAKE_CURRENT_BINARY_DIR}/kcgroups_version.h"
    PACKAGE_VERSION_FILE
    "${CMAKE_CURRENT_BINARY_DIR}/KF5CGroupsConfigVersion.cmake"
    SOVERSION
    1)

add_subdirectory(src)
add_subdirectory(foreground-booster)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KCGroups")

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5CGroups_QCH
        FILE KF5CGroupsQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5CGroupsQchTargets.cmake\")")
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5CGroupsConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5CGroupsConfig.cmake" INSTALL_DESTINATION
    ${CMAKECONFIG_INSTALL_DIR})

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/KF5CGroupsConfig.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/KF5CGroupsConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel)

install(
    EXPORT KF5CGroupsTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KF5CGroupsTargets.cmake
    NAMESPACE KF5::)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/kcgroups_version.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}
    COMPONENT Devel)

# contains list of debug categories, for kdebugsettings
install(FILES kcgroups.categories
        DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
