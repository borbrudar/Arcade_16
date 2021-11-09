cmake_minimum_required(VERSION 3.16.0)
if(WIN32)
    #default install path, so no spaces
    set(CMAKE_INSTALL_PREFIX C:/ CACHE PATH "default path" FORCE)
endif()
project(Arcade VERSION 0.1.1)
set (CMAKE_CXX_STANDARD 17)
include(GNUInstallDirs)

include(CTest)
enable_testing()


find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/SFML/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

#---------ADD SOURCE AND HEADER FILES ---------------
set(SOURCE_FILE_NAMES
   Animation
   Arkanoid
   Asteroids
   Astro 
   Box 
   Button 
   Enemy 
   Entity 
   Ghost 
   Manimation
   Mario
   Menu
   MP
   Pac_Man
   Pong 
   Shield 
   Simon 
   Space_Invaders 
   Super_Mario
   Tetris
   Main
)

list(TRANSFORM SOURCE_FILE_NAMES APPEND ".cpp")
list(TRANSFORM SOURCE_FILE_NAMES PREPEND "src/src/")


set(HEADER_FILE_NAMES
 Animation
 Arkanoid
 Asteroids
 Astro 
 Box 
 Button 
 Enemy 
 Entity 
 Ghost 
 Manimation
 Mario
 Menu
 MP
 Pac_Man
 Pong 
 Shield 
 Simon 
 Space_Invaders 
 Super_Mario
 Tetris
 BoxEnum
 Pellet 
 State 
 stateEnum 
)

list(TRANSFORM HEADER_FILE_NAMES APPEND ".h")
list(TRANSFORM HEADER_FILE_NAMES PREPEND "src/include/")

if(UNIX)
set(CMAKE_INSTALL_RPATH $ORIGIN/../lib)
endif()
add_executable(${PROJECT_NAME} ${SOURCE_FILE_NAMES})

target_include_directories(Arcade
PRIVATE
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/>
$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/src/include/>
)

#openal32
if(WIN32)
 target_include_directories(Arcade
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/win10/
    )

    #sfml
 add_custom_command(TARGET Arcade POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_BINARY_DIR}/external/SFML/lib/sfml-audio-2.dll"
        "${CMAKE_CURRENT_BINARY_DIR}/external/SFML/lib/sfml-graphics-2.dll"
        "${CMAKE_CURRENT_BINARY_DIR}/external/SFML/lib/sfml-system-2.dll"
        "${CMAKE_CURRENT_BINARY_DIR}/external/SFML/lib/sfml-window-2.dll"
         ${CMAKE_CURRENT_BINARY_DIR})

    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/win10/OpenAL32.dll"
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endif()

#-----------COPY RES DIR-------------------------
file(COPY res DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")


#---------ADD SFML LIBRARY----------------------------

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/SFML")
target_link_libraries(Arcade sfml-graphics sfml-audio)

#--------INSTALL LINUX---------------------------------
install(TARGETS Arcade 
RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_INSTALL_BINDIR})

if(WIN32)
    #sfml
    add_custom_command(TARGET Arcade POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_BINARY_DIR}/external/SFML/lib/sfml-audio-2.dll"
        "${CMAKE_CURRENT_BINARY_DIR}/external/SFML/lib/sfml-graphics-2.dll"
        "${CMAKE_CURRENT_BINARY_DIR}/external/SFML/lib/sfml-system-2.dll"
        "${CMAKE_CURRENT_BINARY_DIR}/external/SFML/lib/sfml-window-2.dll"
         ${CMAKE_CURRENT_BINARY_DIR})
    #openal ig
    add_custom_command(TARGET Arcade POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
                "${CMAKE_CURRENT_SOURCE_DIR}/win10/OpenAL32.dll"
                ${CMAKE_INSTALL_BINDIR})
elseif(UNIX)
    install(TARGETS Arcade 
       LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

#-------PACKAGING------------------------------------
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack) 
