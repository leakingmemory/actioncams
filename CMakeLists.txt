cmake_minimum_required(VERSION 3.27)
project(actioncams)

set(CMAKE_CXX_STANDARD 23)

find_package(wxWidgets REQUIRED COMPONENTS core base webview)
include(${wxWidgets_USE_FILE})

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
        libavdevice
        libavfilter
        libavformat
        libavcodec
        libswresample
        libswscale
        libavutil
)

add_subdirectory(ff)

add_executable(actioncams main.cpp
        ActionCamsFrame.cpp
        ActionCamsFrame.h
        ActionCamsIds.h
        VideoDirectory.cpp
        VideoDirectory.h
        VideoFile.cpp
        VideoFile.h
        DateTime.cpp
        DateTime.h
        VideoFileDetailsDialog.cpp
        VideoFileDetailsDialog.h
        FFVideo.cpp
        FFVideo.h
        MoveVideoFileDialog.cpp
        MoveVideoFileDialog.h)

target_link_libraries(actioncams PRIVATE ${wxWidgets_LIBRARIES})
target_include_directories(actioncams PRIVATE ${wxWidgets_INCLUDE})

target_link_libraries(actioncams PRIVATE ff)

target_link_libraries(actioncams
        PRIVATE ${LIBAV_LIBRARIES}
)
