cmake_minimum_required(VERSION 3.16.0)
project(Spark VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 17)

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

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/repo/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

#---------ADD SOURCE AND HEADER FILES ---------------
set(SOURCE_FILE_NAMES
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
list(TRANSFORM SOURCE_FILE_NAMES PREPEND "Arcade_16/")


set(HEADER_FILE_NAMES
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
list(TRANSFORM HEADER_FILE_NAMES PREPEND "Arcade_16/")


#configure_file(SparkConfig.h.in SparkConfig.h)

add_executable(Spark ${SOURCE_FILE_NAMES} ${HEADER_FILE_NAMES})

target_include_directories(Spark PRIVATE ${PROJECT_SOURCE_DIR}/src/include/)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})


#-----------------------------------------------------
#---------ADD SFML LIBRARY----------------------------

add_subdirectory(SFML)
set(SFML_DIR ${PROJECT_SOURCE_DIR}/SFML/)
target_include_directories(Spark PRIVATE ${PROJECT_SOURCE_DIR}/libs/SFML/include/)

find_package(SFML 2.5
COMPONENTS 
system window graphics network audio REQUIRED)


target_link_libraries(Spark sfml-graphics sfml-audio)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack) 
