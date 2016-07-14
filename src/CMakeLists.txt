#
# Cmake qml build system template
#

# List of sources
#
# Define the C++ source files
SET(Project_Srcs
    main.cpp
    launchermodel.cpp
)

# Define the resource files
SET(Project_Resources
    qml/qml.qrc
)

SET(Project_3D_PARTY_LIBRARIES
	"")

INCLUDE_DIRECTORIES(${LIBVLCQT_INCLUDE_DIR})
INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
)


ADD_EXECUTABLE(${PROJECT_NAME} ${Project_Resources} ${Project_Srcs})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} Qt5::Quick Qt5::Widgets KF5::ConfigCore ${Project_3D_PARTY_LIBRARIES} )

INSTALL(FILES ${DESCRIPTOR} DESTINATION /usr/share/applications)

INSTALL(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${PROJECT_INSTALL_BIN_DIR}
    BUNDLE DESTINATION .
)
