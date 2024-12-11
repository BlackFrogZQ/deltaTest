set(CMAKE_AUTOMOC ON)

add_subdirectory(camera)
add_subdirectory(ZMotion)
add_subdirectory(ZMotionAction)
aux_source_directory(. DIR_SYSTEM_SRCS)

add_library(halLib ${DIR_SYSTEM_SRCS})

target_link_libraries(halLib
    baslerCameraLib
    ZMotionLib
    ZMotionActionLib)