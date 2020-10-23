include(KDEInstallDirs)
include(ECMConfiguredInstall)

find_package(KF5 COMPONENTS WindowSystem Config DBusAddons)
if(NOT KF5_FOUND)
  message(STATUS "KWindowSystem, KDBusAddons or KConfig not found, will not build foreground-booster")
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
    KF5::DBusAddons
    KF5::CGroups)
install(TARGETS foreground_booster ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES org.kde.foreground-booster.desktop DESTINATION ${KDE_INSTALL_AUTOSTARTDIR})

ecm_install_configured_files(INPUT plasma-foreground-booster.service.in DESTINATION ${KDE_INSTALL_SYSTEMDUSERUNITDIR})
