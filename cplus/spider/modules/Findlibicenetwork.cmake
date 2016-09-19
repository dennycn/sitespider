message( STATUS "using epoll network lib..." )
find_path(
    ICENETWORK_INC_DIR
    network/Epollor.h
    ${COMMON_INC_DIR}
    )

find_library(
    ICENETWORK_LIB_DIR  icenetwork ${COMMON_LIB_DIR}
    )
