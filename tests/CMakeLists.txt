include(ECMMarkAsTest)
find_package(Qt6 COMPONENTS Test Quick Qml Widgets)
if(NOT Qt6_FOUND)
  message(STATUS "QML/Test Qt components not found, will not build tests")
  return()
endif()

qt_add_resources(RESOURCES qml.qrc)

add_executable(main_test main.cpp ${RESOURCES})
ecm_mark_as_test(main_test)
target_link_libraries(
    main_test
    Qt::DBus
    Qt::Test
    Qt::Qml
    Qt::Quick
    Qt::Widgets
    KF6::CGroups)
