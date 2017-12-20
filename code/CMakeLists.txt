PROJECT(PBS17_group11_project)

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix for Debug mode, usually d on windows")
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules;${CMAKE_MODULE_PATH}")

# check c++11 compiler-flags
INCLUDE(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
CHECK_CXX_COMPILER_FLAG("-std=gnu++11" COMPILER_SUPPORTS_GNUXX11)

IF(COMPILER_SUPPORTS_CXX11)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSEIF(COMPILER_SUPPORTS_CXX0X)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
ELSEIF(COMPILER_SUPPORTS_GNUXX11)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
ELSE()
        MESSAGE(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
ENDIF()

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# set cmake policy
IF(COMMAND CMAKE_POLICY)
	CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

IF(WIN32)
	IF(MSVC)
		ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
		ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
	ENDIF(MSVC)
ELSE(WIN32)
	SET(CMAKE_CXX_FLAGS "-W -Wall -Wno-unused")
ENDIF(WIN32)

# OpenGL
FIND_PACKAGE(OpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
LINK_DIRECTORIES(${OPENGL_INCLUDE_DIR})

# GLUT
FIND_PACKAGE(GLUT)
IF(NOT GLUT_FOUND)
	SET(GLUT_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/libraries/GL)
	FIND_LIBRARY(GLUT_LIBRARY glut32 PATHS ${CMAKE_CURRENT_LIST_DIR}/libraries/GL)
	SET(GLUT_LIBRARIES ${GLUT_LIBRARY})
ENDIF(NOT GLUT_FOUND)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIRS})
LINK_DIRECTORIES(${GLUT_INCLUDE_DIRS})

# find include directory by looking for certain header file
FIND_PATH(OPENSCENEGRAPH_INCLUDE_DIR osg/Referenced
	PATHS
	$ENV{OSG_ROOT}/include
	/usr/include
	/usr/local/include
)

# find library directory by looking for certain library file
FIND_PATH(OPENSCENEGRAPH_LIB_DIR libso.so osg.lib
	PATHS
	$ENV{OSG_ROOT}/lib
	/usr/lib
	/usr/local/lib
	/usr/lib/x86_64-linux-gnu
)

# apply them to following projects
INCLUDE_DIRECTORIES(${OPENSCENEGRAPH_INCLUDE_DIR})
LINK_DIRECTORIES(${OPENSCENEGRAPH_LIB_DIR})

# CGAL
FIND_PACKAGE(CGAL QUIET COMPONENTS Core)

IF (CGAL_FOUND)
	MESSAGE(STATUS "CGAL_FOUND=true.")
  INCLUDE(${CGAL_USE_FILE})
  INCLUDE(CGAL_CreateSingleSourceCGALProgram)
ELSE()  
    MESSAGE(STATUS "This program requires the CGAL library, and will not be compiled.")
ENDIF()

INCLUDE_DIRECTORIES(${CGAL_INCLUDE_DIR})
LINK_DIRECTORIES(${CGAL_INCLUDE_DIR})

# find boost_1_65_1
FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
IF (Boost_FOUND)
    MESSAGE(STATUS "Boost_FOUND=true.")
    INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )
    LINK_DIRECTORIES( ${Boost_LIBRARY_DIRS} )
ENDIF()

