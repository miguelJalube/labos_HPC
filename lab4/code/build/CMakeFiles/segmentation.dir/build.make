# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/miguel/Bureau/HPC/labos_HPC/lab4/code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/miguel/Bureau/HPC/labos_HPC/lab4/code/build

# Include any dependencies generated for this target.
include CMakeFiles/segmentation.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/segmentation.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/segmentation.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/segmentation.dir/flags.make

CMakeFiles/segmentation.dir/src/main_segmentation.c.o: CMakeFiles/segmentation.dir/flags.make
CMakeFiles/segmentation.dir/src/main_segmentation.c.o: ../src/main_segmentation.c
CMakeFiles/segmentation.dir/src/main_segmentation.c.o: CMakeFiles/segmentation.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/miguel/Bureau/HPC/labos_HPC/lab4/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/segmentation.dir/src/main_segmentation.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/segmentation.dir/src/main_segmentation.c.o -MF CMakeFiles/segmentation.dir/src/main_segmentation.c.o.d -o CMakeFiles/segmentation.dir/src/main_segmentation.c.o -c /home/miguel/Bureau/HPC/labos_HPC/lab4/code/src/main_segmentation.c

CMakeFiles/segmentation.dir/src/main_segmentation.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/segmentation.dir/src/main_segmentation.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/miguel/Bureau/HPC/labos_HPC/lab4/code/src/main_segmentation.c > CMakeFiles/segmentation.dir/src/main_segmentation.c.i

CMakeFiles/segmentation.dir/src/main_segmentation.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/segmentation.dir/src/main_segmentation.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/miguel/Bureau/HPC/labos_HPC/lab4/code/src/main_segmentation.c -o CMakeFiles/segmentation.dir/src/main_segmentation.c.s

CMakeFiles/segmentation.dir/src/k-means.c.o: CMakeFiles/segmentation.dir/flags.make
CMakeFiles/segmentation.dir/src/k-means.c.o: ../src/k-means.c
CMakeFiles/segmentation.dir/src/k-means.c.o: CMakeFiles/segmentation.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/miguel/Bureau/HPC/labos_HPC/lab4/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/segmentation.dir/src/k-means.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/segmentation.dir/src/k-means.c.o -MF CMakeFiles/segmentation.dir/src/k-means.c.o.d -o CMakeFiles/segmentation.dir/src/k-means.c.o -c /home/miguel/Bureau/HPC/labos_HPC/lab4/code/src/k-means.c

CMakeFiles/segmentation.dir/src/k-means.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/segmentation.dir/src/k-means.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/miguel/Bureau/HPC/labos_HPC/lab4/code/src/k-means.c > CMakeFiles/segmentation.dir/src/k-means.c.i

CMakeFiles/segmentation.dir/src/k-means.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/segmentation.dir/src/k-means.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/miguel/Bureau/HPC/labos_HPC/lab4/code/src/k-means.c -o CMakeFiles/segmentation.dir/src/k-means.c.s

CMakeFiles/segmentation.dir/src/image.c.o: CMakeFiles/segmentation.dir/flags.make
CMakeFiles/segmentation.dir/src/image.c.o: ../src/image.c
CMakeFiles/segmentation.dir/src/image.c.o: CMakeFiles/segmentation.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/miguel/Bureau/HPC/labos_HPC/lab4/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/segmentation.dir/src/image.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/segmentation.dir/src/image.c.o -MF CMakeFiles/segmentation.dir/src/image.c.o.d -o CMakeFiles/segmentation.dir/src/image.c.o -c /home/miguel/Bureau/HPC/labos_HPC/lab4/code/src/image.c

CMakeFiles/segmentation.dir/src/image.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/segmentation.dir/src/image.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/miguel/Bureau/HPC/labos_HPC/lab4/code/src/image.c > CMakeFiles/segmentation.dir/src/image.c.i

CMakeFiles/segmentation.dir/src/image.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/segmentation.dir/src/image.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/miguel/Bureau/HPC/labos_HPC/lab4/code/src/image.c -o CMakeFiles/segmentation.dir/src/image.c.s

# Object files for target segmentation
segmentation_OBJECTS = \
"CMakeFiles/segmentation.dir/src/main_segmentation.c.o" \
"CMakeFiles/segmentation.dir/src/k-means.c.o" \
"CMakeFiles/segmentation.dir/src/image.c.o"

# External object files for target segmentation
segmentation_EXTERNAL_OBJECTS =

segmentation: CMakeFiles/segmentation.dir/src/main_segmentation.c.o
segmentation: CMakeFiles/segmentation.dir/src/k-means.c.o
segmentation: CMakeFiles/segmentation.dir/src/image.c.o
segmentation: CMakeFiles/segmentation.dir/build.make
segmentation: CMakeFiles/segmentation.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/miguel/Bureau/HPC/labos_HPC/lab4/code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable segmentation"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/segmentation.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/segmentation.dir/build: segmentation
.PHONY : CMakeFiles/segmentation.dir/build

CMakeFiles/segmentation.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/segmentation.dir/cmake_clean.cmake
.PHONY : CMakeFiles/segmentation.dir/clean

CMakeFiles/segmentation.dir/depend:
	cd /home/miguel/Bureau/HPC/labos_HPC/lab4/code/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/miguel/Bureau/HPC/labos_HPC/lab4/code /home/miguel/Bureau/HPC/labos_HPC/lab4/code /home/miguel/Bureau/HPC/labos_HPC/lab4/code/build /home/miguel/Bureau/HPC/labos_HPC/lab4/code/build /home/miguel/Bureau/HPC/labos_HPC/lab4/code/build/CMakeFiles/segmentation.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/segmentation.dir/depend

