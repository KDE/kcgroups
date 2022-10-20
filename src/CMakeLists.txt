set(libkcgroups_SRCS
    kapplicationscope.cpp
    kapplicationscopelister.cpp
    kapplicationscopelistmodel.cpp)

ecm_qt_declare_logging_category(libkcgroups_SRCS
    HEADER kcgroups_debug.h
    IDENTIFIER KCGROUPS_LOG
    CATEGORY_NAME kf.cgroups
    OLD_CATEGORY_NAMES kf5.kcgroups
    DESCRIPTION "KCGroups"
    EXPORT KCGROUPS
)

set(DBUS_INTERFACE_XML_FILES
    dbus/org.freedesktop.DBus.Properties.xml
    dbus/org.freedesktop.systemd1.Manager.xml
    dbus/org.freedesktop.systemd1.Unit.xml)

set_property(SOURCE ${DBUS_INTERFACE_XML_FILES} PROPERTY INCLUDE dbustypes_p.h)

qt_add_dbus_interfaces(libkcgroups_SRCS ${DBUS_INTERFACE_XML_FILES})

add_library(KF5CGroups ${libkcgroups_SRCS})
generate_export_header(KF5CGroups BASE_NAME KCGroups)
add_library(KF5::CGroups ALIAS KF5CGroups)

target_include_directories(
    KF5CGroups
    INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KCGroups>")

CHECK_INCLUDE_FILE_CXX("optional" _stdOptionalAvailable)
target_compile_definitions(KF5CGroups PRIVATE $<${_stdOptionalAvailable}:STD_OPTIONAL_AVAILABLE>)

target_compile_features(KF5CGroups PUBLIC cxx_std_14)

target_link_libraries(
    KF5CGroups
    PUBLIC Qt${QT_MAJOR_VERSION}::Core
    PRIVATE Qt${QT_MAJOR_VERSION}::DBus)

set_target_properties(
    KF5CGroups
    PROPERTIES VERSION ${KCGROUPS_VERSION_STRING}
               SOVERSION ${KCGROUPS_SOVERSION}
               EXPORT_NAME CGroups)

ecm_generate_headers(
    KCGroups_HEADERS
    HEADER_NAMES
    KApplicationScope
    KApplicationScopeLister
    KApplicationScopeListModel
    REQUIRED_HEADERS KCGroups_HEADERS)

list(APPEND
    KCGroups_HEADERS
    optional.h
    optionalgadget.h
    ${CMAKE_CURRENT_BINARY_DIR}/kcgroups_export.h)

install(
    TARGETS KF5CGroups
    EXPORT KF5CGroupsTargets
    ${KF_INSTALL_TARGETS_DEFAULT_ARGS})

install(
    FILES ${KCGroups_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KCGroups
    COMPONENT Devel)

ecm_qt_install_logging_categories(
    EXPORT KCGROUPS
    FILE kcgroups.categories
    DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)

if(BUILD_QCH)
    ecm_add_qch(
        KF5CGroups_QCH
        NAME KCGroups
        BASE_NAME KF5CGroups
        VERSION ${KF5_VERSION}
        ORG_DOMAIN org.kde
        SOURCES ${KCGroups_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
        Qt${QT_MAJOR_VERSION}DBus_QCH
        Qt${QT_MAJOR_VERSION}Core_QCH
        INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KCGROUPS_EXPORT
            KCGROUPS_DEPRECATED
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
        CONFIG_TEMPLATE ${CMAKE_SOURCE_DIR}/Doxyfile.in)
endif()

include(ECMGeneratePriFile)
ecm_generate_pri_file(
    BASE_NAME
    KCGroups
    LIB_NAME
    KF5CGroups
    DEPS
    "dbus"
    FILENAME_VAR
    PRI_FILENAME
    INCLUDE_INSTALL_DIR
    ${KDE_INSTALL_INCLUDEDIR_KF5}/KCGroups)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
