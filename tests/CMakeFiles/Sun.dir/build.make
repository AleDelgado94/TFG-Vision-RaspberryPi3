# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_SOURCE_DIR = /home/aledelgado/tfg

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aledelgado/tfg

# Include any dependencies generated for this target.
include CMakeFiles/Sun.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Sun.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Sun.dir/flags.make

CMakeFiles/Sun.dir/sun.cpp.o: CMakeFiles/Sun.dir/flags.make
CMakeFiles/Sun.dir/sun.cpp.o: sun.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aledelgado/tfg/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Sun.dir/sun.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Sun.dir/sun.cpp.o -c /home/aledelgado/tfg/sun.cpp

CMakeFiles/Sun.dir/sun.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Sun.dir/sun.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aledelgado/tfg/sun.cpp > CMakeFiles/Sun.dir/sun.cpp.i

CMakeFiles/Sun.dir/sun.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Sun.dir/sun.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aledelgado/tfg/sun.cpp -o CMakeFiles/Sun.dir/sun.cpp.s

CMakeFiles/Sun.dir/sun.cpp.o.requires:

.PHONY : CMakeFiles/Sun.dir/sun.cpp.o.requires

CMakeFiles/Sun.dir/sun.cpp.o.provides: CMakeFiles/Sun.dir/sun.cpp.o.requires
	$(MAKE) -f CMakeFiles/Sun.dir/build.make CMakeFiles/Sun.dir/sun.cpp.o.provides.build
.PHONY : CMakeFiles/Sun.dir/sun.cpp.o.provides

CMakeFiles/Sun.dir/sun.cpp.o.provides.build: CMakeFiles/Sun.dir/sun.cpp.o


# Object files for target Sun
Sun_OBJECTS = \
"CMakeFiles/Sun.dir/sun.cpp.o"

# External object files for target Sun
Sun_EXTERNAL_OBJECTS =

Sun: CMakeFiles/Sun.dir/sun.cpp.o
Sun: CMakeFiles/Sun.dir/build.make
Sun: /usr/local/lib/libopencv_stitching.so.3.2.0
Sun: /usr/local/lib/libopencv_superres.so.3.2.0
Sun: /usr/local/lib/libopencv_videostab.so.3.2.0
Sun: /usr/local/lib/libopencv_aruco.so.3.2.0
Sun: /usr/local/lib/libopencv_bgsegm.so.3.2.0
Sun: /usr/local/lib/libopencv_bioinspired.so.3.2.0
Sun: /usr/local/lib/libopencv_ccalib.so.3.2.0
Sun: /usr/local/lib/libopencv_dpm.so.3.2.0
Sun: /usr/local/lib/libopencv_freetype.so.3.2.0
Sun: /usr/local/lib/libopencv_fuzzy.so.3.2.0
Sun: /usr/local/lib/libopencv_line_descriptor.so.3.2.0
Sun: /usr/local/lib/libopencv_optflow.so.3.2.0
Sun: /usr/local/lib/libopencv_reg.so.3.2.0
Sun: /usr/local/lib/libopencv_saliency.so.3.2.0
Sun: /usr/local/lib/libopencv_stereo.so.3.2.0
Sun: /usr/local/lib/libopencv_structured_light.so.3.2.0
Sun: /usr/local/lib/libopencv_surface_matching.so.3.2.0
Sun: /usr/local/lib/libopencv_tracking.so.3.2.0
Sun: /usr/local/lib/libopencv_xfeatures2d.so.3.2.0
Sun: /usr/local/lib/libopencv_ximgproc.so.3.2.0
Sun: /usr/local/lib/libopencv_xobjdetect.so.3.2.0
Sun: /usr/local/lib/libopencv_xphoto.so.3.2.0
Sun: /usr/local/lib/libopencv_shape.so.3.2.0
Sun: /usr/local/lib/libopencv_phase_unwrapping.so.3.2.0
Sun: /usr/local/lib/libopencv_rgbd.so.3.2.0
Sun: /usr/local/lib/libopencv_calib3d.so.3.2.0
Sun: /usr/local/lib/libopencv_video.so.3.2.0
Sun: /usr/local/lib/libopencv_datasets.so.3.2.0
Sun: /usr/local/lib/libopencv_dnn.so.3.2.0
Sun: /usr/local/lib/libopencv_face.so.3.2.0
Sun: /usr/local/lib/libopencv_plot.so.3.2.0
Sun: /usr/local/lib/libopencv_text.so.3.2.0
Sun: /usr/local/lib/libopencv_features2d.so.3.2.0
Sun: /usr/local/lib/libopencv_flann.so.3.2.0
Sun: /usr/local/lib/libopencv_objdetect.so.3.2.0
Sun: /usr/local/lib/libopencv_ml.so.3.2.0
Sun: /usr/local/lib/libopencv_highgui.so.3.2.0
Sun: /usr/local/lib/libopencv_photo.so.3.2.0
Sun: /usr/local/lib/libopencv_videoio.so.3.2.0
Sun: /usr/local/lib/libopencv_imgcodecs.so.3.2.0
Sun: /usr/local/lib/libopencv_imgproc.so.3.2.0
Sun: /usr/local/lib/libopencv_core.so.3.2.0
Sun: CMakeFiles/Sun.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/aledelgado/tfg/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Sun"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Sun.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Sun.dir/build: Sun

.PHONY : CMakeFiles/Sun.dir/build

CMakeFiles/Sun.dir/requires: CMakeFiles/Sun.dir/sun.cpp.o.requires

.PHONY : CMakeFiles/Sun.dir/requires

CMakeFiles/Sun.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Sun.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Sun.dir/clean

CMakeFiles/Sun.dir/depend:
	cd /home/aledelgado/tfg && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aledelgado/tfg /home/aledelgado/tfg /home/aledelgado/tfg /home/aledelgado/tfg /home/aledelgado/tfg/CMakeFiles/Sun.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Sun.dir/depend

