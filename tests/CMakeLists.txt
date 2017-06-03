cmake_minimum_required(VERSION 2.8)
project(TFG-VISION)

#set(raspicam_DIR "/home/Escritorio/raspicam-0.1.6/build")

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS system program_options signals thread filesystem REQUIRED COMPONENTS)


#### BOOST ####
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

#find_package(raspicam REQUIRED)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lsqlite3")


if (SQLITE3_LIBRARIES AND SQLITE3_INCLUDE_DIRS)
  # in cache already
  set(SQLITE3_FOUND TRUE)
else (SQLITE3_LIBRARIES AND SQLITE3_INCLUDE_DIRS)
  # use pkg-config to get the directories and then use these values
  # in the FIND_PATH() and FIND_LIBRARY() calls
  if ( EQUAL 2 AND  EQUAL 4)
    include(UsePkgConfig)
    pkgconfig(sqlite3 _SQLITE3_INCLUDEDIR _SQLITE3_LIBDIR _SQLITE3_LDFLAGS _SQLITE3_CFLAGS)
  else ( EQUAL 2 AND  EQUAL 4)
    find_package(PkgConfig)
    if (PKG_CONFIG_FOUND)
      pkg_check_modules(_SQLITE3 sqlite3)
    endif (PKG_CONFIG_FOUND)
  endif ( EQUAL 2 AND  EQUAL 4)
  find_path(SQLITE3_INCLUDE_DIR
    NAMES
      sqlite3.h
    PATHS

      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
  )

  find_library(SQLITE3_LIBRARY
    NAMES
      sqlite3
    PATHS

      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
  )

  if (SQLITE3_LIBRARY)
    set(SQLITE3_FOUND TRUE)
  endif (SQLITE3_LIBRARY)

  set(SQLITE3_INCLUDE_DIRS

  )

  if (SQLITE3_FOUND)
    set(SQLITE3_LIBRARIES


    )
  endif (SQLITE3_FOUND)

  if (SQLITE3_INCLUDE_DIRS AND SQLITE3_LIBRARIES)
     set(SQLITE3_FOUND TRUE)
  endif (SQLITE3_INCLUDE_DIRS AND SQLITE3_LIBRARIES)

  if (SQLITE3_FOUND)
    if (NOT Sqlite3_FIND_QUIETLY)
      message(STATUS Found Sqlite3: )
    endif (NOT Sqlite3_FIND_QUIETLY)
  else (SQLITE3_FOUND)
    if (Sqlite3_FIND_REQUIRED)
      message(FATAL_ERROR No se encuentra SQLite)
    endif (Sqlite3_FIND_REQUIRED)
  endif (SQLITE3_FOUND)

  # show the SQLITE3_INCLUDE_DIRS and SQLITE3_LIBRARIES variables only in the advanced view
  mark_as_advanced(SQLITE3_INCLUDE_DIRS SQLITE3_LIBRARIES)

endif (SQLITE3_LIBRARIES AND SQLITE3_INCLUDE_DIRS)




set(THREADS_PREFER_PTHREAD_FLAG ON)
#add_executable(Captura_imagen_rgb.o captura_imagenes.cpp)

################  CAPTURA IMAGEN  #######################


#target_link_libraries(Captura_imagen_rgb.o Threads::Threads)
#target_link_libraries(Captura_imagen_rgb.o ${OpenCV_LIBS})
#target_link_libraries(Captura_imagen_rgb.o /opt/vc/lib/libmmal_core.so;/opt/vc/lib/libmmal_util.so;/opt/vc/lib/libmmal.so raspicam opencv_calib3d;opencv_core;opencv_features2d;opencv_flann;opencv_highgui;opencv_imgcodecs;opencv_imgproc;opencv_ml;opencv_objdetect;opencv_photo;opencv_shape;opencv_stitching;opencv_superres;opencv_video;opencv_videoio;opencv_videostab raspicam_cv)

################  MERGE HDR  #######################

add_executable(Merge.o MergeHDR/merge.cpp)
target_link_libraries(Merge.o Threads::Threads)
target_link_libraries(Merge.o ${Boost_PROGRAM_OPTIONS_LIBRARY})
target_link_libraries(Merge.o ${Boost_LIBRARIES})
#target_link_libraries(Merge.o libboost_program_options)
target_link_libraries(Merge.o ${OpenCV_LIBS})

################  SUN  #######################

add_executable(Sun.o Sun/sun.cpp)
target_link_libraries(Sun.o Threads::Threads)
target_link_libraries(Sun.o ${OpenCV_LIBS})

################  CLOUDS  #######################

add_executable(Clouds.o Clouds/clouds.cpp)
target_link_libraries(Clouds.o Threads::Threads)
target_link_libraries(Clouds.o ${OpenCV_LIBS})

################  HOMOGRAFIA  #######################

add_executable(Homography.o Homography/homography.cpp)
target_link_libraries(Homography.o Threads::Threads)
target_link_libraries(Homography.o ${OpenCV_LIBS})

################  FLUJO OPTICO  #######################

add_executable(Optical_flow.o OpticalFlow/optical_flow.cpp)
target_link_libraries(Optical_flow.o Threads::Threads)
target_link_libraries(Optical_flow.o ${OpenCV_LIBS})

################  TRACKING NUBES  #######################

add_executable(Tracking_cloud.o CloudTracking/cloud_tracking.cpp)
target_link_libraries(Tracking_cloud.o Threads::Threads)
target_link_libraries(Tracking_cloud.o ${OpenCV_LIBS})

################  DETECCION  #######################

add_executable(Detection.o Detection/detection.cpp)
target_link_libraries(Detection.o Threads::Threads)
target_link_libraries(Detection.o ${OpenCV_LIBS})

################  TEMPERATURE  #######################

add_executable(Temperature.o Arduino/Temperature/temperature.cpp)
target_link_libraries(Temperature.o Threads::Threads)
target_link_libraries(Temperature.o ${Boost_PROGRAM_OPTIONS_LIBRARY})
target_link_libraries(Temperature.o ${Boost_SYSTEM_LIBRARY})
target_link_libraries(Temperature.o ${OpenCV_LIBS})
target_link_libraries(Temperature.o sqlite3)
target_link_libraries(Temperature.o ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(Temperature.o PUBLIC SYSTEM ${Boost_INCLUDE_DIRS})
