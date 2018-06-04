IF(TeamCity)
    set(TestArgs "-teamcity;-o;teamcity.txt")
ENDIF(TeamCity)

add_subdirectory(Plugins)
add_subdirectory(Core)
add_subdirectory(GUI)
