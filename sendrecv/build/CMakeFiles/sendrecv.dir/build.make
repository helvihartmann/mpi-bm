# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hartmann/benchmark/sendrecv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hartmann/benchmark/sendrecv/build

# Include any dependencies generated for this target.
include CMakeFiles/sendrecv.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sendrecv.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sendrecv.dir/flags.make

CMakeFiles/sendrecv.dir/sendrecv.cpp.o: CMakeFiles/sendrecv.dir/flags.make
CMakeFiles/sendrecv.dir/sendrecv.cpp.o: ../sendrecv.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/hartmann/benchmark/sendrecv/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/sendrecv.dir/sendrecv.cpp.o"
	mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/sendrecv.dir/sendrecv.cpp.o -c /home/hartmann/benchmark/sendrecv/sendrecv.cpp

CMakeFiles/sendrecv.dir/sendrecv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sendrecv.dir/sendrecv.cpp.i"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/hartmann/benchmark/sendrecv/sendrecv.cpp > CMakeFiles/sendrecv.dir/sendrecv.cpp.i

CMakeFiles/sendrecv.dir/sendrecv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sendrecv.dir/sendrecv.cpp.s"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/hartmann/benchmark/sendrecv/sendrecv.cpp -o CMakeFiles/sendrecv.dir/sendrecv.cpp.s

CMakeFiles/sendrecv.dir/sendrecv.cpp.o.requires:
.PHONY : CMakeFiles/sendrecv.dir/sendrecv.cpp.o.requires

CMakeFiles/sendrecv.dir/sendrecv.cpp.o.provides: CMakeFiles/sendrecv.dir/sendrecv.cpp.o.requires
	$(MAKE) -f CMakeFiles/sendrecv.dir/build.make CMakeFiles/sendrecv.dir/sendrecv.cpp.o.provides.build
.PHONY : CMakeFiles/sendrecv.dir/sendrecv.cpp.o.provides

CMakeFiles/sendrecv.dir/sendrecv.cpp.o.provides.build: CMakeFiles/sendrecv.dir/sendrecv.cpp.o

CMakeFiles/sendrecv.dir/bufferoperations.cpp.o: CMakeFiles/sendrecv.dir/flags.make
CMakeFiles/sendrecv.dir/bufferoperations.cpp.o: ../bufferoperations.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/hartmann/benchmark/sendrecv/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/sendrecv.dir/bufferoperations.cpp.o"
	mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/sendrecv.dir/bufferoperations.cpp.o -c /home/hartmann/benchmark/sendrecv/bufferoperations.cpp

CMakeFiles/sendrecv.dir/bufferoperations.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sendrecv.dir/bufferoperations.cpp.i"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/hartmann/benchmark/sendrecv/bufferoperations.cpp > CMakeFiles/sendrecv.dir/bufferoperations.cpp.i

CMakeFiles/sendrecv.dir/bufferoperations.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sendrecv.dir/bufferoperations.cpp.s"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/hartmann/benchmark/sendrecv/bufferoperations.cpp -o CMakeFiles/sendrecv.dir/bufferoperations.cpp.s

CMakeFiles/sendrecv.dir/bufferoperations.cpp.o.requires:
.PHONY : CMakeFiles/sendrecv.dir/bufferoperations.cpp.o.requires

CMakeFiles/sendrecv.dir/bufferoperations.cpp.o.provides: CMakeFiles/sendrecv.dir/bufferoperations.cpp.o.requires
	$(MAKE) -f CMakeFiles/sendrecv.dir/build.make CMakeFiles/sendrecv.dir/bufferoperations.cpp.o.provides.build
.PHONY : CMakeFiles/sendrecv.dir/bufferoperations.cpp.o.provides

CMakeFiles/sendrecv.dir/bufferoperations.cpp.o.provides.build: CMakeFiles/sendrecv.dir/bufferoperations.cpp.o

CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.o: CMakeFiles/sendrecv.dir/flags.make
CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.o: ../totaldatasendcalc.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/hartmann/benchmark/sendrecv/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.o"
	mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.o -c /home/hartmann/benchmark/sendrecv/totaldatasendcalc.cpp

CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.i"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/hartmann/benchmark/sendrecv/totaldatasendcalc.cpp > CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.i

CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.s"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/hartmann/benchmark/sendrecv/totaldatasendcalc.cpp -o CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.s

CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.o.requires:
.PHONY : CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.o.requires

CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.o.provides: CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.o.requires
	$(MAKE) -f CMakeFiles/sendrecv.dir/build.make CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.o.provides.build
.PHONY : CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.o.provides

CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.o.provides.build: CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.o

# Object files for target sendrecv
sendrecv_OBJECTS = \
"CMakeFiles/sendrecv.dir/sendrecv.cpp.o" \
"CMakeFiles/sendrecv.dir/bufferoperations.cpp.o" \
"CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.o"

# External object files for target sendrecv
sendrecv_EXTERNAL_OBJECTS =

sendrecv: CMakeFiles/sendrecv.dir/sendrecv.cpp.o
sendrecv: CMakeFiles/sendrecv.dir/bufferoperations.cpp.o
sendrecv: CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.o
sendrecv: CMakeFiles/sendrecv.dir/build.make
sendrecv: CMakeFiles/sendrecv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable sendrecv"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sendrecv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sendrecv.dir/build: sendrecv
.PHONY : CMakeFiles/sendrecv.dir/build

CMakeFiles/sendrecv.dir/requires: CMakeFiles/sendrecv.dir/sendrecv.cpp.o.requires
CMakeFiles/sendrecv.dir/requires: CMakeFiles/sendrecv.dir/bufferoperations.cpp.o.requires
CMakeFiles/sendrecv.dir/requires: CMakeFiles/sendrecv.dir/totaldatasendcalc.cpp.o.requires
.PHONY : CMakeFiles/sendrecv.dir/requires

CMakeFiles/sendrecv.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sendrecv.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sendrecv.dir/clean

CMakeFiles/sendrecv.dir/depend:
	cd /home/hartmann/benchmark/sendrecv/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hartmann/benchmark/sendrecv /home/hartmann/benchmark/sendrecv /home/hartmann/benchmark/sendrecv/build /home/hartmann/benchmark/sendrecv/build /home/hartmann/benchmark/sendrecv/build/CMakeFiles/sendrecv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sendrecv.dir/depend

