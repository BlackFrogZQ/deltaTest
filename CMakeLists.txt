cmake_minimum_required(VERSION 3.0.0)
project(deltaTest VERSION 0.1.0)

# 打开我的电脑>属性>高级系统设置>环境变量
# 新建用户变量 变量名"QT5.12.5"，变量值"qt5.12.5下编译器的目录，比如我的是'C:\Qt\Qt5.12.5\5.12.5\msvc2017_64'"
# 同理 新建用户变量 变量名"OrionLib"，变量值"OrionLib所在目录，比如我的是在'C:\OrionLib'"
# 确定保存即可，cmake访问系统变量 $ENV{变量名},如:访问上述变量'QT5.12.5变量' $ENV{QT5.12.5}
# 添加用户变量后需重启
# 自动打包需要在setting.json 添加 "cmake.setBuildTypeOnMultiConfig": true,
SET(CMAKE_PREFIX_PATH $ENV{QT_Dir})
include_directories(${PROJECT_SOURCE_DIR})
include_directories($ENV{QT_Dir}\\include)
include_directories($ENV{QT_Dir}\\include\\QtGui)
include_directories($ENV{QT_Dir}\\include\\QtWidgets)
include_directories($ENV{QT_Dir}\\include\\QtCore)
include_directories($ENV{QT_Dir}\\include\\QtNetwork)
include_directories($ENV{QT_Dir}\\include\\QtSerialBus)
include_directories($ENV{QT_Dir}\\include\\QtSerialPort)
include_directories($ENV{QT_Dir}\\include\\QtConcurrent)
include_directories($ENV{QT_Dir}\\include\\QtMultimedia)
include_directories($ENV{QT_Dir}\\include\\QtMultimediaWidgets)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
add_definitions(-DNOMINMAX)
message("Looking for Qt...")
find_package(Qt5 COMPONENTS Core Gui Network SerialBus Widgets SerialPort Concurrent Multimedia MultimediaWidgets Qml REQUIRED)

if(${Qt5_FOUND})
    message("Found Qt " ${Qt5_VERSION})
else()
    message(FATAL_ERROR "Couldn't find Qt")
endif()

execute_process(
    COMMAND chcp 65001
)

set(Qt5model
    Qt5::Widgets
    Qt5::Network
    Qt5::SerialBus
    Qt5::Gui
    Qt5::Core
    Qt5::SerialPort
    Qt5::Concurrent)

set(isRelease TRUE)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(isRelease FALSE)
    add_definitions(-DTrace)
endif()

if(isRelease)
    file(GLOB OpencvLIBS "${PROJECT_SOURCE_DIR}/lib/opencv/release/*.lib")
    file(GLOB OpencvDlls "${PROJECT_SOURCE_DIR}/lib/opencv/release/*.dll")
else()
    file(GLOB OpencvLIBS "${PROJECT_SOURCE_DIR}/lib/opencv/debug/*.lib")
    file(GLOB OpencvDlls "${PROJECT_SOURCE_DIR}/lib/opencv/debug/*.dll")
endif()
file(GLOB PythonLIBS "${PROJECT_SOURCE_DIR}/lib/python312/*.lib")
file(GLOB PythonDlls "${PROJECT_SOURCE_DIR}/lib/python312/*.dll")
file(GLOB numpyLIBS "${PROJECT_SOURCE_DIR}/lib/numpy/*.lib")
file(GLOB numpyDlls "${PROJECT_SOURCE_DIR}/lib/numpy/*.dll")
file(GLOB ZMotionLIBS "${PROJECT_SOURCE_DIR}/lib/ZMotion/*.lib")
file(GLOB ZMotionDlls "${PROJECT_SOURCE_DIR}/lib/ZMotion/*.dll")
file(GLOB baslerLIBS "${PROJECT_SOURCE_DIR}/lib/pylon5x64/*.lib")
file(GLOB baslerDlls "${PROJECT_SOURCE_DIR}/lib/pylon5x64/*.dll")
# 所有非Qt通用dll库 不一定在统一目录 可直接追加目录
set_property(GLOBAL APPEND PROPERTY LIBS ${OpencvLIBS} ${PythonLIBS} ${numpyLIBS} ${ZMotionLIBS} ${baslerLIBS})
# 所有非Qt通用dll库 不一定在统一目录 可直接追加目录
# set_property(GLOBAL APPEND PROPERTY libDlls ${OpencvDlls} ${PythonDlls} ${numpyDlls} ${ZMotionDlls} ${baslerDlls})
set_property(GLOBAL APPEND PROPERTY libDlls ${OpencvDlls} ${numpyDlls} ${ZMotionDlls} ${baslerDlls})
get_property(currentLibs GLOBAL PROPERTY LIBS)

