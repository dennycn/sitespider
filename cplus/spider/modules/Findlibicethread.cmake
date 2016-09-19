message( STATUS "using epoll thread lib..." )
find_path(
    ICETHREAD_INC_DIR
    thread/Lock.h
    ${COMMON_INC_DIR}
    )

find_library(
    ICETHREAD_LIB_DIR  icethread ${COMMON_LIB_DIR}
    )
