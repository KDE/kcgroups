cmake_minimum_required(VERSION 3.10)

project(ForegroundBooster)

find_package(ECM 5.73.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)
include(ECMConfiguredInstall)

find_package(KF5 REQUIRED COMPONENTS CGroups Config DBusAddons)
find_package(LibTaskManager CONFIG REQUIRED)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_executable(foreground_booster foregroundbooster.cpp foregroundbooster.h booster_main.cpp)
kconfig_add_kcfg_files(foreground_booster boostersettings.kcfgc)
target_link_libraries(
    foreground_booster
    Qt5::DBus
    PW::LibTaskManager
    KF5::ConfigCore
    KF5::ConfigGui
    KF5::DBusAddons
    KF5::CGroups)
install(TARGETS foreground_booster ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES org.kde.foreground-booster.desktop DESTINATION ${KDE_INSTALL_AUTOSTARTDIR})

ecm_install_configured_files(INPUT plasma-foreground-booster.service.in DESTINATION ${KDE_INSTALL_SYSTEMDUSERUNITDIR})
