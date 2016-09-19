project( SPIDER )
cmake_minimum_required( VERSION 2.6 )
add_subdirectory( md5 )

set( CMAKE_CXX_FLAGS "-O2 -Wall -g" )
add_definitions(  )

set( PROCESS_NAME happiness)
aux_source_directory( . SPIDER_SRC )
add_executable( ${PROCESS_NAME} ${SPIDER_SRC} )

target_link_libraries( ${PROCESS_NAME} pthread )
target_link_libraries( ${PROCESS_NAME} md5 )

# set module relational
set( CMAKE_MODULE_PATH modules )
set( COMMON_INC_DIR ../../../commonlib/include )
IF ( CMAKE_SIZEOF_VOID_P  EQUAL 8 )
	set( COMMON_LIB_DIR ../../../commonlib/lib64 )
ELSE()
	set( COMMON_LIB_DIR ../../../commonlib/lib )
ENDIF()

find_package( libicethread  REQUIRED )
find_package( libicenetwork REQUIRED )
find_package( libgflags     REQUIRED )
find_package( liblog4cplus  REQUIRED )
find_package( libkittylog   REQUIRED )

message( STATUS ${ICETHREAD_LIB_DIR} )
message( STATUS ${ICETHREAD_INC_DIR} )
message( STATUS ${ICENETWORK_LIB_DIR} )
message( STATUS ${ICENETWORK_INC_DIR} )

IF ( ICETHREAD_LIB_DIR AND ICETHREAD_INC_DIR )
    message( STATUS "ice thread.............." )
    target_link_libraries( ${PROCESS_NAME} ${ICETHREAD_LIB_DIR} )
ENDIF ( ICETHREAD_LIB_DIR AND ICETHREAD_INC_DIR )

IF ( ICENETWORK_LIB_DIR AND ICENETWORK_INC_DIR )
    message( STATUS "ice network.............." )
    target_link_libraries( ${PROCESS_NAME} ${ICENETWORK_LIB_DIR} )
    include_directories( ${ICETHREAD_INC_DIR} )
ENDIF ( ICENETWORK_LIB_DIR AND ICENETWORK_INC_DIR )

IF ( GFLAGS_LIB_DIR AND GFLAGS_INC_DIR )
    message( STATUS "gflags.............." )
    target_link_libraries( ${PROCESS_NAME} ${GFLAGS_LIB_DIR} )
ENDIF ( GFLAGS_LIB_DIR AND GFLAGS_INC_DIR )

IF ( LOG4CPLUS_LIB_DIR AND LOG4CPLUS_INC_DIR )
    message( STATUS "log4cplus.............." )
    target_link_libraries( ${PROCESS_NAME} ${LOG4CPLUS_LIB_DIR} )
ENDIF ( LOG4CPLUS_LIB_DIR AND LOG4CPLUS_INC_DIR )

IF ( KITTYLOG_LIB_DIR AND KITTYLOG_INC_DIR )
    message( STATUS "kitty log.............." )
    target_link_libraries( ${PROCESS_NAME} ${KITTYLOG_LIB_DIR} )
    #include_directories( ${LOG4CPLUS_LIB_DIR} )
ENDIF ( KITTYLOG_LIB_DIR AND KITTYLOG_INC_DIR )
