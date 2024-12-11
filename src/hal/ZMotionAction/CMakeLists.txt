add_subdirectory(ZMotionReset)
add_subdirectory(ZMotionAutoWork)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(ZMotionActionLib ${DIR_SYSTEM_SRCS})

target_link_libraries(ZMotionActionLib ZMotionResetLib ZMotionAutoWorkLib)