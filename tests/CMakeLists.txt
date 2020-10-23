include(ECMMarkAsTest)
find_package(Qt5 COMPONENTS Test Quick Qml Widgets)
if(NOT Qt5_FOUND)
  message(STATUS "QML/Test Qt5 components not found, will not build tests")
  return()
endif()

qt5_add_resources(RESOURCES qml.qrc)

add_executable(spinner spinner.cpp ${RESOURCES})
target_link_libraries(
    spinner
    Qt5::Qml
    Qt5::Quick
    Qt5::Widgets)

add_executable(foregroundtest foregroundtest.cpp)
ecm_mark_as_test(foregroundtest)
add_dependencies(foregroundtest spinner)
target_link_libraries(
    foregroundtest
    Qt5::Test)
