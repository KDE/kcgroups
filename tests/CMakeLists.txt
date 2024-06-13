include(ECMMarkAsTest)
find_package(Qt6 COMPONENTS Test Quick Qml Widgets)
if(NOT Qt6_FOUND)
  message(STATUS "QML/Test Qt6 components not found, will not build tests")
  return()
endif()

qt6_add_resources(RESOURCES qml.qrc)

add_executable(spinner spinner.cpp ${RESOURCES})
target_link_libraries(
    spinner
    Qt6::Qml
    Qt6::Quick
    Qt6::Widgets)

add_executable(foregroundtest foregroundtest.cpp)
ecm_mark_as_test(foregroundtest)
add_dependencies(foregroundtest spinner)
target_link_libraries(
    foregroundtest
    Qt6::Test)
