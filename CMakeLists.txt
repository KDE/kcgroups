include(KDEInstallDirs)
include(ECMConfiguredInstall)

find_package(KF5WindowSystem)
find_package(KF5Config)
if(NOT KF5WindowSystem_FOUND OR NOT KF5Config_FOUND)
  message(STATUS "KF5WindowSystem or KF5Config not found, will not build foreground-booster")
  return()
endif()

add_executable(foreground_booster foregroundbooster.cpp foregroundbooster.h booster_main.cpp)
kconfig_add_kcfg_files(foreground_booster boostersettings.kcfgc)
target_link_libraries(
    foreground_booster
    Qt5::DBus
    KF5::WindowSystem
    KF5::ConfigCore
    KF5::ConfigGui
    KF5::CGroups)
install(TARGETS foreground_booster ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES org.kde.foreground-booster.desktop DESTINATION ${KDE_INSTALL_AUTOSTARTDIR})

ecm_install_configured_files(INPUT plasma-foreground-booster.service.in DESTINATION ${KDE_INSTALL_SYSTEMDUSERUNITDIR})
