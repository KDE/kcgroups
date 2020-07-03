include(KDEInstallDirs)

find_package(KF5WindowSystem)
add_executable(foreground_booster foregroundbooster.cpp foregroundbooster.h booster_main.cpp)
target_link_libraries(
    foreground_booster
    Qt5::DBus
    KF5::WindowSystem
    KF5::CGroups)
install(TARGETS foreground_booster ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

set(AUTOSTART_FOREGROUND_BOOSTER FALSE CACHE BOOL "install foreground_booster to autostart")

if(AUTOSTART_FOREGROUND_BOOSTER)
    install(FILES org.kde.foreground-booster.desktop DESTINATION ${KDE_INSTALL_AUTOSTARTDIR})
endif()
