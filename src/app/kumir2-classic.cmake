set(CONFIGURATION_TEMPLATE
    "Editor,Browser,Actor*,KumirAnalizer,KumirCodeGenerator,KumirCodeRun,!CoreGUI\(notabs,icon=classic,nostartpage,nosessions\)"
)
set(SPLASHSCREEN
    "coregui/splashscreens/classic.png"
)

set(SRC main.cpp)
if(WIN32)
    list(APPEND SRC kumir2-classic.rc)
endif(WIN32)

add_executable(kumir2-classic WIN32 ${SRC})
target_link_libraries(kumir2-classic ${QT_LIBRARIES} ExtensionSystem)
set_property(TARGET kumir2-classic APPEND PROPERTY COMPILE_DEFINITIONS CONFIGURATION_TEMPLATE="${CONFIGURATION_TEMPLATE}")
set_property(TARGET kumir2-classic APPEND PROPERTY COMPILE_DEFINITIONS SPLASHSCREEN="${SPLASHSCREEN}")
install(TARGETS kumir2-classic DESTINATION ${EXEC_DIR})
