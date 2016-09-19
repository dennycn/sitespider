message( STATUS "using kittylog lib..." )
find_path(
    KITTYLOG_INC_DIR
    kittylog/KittyLog.h
    ${COMMON_INC_DIR}
    )

find_library(
    KITTYLOG_LIB_DIR  kittylog ${COMMON_LIB_DIR}
    )
