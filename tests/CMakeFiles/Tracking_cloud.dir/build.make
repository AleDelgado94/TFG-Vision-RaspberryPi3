# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/aledelgado/TFG-Vision-RaspberryPi3/tests

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aledelgado/TFG-Vision-RaspberryPi3/tests

# Include any dependencies generated for this target.
include CMakeFiles/Tracking_cloud.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Tracking_cloud.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Tracking_cloud.dir/flags.make

CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.o: CMakeFiles/Tracking_cloud.dir/flags.make
CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.o: cloud_tracking.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aledelgado/TFG-Vision-RaspberryPi3/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.o -c /home/aledelgado/TFG-Vision-RaspberryPi3/tests/cloud_tracking.cpp

CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aledelgado/TFG-Vision-RaspberryPi3/tests/cloud_tracking.cpp > CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.i

CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aledelgado/TFG-Vision-RaspberryPi3/tests/cloud_tracking.cpp -o CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.s

CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.o.requires:

.PHONY : CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.o.requires

CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.o.provides: CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.o.requires
	$(MAKE) -f CMakeFiles/Tracking_cloud.dir/build.make CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.o.provides.build
.PHONY : CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.o.provides

CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.o.provides.build: CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.o


# Object files for target Tracking_cloud
Tracking_cloud_OBJECTS = \
"CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.o"

# External object files for target Tracking_cloud
Tracking_cloud_EXTERNAL_OBJECTS =

Tracking_cloud: CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.o
Tracking_cloud: CMakeFiles/Tracking_cloud.dir/build.make
Tracking_cloud: /usr/local/lib/libopencv_stitching.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_superres.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_videostab.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_aruco.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_bgsegm.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_bioinspired.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_ccalib.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_dpm.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_freetype.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_fuzzy.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_line_descriptor.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_optflow.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_reg.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_saliency.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_stereo.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_structured_light.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_surface_matching.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_tracking.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_xfeatures2d.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_ximgproc.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_xobjdetect.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_xphoto.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_shape.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_phase_unwrapping.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_rgbd.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_calib3d.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_video.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_datasets.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_dnn.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_face.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_plot.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_text.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_features2d.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_flann.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_objdetect.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_ml.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_highgui.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_photo.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_videoio.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_imgcodecs.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_imgproc.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_core.so.3.2.0
Tracking_cloud: /usr/local/lib/libopencv_cudev.so.3.2.0
Tracking_cloud: CMakeFiles/Tracking_cloud.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/aledelgado/TFG-Vision-RaspberryPi3/tests/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Tracking_cloud"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Tracking_cloud.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Tracking_cloud.dir/build: Tracking_cloud

.PHONY : CMakeFiles/Tracking_cloud.dir/build

CMakeFiles/Tracking_cloud.dir/requires: CMakeFiles/Tracking_cloud.dir/cloud_tracking.cpp.o.requires

.PHONY : CMakeFiles/Tracking_cloud.dir/requires

CMakeFiles/Tracking_cloud.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Tracking_cloud.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Tracking_cloud.dir/clean

CMakeFiles/Tracking_cloud.dir/depend:
	cd /home/aledelgado/TFG-Vision-RaspberryPi3/tests && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aledelgado/TFG-Vision-RaspberryPi3/tests /home/aledelgado/TFG-Vision-RaspberryPi3/tests /home/aledelgado/TFG-Vision-RaspberryPi3/tests /home/aledelgado/TFG-Vision-RaspberryPi3/tests /home/aledelgado/TFG-Vision-RaspberryPi3/tests/CMakeFiles/Tracking_cloud.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Tracking_cloud.dir/depend

