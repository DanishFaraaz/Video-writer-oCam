project (CMT)

cmake_minimum_required (VERSION 2.6)

## load in pkg-config support
find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)

## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from about to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

option(BUILD_TRAX_CLIENT "Build the trax client." OFF)

find_package(OpenCV REQUIRED)
find_library(HAS_LIBUDEV udev)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

#add_definitions ("-Wall")
set( CMAKE_CXX_FLAGS "-Wall -pthread -std=c++11 -lv4l2 -udev" )
set(USB_INCLUDE_DIR ${LIBUDEV_INCLUDE_DIR})
set(V4L2_LIBRARIES v4l2)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${LIBUDEV_INCLUDE_DIR})
include_directories(${V4L2_LIBRARIES})

if(WIN32)
add_executable (cmt common.cpp gui.cpp main.cpp
    CMT.cpp Consensus.cpp Fusion.cpp Matcher.cpp Tracker.cpp
    fastcluster/fastcluster.cpp getopt/getopt.cpp withrobot_camera.cpp withrobot_utility.cpp
    )

add_executable(surf surf.cpp)
add_executable(kcf kcf.cpp)
else()
add_executable (cmt common.cpp gui.cpp main.cpp
    CMT.cpp Consensus.cpp Fusion.cpp Matcher.cpp Tracker.cpp
    fastcluster/fastcluster.cpp withrobot_camera.cpp withrobot_utility.cpp)
add_executable (cmt_test common.cpp gui.cpp cmt_test.cpp
    CMT.cpp Consensus.cpp Fusion.cpp Matcher.cpp Tracker.cpp
    fastcluster/fastcluster.cpp withrobot_camera.cpp withrobot_utility.cpp)
add_executable (cnt_tracker common.cpp gui.cpp cnt_tracker.cpp
    CMT.cpp Consensus.cpp Fusion.cpp Matcher.cpp Tracker.cpp
    fastcluster/fastcluster.cpp withrobot_camera.cpp withrobot_utility.cpp)
add_executable (video_write common.cpp gui.cpp video_write.cpp
    CMT.cpp Consensus.cpp Fusion.cpp Matcher.cpp Tracker.cpp
    fastcluster/fastcluster.cpp withrobot_camera.cpp withrobot_utility.cpp)
add_executable(surf surf.cpp withrobot_camera.cpp withrobot_utility.cpp)
add_executable(surf_match surf_match.cpp)
add_executable(kcf kcf.cpp)
add_executable(calibrate calibrate_camera.cpp)
add_executable(client client.cpp)
add_executable(server server.cpp)
add_executable(sub sub.cpp)
add_executable(contours contours.cpp withrobot_camera.cpp withrobot_utility.cpp)
add_executable(writer writer.cpp withrobot_camera.cpp withrobot_utility.cpp)

endif()

target_link_libraries(cmt ${OpenCV_LIBS} udev ${V4L2_LIBRARIES})
target_link_libraries(cmt_test ${OpenCV_LIBS} udev ${V4L2_LIBRARIES} ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(cnt_tracker ${OpenCV_LIBS} udev ${V4L2_LIBRARIES} ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(video_write ${OpenCV_LIBS} udev ${V4L2_LIBRARIES} ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(surf ${OpenCV_LIBS} udev ${V4L2_LIBRARIES})
target_link_libraries(surf_match ${OpenCV_LIBS})
target_link_libraries(kcf ${OpenCV_LIBS})
target_link_libraries(contours ${OpenCV_LIBS} udev ${V4L2_LIBRARIES} ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(writer ${OpenCV_LIBS} udev ${V4L2_LIBRARIES})
target_link_libraries(calibrate ${OpenCV_LIBS} udev ${V4L2_LIBRARIES})

target_include_directories(video_write PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_include_directories(client PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_include_directories(server PUBLIC ${ZeroMQ_INCLUDE_DIR})

target_link_libraries(video_write PUBLIC ${ZeroMQ_LIBRARY})
target_link_libraries(cmt_test PUBLIC ${ZeroMQ_LIBRARY})
target_link_libraries(cnt_tracker PUBLIC ${ZeroMQ_LIBRARY})
target_link_libraries(sub PUBLIC ${ZeroMQ_LIBRARY})
target_link_libraries(client PUBLIC ${ZeroMQ_LIBRARY})
target_link_libraries(server PUBLIC ${ZeroMQ_LIBRARY})


if(BUILD_TRAX_CLIENT)
    set(TRAX_DIR "" CACHE FILEPATH "Path to trax")

    include_directories(${TRAX_DIR}/include)

    add_executable (trax_client cmt common.cpp gui.cpp trax.cpp
        CMT.cpp Consensus.cpp Fusion.cpp Matcher.cpp Tracker.cpp
        fastcluster/fastcluster.cpp withrobot_camera.cpp withrobot_utility.cpp)

    find_library(TRAX_LIBRARY NAMES trax HINTS "${TRAX_DIR}/lib")

    target_link_libraries(trax_client ${OpenCV_LIBS} ${TRAX_LIBRARY})

endif()

