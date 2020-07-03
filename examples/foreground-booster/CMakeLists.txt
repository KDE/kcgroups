include(KDEInstallDirs)

find_package(KF5WindowSystem)
add_executable(foreground_booster foregroundbooster.cpp foregroundbooster.h booster_main.cpp)
target_link_libraries(
    foreground_booster
    Qt5::DBus
    KF5::WindowSystem
    KF5::CGroups)
install(TARGETS foreground_booster ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

configure_file(foreground-booster.service.in ${CMAKE_CURRENT_BINARY_DIR}/foreground-booster.service)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/foreground-booster.service DESTINATION ${KDE_INSTALL_SYSTEMDUNITDIR})
