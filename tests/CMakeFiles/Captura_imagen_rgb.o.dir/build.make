# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/Desktop/TFG-Vision-RaspberryPi3/tests

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/Desktop/TFG-Vision-RaspberryPi3/tests

# Include any dependencies generated for this target.
include CMakeFiles/Captura_imagen_rgb.o.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Captura_imagen_rgb.o.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Captura_imagen_rgb.o.dir/flags.make

CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.o: CMakeFiles/Captura_imagen_rgb.o.dir/flags.make
CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.o: Capture/captura_imagenes.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Desktop/TFG-Vision-RaspberryPi3/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.o -c /home/pi/Desktop/TFG-Vision-RaspberryPi3/tests/Capture/captura_imagenes.cpp

CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/Desktop/TFG-Vision-RaspberryPi3/tests/Capture/captura_imagenes.cpp > CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.i

CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/Desktop/TFG-Vision-RaspberryPi3/tests/Capture/captura_imagenes.cpp -o CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.s

CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.o.requires:

.PHONY : CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.o.requires

CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.o.provides: CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.o.requires
	$(MAKE) -f CMakeFiles/Captura_imagen_rgb.o.dir/build.make CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.o.provides.build
.PHONY : CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.o.provides

CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.o.provides.build: CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.o


# Object files for target Captura_imagen_rgb.o
Captura_imagen_rgb_o_OBJECTS = \
"CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.o"

# External object files for target Captura_imagen_rgb.o
Captura_imagen_rgb_o_EXTERNAL_OBJECTS =

Captura_imagen_rgb.o: CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.o
Captura_imagen_rgb.o: CMakeFiles/Captura_imagen_rgb.o.dir/build.make
Captura_imagen_rgb.o: /usr/local/lib/libopencv_aruco.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_bgsegm.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_bioinspired.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_ccalib.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_dpm.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_freetype.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_fuzzy.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_line_descriptor.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_optflow.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_reg.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_saliency.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_stereo.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_structured_light.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_surface_matching.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_tracking.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_ximgproc.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_xobjdetect.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_xphoto.so.3.2.0
Captura_imagen_rgb.o: /opt/vc/lib/libmmal_core.so
Captura_imagen_rgb.o: /opt/vc/lib/libmmal_util.so
Captura_imagen_rgb.o: /opt/vc/lib/libmmal.so
Captura_imagen_rgb.o: /usr/local/lib/libopencv_stitching.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_superres.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_videostab.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_xfeatures2d.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_shape.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_phase_unwrapping.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_rgbd.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_calib3d.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_video.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_datasets.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_dnn.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_face.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_plot.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_text.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_features2d.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_flann.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_objdetect.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_ml.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_highgui.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_photo.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_videoio.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_imgcodecs.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_imgproc.so.3.2.0
Captura_imagen_rgb.o: /usr/local/lib/libopencv_core.so.3.2.0
Captura_imagen_rgb.o: CMakeFiles/Captura_imagen_rgb.o.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/Desktop/TFG-Vision-RaspberryPi3/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Captura_imagen_rgb.o"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Captura_imagen_rgb.o.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Captura_imagen_rgb.o.dir/build: Captura_imagen_rgb.o

.PHONY : CMakeFiles/Captura_imagen_rgb.o.dir/build

CMakeFiles/Captura_imagen_rgb.o.dir/requires: CMakeFiles/Captura_imagen_rgb.o.dir/Capture/captura_imagenes.cpp.o.requires

.PHONY : CMakeFiles/Captura_imagen_rgb.o.dir/requires

CMakeFiles/Captura_imagen_rgb.o.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Captura_imagen_rgb.o.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Captura_imagen_rgb.o.dir/clean

CMakeFiles/Captura_imagen_rgb.o.dir/depend:
	cd /home/pi/Desktop/TFG-Vision-RaspberryPi3/tests && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/Desktop/TFG-Vision-RaspberryPi3/tests /home/pi/Desktop/TFG-Vision-RaspberryPi3/tests /home/pi/Desktop/TFG-Vision-RaspberryPi3/tests /home/pi/Desktop/TFG-Vision-RaspberryPi3/tests /home/pi/Desktop/TFG-Vision-RaspberryPi3/tests/CMakeFiles/Captura_imagen_rgb.o.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Captura_imagen_rgb.o.dir/depend

