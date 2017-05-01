cmake_minimum_required(VERSION 2.8)
project(TFG-VISION)

#set(raspicam_DIR "/home/Escritorio/raspicam-0.1.6/build")

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
#find_package(raspicam REQUIRED)
set (CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)
#add_executable(Captura_imagen_rgb captura_imagenes.cpp)
add_executable(Merge merge.cpp)
#target_link_libraries(Captura_imagen_rgb Threads::Threads)
#target_link_libraries(Captura_imagen_rgb ${OpenCV_LIBS})
#target_link_libraries(Captura_imagen_rgb /opt/vc/lib/libmmal_core.so;/opt/vc/lib/libmmal_util.so;/opt/vc/lib/libmmal.so raspicam opencv_calib3d;opencv_core;opencv_features2d;opencv_flann;opencv_highgui;opencv_imgcodecs;opencv_imgproc;opencv_ml;opencv_objdetect;opencv_photo;opencv_shape;opencv_stitching;opencv_superres;opencv_video;opencv_videoio;opencv_videostab raspicam_cv)

target_link_libraries(Merge Threads::Threads)
target_link_libraries(Merge ${OpenCV_LIBS})
#target_link_libraries(Merge /opt/vc/lib/libmmal_core.so;/opt/vc/lib/libmmal_util.so;/opt/vc/lib/libmmal.so raspicam opencv_calib3d;opencv_core;opencv_features2d;opencv_flann;opencv_highgui;opencv_imgcodecs;opencv_imgproc;opencv_ml;opencv_objdetect;opencv_photo;opencv_shape;opencv_stitching;opencv_superres;opencv_video;opencv_videoio;opencv_videostab raspicam_cv)

add_executable(Sun sun.cpp)
target_link_libraries(Sun Threads::Threads)
target_link_libraries(Sun ${OpenCV_LIBS})

add_executable(Clouds clouds.cpp)
target_link_libraries(Clouds Threads::Threads)
target_link_libraries(Clouds ${OpenCV_LIBS})
