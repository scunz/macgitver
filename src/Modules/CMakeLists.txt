
MACRO(ADD_MGV_MODULE _Name )

    SET( _Target Mod${_Name} )

    ADD_QT_LIBRARY( ${_Target} MODULE ${ARGN} )

    SET_TARGET_PROPERTIES(
        ${_Target}
        PROPERTIES  OUTPUT_NAME     "${_Name}"
                    PREFIX          "Mod"
                    SUFFIX          ".mgv"
    )

    TARGET_LINK_LIBRARIES(
        ${_Target}

        GitWrap Interfaces Heaven MacGitverCore Widgets
    )

    IF( UNIX )

        SET_TARGET_PROPERTIES(	${_Target}
            PROPERTIES  INSTALL_RPATH "\$ORIGIN/../lib:\$ORIGIN/"
        )

        INSTALL(
            TARGETS     ${_Target}
            RUNTIME     DESTINATION bin/macgitver/modules
            LIBRARY     DESTINATION bin/macgitver/modules
        )

    ENDIF()

ENDMACRO(ADD_MGV_MODULE)

ADD_SUBDIRECTORY( GitConfig )
ADD_SUBDIRECTORY( History )
ADD_SUBDIRECTORY( Logging )
ADD_SUBDIRECTORY( RefsViews )
ADD_SUBDIRECTORY( Remotes )
ADD_SUBDIRECTORY( Repository )
ADD_SUBDIRECTORY( Submodules )
ADD_SUBDIRECTORY( Welcome )
ADD_SUBDIRECTORY( WorkingTree )
