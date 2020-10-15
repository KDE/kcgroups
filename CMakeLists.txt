include(KDEInstallDirs)
include(ECMConfiguredInstall)

find_package(KF5WindowSystem)
find_package(KF5Config)
add_executable(foreground_booster foregroundbooster.cpp foregroundbooster.h booster_main.cpp)
target_link_libraries(
    foreground_booster
    Qt5::DBus
    KF5::WindowSystem
    KF5::ConfigCore
    KF5::CGroups)
install(TARGETS foreground_booster ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

set(AUTOSTART_FOREGROUND_BOOSTER FALSE CACHE BOOL "install foreground_booster to XDG autostart")

if(AUTOSTART_FOREGROUND_BOOSTER)
    install(FILES org.kde.foreground-booster.desktop DESTINATION ${KDE_INSTALL_AUTOSTARTDIR})
endif()

ecm_install_configured_files(INPUT plasma-foreground-booster.service.in DESTINATION ${KDE_INSTALL_SYSTEMDUSERUNITDIR})
