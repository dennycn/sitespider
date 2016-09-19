message( STATUS "using log4cplus lib..." )
find_path(
    LOG4CPLUS_INC_DIR
    log4cplus/logger.h
    ${COMMON_INC_DIR}
    )

find_library(
    LOG4CPLUS_LIB_DIR  log4cplus ${COMMON_LIB_DIR}
    )
