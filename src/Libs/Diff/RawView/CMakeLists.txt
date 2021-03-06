
PROJECT( MGV_DIFF_RAWVIEW )

QT_PREPARE( Core Gui Widgets )

INCLUDE_DIRECTORIES(
    ${MGV_LIBS_SOURCE_DIR}
    ${MGV_DIFF_RAWVIEW_SOURCE_DIR}
    ${MGV_DIFF_RAWVIEW_BINARY_DIR}
)

SET( SRC_FILES

    DiffRawView.cpp
    DiffRawHighlighter.cpp
)

SET( PUB_HDR_FILES

    DiffRawViewApi.h
    DiffRawView.h
)

SET( PRI_HDR_FILES

    DiffRawHighlighter.h
)

SET( HDR_FILES ${PRI_HDR_FILES} ${PUB_HDR_FILES} )

QT_MOC( MOC_FILES ${HDR_FILES} )

ADD_QT_LIBRARY(
    DiffRawView SHARED

    ${SRC_FILES}
    ${HDR_FILES}
    ${MOC_FILES}
)

TARGET_LINK_LIBRARIES(
    DiffRawView

    DiffModel
    Config
)

INSTALL_MGV_LIB( DiffRawView )
