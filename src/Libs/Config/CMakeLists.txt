
PROJECT( MGV_CONFIG )

QT_PREPARE( Core Gui Widgets Xml )

INCLUDE_DIRECTORIES(
    ${MGV_LIBS_SOURCE_DIR}
    ${MGV_CONFIG_SOURCE_DIR}
    ${MGV_CONFIG_BINARY_DIR}
)

SET( SRC_FILES

    Config.cpp
    ConfigUser.cpp
    UserLevelDefinition.cpp
)

SET( PUB_HDR_FILES

    ConfigApi.h
    Config.h
    ConfigUser.h
    UserLevelDefinition.h
)

SET( PRI_HDR_FILES

)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT_MOC( MOC_FILES ${HDR_FILES} )

ADD_QT_LIBRARY(
    Config SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

TARGET_LINK_LIBRARIES(
    Config

    Heaven
)

INSTALL_MGV_LIB( Config )
