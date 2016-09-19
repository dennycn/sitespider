message( STATUS "using xmlcc lib..." )
find_path(
    XMLCC_INC_DIR
    xmlcc/xmlcc.h
    ${COMMON_INC_DIR}
    )

find_library(
    XMLCC_LIB_DIR  xmlcc ${COMMON_LIB_DIR}
    )
