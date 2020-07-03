include(ECMMarkAsTest)
#find_package(Qt5 COMPONENTS)
find_package(KF5WindowSystem)
add_executable(foreground_booster foregroundbooster.cpp foregroundbooster.h booster_main.cpp)
target_link_libraries(
    foreground_booster
    Qt5::DBus
    KF5::WindowSystem
    KF5::CGroups)
