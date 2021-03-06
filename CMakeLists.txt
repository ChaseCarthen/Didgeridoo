PROJECT(INSTRUMENT)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
FIND_PACKAGE(CAVR REQUIRED)
SET(HOME_BUILD_PATH ${PROJECT_SOURCE_DIR})

FIND_PACKAGE(Freetype)
FIND_PACKAGE(Lua51 REQUIRED)
FIND_PACKAGE(GLOG REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
#FIND_PACKAGE(CAVR REQUIRED)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
SET(CXX11_FLAGS -std=gnu++11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")


#add_definitions(-DGLM_FORCE_RADIANS)


INCLUDE_DIRECTORIES(
	"${LUA_INCLUDE_DIR}"
	"${CAVR_INCLUDE_PATH}"
  	"${PROJECT_SOURCE_DIR}/src/program"
  	"${PROJECT_SOURCE_DIR}/src/util"
  	"${PROJECT_SOURCE_DIR}/src/math"
  	"${PROJECT_SOURCE_DIR}/src/gfx"
  	"${PROJECT_SOURCE_DIR}/src/hydra"
	"${GLOG_INCLUDE_DIRS}"
  "/usr/local/include"
  "${FREETYPE_INCLUDE_DIR_freetype2}"
)

LINK_DIRECTORIES(
	"${CAVR_LIBRARIES}"
)

# Set Includes
SET(INCLUDES ${PROJECT_SOURCE_DIR}/include)
#MESSAGE( STATUS "FILES: " ${INCLUDES} )
INCLUDE_DIRECTORIES(${INCLUDES})

# Set sources
file(GLOB_RECURSE SOURCES "src/gfx/*.cpp")
file(GLOB_RECURSE SOURCES2 "src/math/*.cpp")
file(GLOB_RECURSE SOURCES3 "src/program/*.cpp")
file(GLOB_RECURSE SOURCES4 "src/util/*.cpp")
ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES} ${SOURCES2} ${SOURCES3} ${SOURCES4})

add_custom_target("${PROJECT_NAME}_SUCCESSFUL" ALL
                  DEPENDS ${PROJECT_NAME}
                  COMMAND ${CMAKE_COMMAND} -E echo ""
                  COMMAND ${CMAKE_COMMAND} -E echo "====================="
                  COMMAND ${CMAKE_COMMAND} -E echo "  Compile complete!"
                  COMMAND ${CMAKE_COMMAND} -E echo "====================="
                  COMMAND ${CMAKE_COMMAND} -E echo "${CMAKE_CURRENT_BINARY_DIR}"
                  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/shaders/ ${CMAKE_CURRENT_BINARY_DIR}/shaders
		  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/media/ ${CMAKE_CURRENT_BINARY_DIR}/media
                  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/config/ ${CMAKE_CURRENT_BINARY_DIR}/config
                  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/bin/ ${CMAKE_CURRENT_BINARY_DIR}
                  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/config/cavrconfig.lua ${CMAKE_CURRENT_BINARY_DIR}
                  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/config/cavrplugins.lua ${CMAKE_CURRENT_BINARY_DIR}
                 )

TARGET_LINK_LIBRARIES(${PROJECT_NAME} cavr cavrgl cavrgfx ${GLOG_LIBRARIES} ${OPENGL_LIBRARY} ${GLEW_LIBRARIES} ${CAVR_LIBRARIES} ${FREETYPE_LIBRARIES} alut openal freeimage)

#TARGET_LINK_LIBRARIES(${PROJECT_NAME} LINK_PUBLIC )

