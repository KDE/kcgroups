include(ECMMarkAsTest)
find_package(Qt5 COMPONENTS Test Quick Qml Widgets)
if(NOT Qt5_FOUND)
  message(STATUS "QML/Test Qt5 components not found, will not build tests")
  return()
endif()

qt5_add_resources(RESOURCES qml.qrc)

add_executable(main_test main.cpp ${RESOURCES})
ecm_mark_as_test(main_test)
target_link_libraries(
    main_test
    Qt5::DBus
    Qt5::Test
    Qt5::Qml
    Qt5::Quick
    Qt5::Widgets
    KF5::CGroups)