macro(Macro_SetOutPath outPath)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${outPath})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${outPath}/libs)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${outPath}/libs)
    set(LIBRARY_OUTPUT_PATH ${outPath}/libs)
endmacro()
Macro_SetOutPath(${CMAKE_CURRENT_BINARY_DIR})

include_directories(./)
include_directories(${PROJECT_SOURCE_DIR}/include)
# opencv
include_directories(${PROJECT_SOURCE_DIR}/include/opencv)
include_directories(${PROJECT_SOURCE_DIR}/include/opencv/opencv2)
# python
include_directories(${PROJECT_SOURCE_DIR}/include/python312)
link_directories("${PROJECT_SOURCE_DIR}/lib/python312")
# numpy
include_directories(${PROJECT_SOURCE_DIR}/include/numpy)
link_directories("${PROJECT_SOURCE_DIR}/lib/numpy")
# ZMotion
include_directories(${PROJECT_SOURCE_DIR}/include/ZMotion)
link_directories("${PROJECT_SOURCE_DIR}/lib/ZMotion")
# basler
include_directories(${PROJECT_SOURCE_DIR}/include/pylon5)
link_directories("${PROJECT_SOURCE_DIR}/lib/pylon5x64")

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/qtSingleApplication)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/ui)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/system)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/para)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/hal)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/imageProcess)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/qtSingleApplication)
set(paraServerCpp
    src/para/define/paraDef.cpp
    src/para/define/cameraDef.cpp
    src/para/define/ZMotionDef.cpp)

qt5_add_resources(sources qml.qrc)

# Specify MSVC UTF-8 encoding
# add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
# add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(${PROJECT_NAME}
    WIN32 # If you need a terminal for debug, please comment this statement#是在调试控制台输出还是在终端输出
    ${paraServerCpp}
    ${sources}
    main.cpp
    ico.rc)

target_link_libraries(${PROJECT_NAME} PRIVATE ${Qt5model} ${currentLibs}
                        qtSingleApplicationLib halLib sysLib imageProcessLib paraLib uiLib)


# 打包
function(Fun_CopyDll exeBasicName)
set(exeDir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
message(STATUS "${exeBasicName}.exe 所在目录:" ${exeDir})
file(GLOB currentDlls "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/*.dll")

foreach(currentDll ${currentDlls})
    file(REMOVE ${currentDll})
endforeach()

get_property(currentDlls GLOBAL PROPERTY libDlls)
string(REPLACE ";" "\n\t" outLibDllNames "${currentDlls}")
message(STATUS "拷贝 libDlls 库...\n\t${outLibDllNames}")
file(COPY ${currentDlls} DESTINATION ${exeDir})
message(STATUS "拷贝结束...")
message(STATUS "运行qt打包程序...")
execute_process(
    COMMAND windeployqt ${exeDir}\\${exeBasicName}.exe
    WORKING_DIRECTORY $ENV{QT_Dir}\\bin
    OUTPUT_QUIET

    # ERROR_QUIET
    ERROR_STRIP_TRAILING_WHITESPACE
    ERROR_VARIABLE errorMsg
)

if("${errorMsg}" STREQUAL "")
    message(STATUS "qt打包成功...")
else()
    message(STATUS "copy error:${errorMsg}")
    message(WARNING "qt打包失败...请按F7生成exe后,重新运行cmake(在此文件中ctrl+s)")
endif()
endfunction()

Fun_CopyDll(${PROJECT_NAME})

# if(DEFINED CMAKE_BUILD_TYPE)
#     set(exeDir ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
#     # 所有非Qt dll库 不一定在统一目录 可直接追加目录
#     file(GLOB libDlls
#         "${PROJECT_SOURCE_DIR}/lib/halcon12x64/*.dll"
#         "${PROJECT_SOURCE_DIR}/lib/pylon5x64/*.dll")
#     message("${PROJECT_NAME}.exe 所在目录:" ${exeDir})
#     message("拷贝 libDlls 库到程序运行目录...")
#     string(REPLACE ";" "\n\t" outLibDllNames "${libDlls}")
#     message("libDlls:\n\t${outLibDllNames}")
#     file(COPY ${libDlls} DESTINATION ${exeDir})
#     message("拷贝结束...")
#     message("运行qt打包程序...")
#     execute_process(
#         COMMAND windeployqt ${exeDir}\\${PROJECT_NAME}.exe
#         WORKING_DIRECTORY $ENV{QT5.15.2}\\bin
#     )
#     message("qt打包结束")
# else()
#     message("未在用户设置中设置 'cmake.setBuildTypeOnMultiConfig'为 true,无法打包")
# endif()