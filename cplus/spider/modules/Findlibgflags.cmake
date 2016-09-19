message( STATUS "using gflags lib..." )
# can be modified by your real env
find_path(
    GFLAGS_INC_DIR
    gflags/gflags.h
    ${COMMON_INC_DIR}
    )

find_library(
    GFLAGS_LIB_DIR gflags ${COMMON_LIB_DIR}
    )
