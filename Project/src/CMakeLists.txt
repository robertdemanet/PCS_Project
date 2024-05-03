list(APPEND src_sources "${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp")
list(APPEND src_header "${CMAKE_CURRENT_SOURCE_DIR}/Utils.hpp")
list(APPEND src_include "${CMAKE_CURRENT_SOURCE_DIR}")
list(APPEND src_header "${CMAKE_CURRENT_SOURCE_DIR}/GeometryLibrary.hpp")
set(src_sources ${src_sources} PARENT_SCOPE)
set(src_header ${src_header} PARENT_SCOPE)
set(src_include ${src_include} PARENT_SCOPE)