MACRO(START_PROJECT)

    INCLUDE_DIRECTORIES(${EXTERNAL_INCLUDE_DIR})
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/libraries/eigen)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/libraries/json)
    ADD_EXECUTABLE(${EXAMPLE_NAME} ${SOURCES} ${COMMON_FILES})
    SET_TARGET_PROPERTIES(${EXAMPLE_NAME} PROPERTIES
		DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
		CXX_STANDARD 11
		CXX_STANDARD_REQUIRED YES
	)
	
    TARGET_LINK_LIBRARIES(${EXAMPLE_NAME}
        debug osg${CMAKE_DEBUG_POSTFIX}          optimized osg
        debug osgAnimation${CMAKE_DEBUG_POSTFIX} optimized osgAnimation
        debug osgParticle${CMAKE_DEBUG_POSTFIX}  optimized osgParticle
        debug osgDB${CMAKE_DEBUG_POSTFIX}        optimized osgDB
        debug osgGA${CMAKE_DEBUG_POSTFIX}        optimized osgGA
        debug osgText${CMAKE_DEBUG_POSTFIX}      optimized osgText
        debug osgUtil${CMAKE_DEBUG_POSTFIX}      optimized osgUtil
        debug osgViewer${CMAKE_DEBUG_POSTFIX}    optimized osgViewer
        debug OpenThreads${CMAKE_DEBUG_POSTFIX}  optimized OpenThreads
		${OPENGL_LIBRARIES}
		${GLUT_LIBRARY}
        ${Boost_LIBRARIES}
	)
    INSTALL(TARGETS ${EXAMPLE_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

ENDMACRO(START_PROJECT)

# Project
SET(EXAMPLE_NAME asteroid_field)
SET(SOURCES "")

SET(DATA_MODEL_PATH ${CMAKE_CURRENT_LIST_DIR}/data)
SET(SCENES_PATH ${CMAKE_CURRENT_LIST_DIR}/demoScenes)
SET(SCREENSHOT_PATH ${CMAKE_CURRENT_LIST_DIR}/screenshots)
SET(VERSION "0.9.1")
CONFIGURE_FILE(precompile/config.h.in ${CMAKE_CURRENT_LIST_DIR}/config.h)

FILE(GLOB main_SRCS "./*.cpp")
FILE(GLOB main_HDRS "./*.h")
SOURCE_GROUP(Core FILES ${main_SRCS} ${main_HDRS})
LIST(APPEND SOURCES ${main_SRCS} ${main_HDRS})

FILE(GLOB scene_SRCS "./scene/*.cpp")
FILE(GLOB scene_HDRS "./scene/*.h")
SOURCE_GROUP(Scene FILES ${scene_SRCS} ${scene_HDRS})
LIST(APPEND SOURCES ${scene_SRCS} ${scene_HDRS})

FILE(GLOB osg_SRCS "./osg/*.cpp")
FILE(GLOB osg_HDRS "./osg/*.h")
SOURCE_GROUP(Osg FILES ${osg_SRCS} ${osg_HDRS})
LIST(APPEND SOURCES ${osg_SRCS} ${osg_HDRS})

FILE(GLOB osg_visitors_SRCS "./osg/visitors/*.cpp")
FILE(GLOB osg_visitors_HDRS "./osg/visitors/*.h")
SOURCE_GROUP(Osg\\Visitors FILES ${osg_visitors_SRCS} ${osg_visitors_HDRS})
LIST(APPEND SOURCES ${osg_visitors_SRCS} ${osg_visitors_HDRS})

FILE(GLOB osg_shaders_SRCS "./osg/shaders/*.cpp")
FILE(GLOB osg_shaders_HDRS "./osg/shaders/*.h")
SOURCE_GROUP(Osg\\Shaders FILES ${osg_shaders_SRCS} ${osg_shaders_HDRS})
LIST(APPEND SOURCES ${osg_shaders_SRCS} ${osg_shaders_HDRS})

FILE(GLOB osg_events_SRCS "./osg/events/*.cpp")
FILE(GLOB osg_events_HDRS "./osg/events/*.h")
SOURCE_GROUP(Osg\\Events FILES ${osg_events_SRCS} ${osg_events_HDRS})
LIST(APPEND SOURCES ${osg_events_SRCS} ${osg_events_HDRS})

FILE(GLOB osg_particles_SRCS "./osg/particles/*.cpp")
FILE(GLOB osg_particles_HDRS "./osg/particles/*.h")
SOURCE_GROUP(Osg\\Particles FILES ${osg_particles_SRCS} ${osg_particles_HDRS})
LIST(APPEND SOURCES ${osg_particles_SRCS} ${osg_particles_HDRS})

FILE(GLOB physics_SRCS "./physics/*.cpp")
FILE(GLOB physics_HDRS "./physics/*.h")
SOURCE_GROUP(Physics FILES ${physics_SRCS} ${physics_HDRS})
LIST(APPEND SOURCES ${physics_SRCS} ${physics_HDRS})

FILE(GLOB graphics_SRCS "./graphics/*.cpp")
FILE(GLOB graphics_HDRS "./graphics/*.h")
SOURCE_GROUP(Graphics FILES ${graphics_SRCS} ${graphics_HDRS})
LIST(APPEND SOURCES ${graphics_SRCS} ${graphics_HDRS})

START_PROJECT()