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
include CMakeFiles/pingpong.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/pingpong.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pingpong.dir/flags.make

CMakeFiles/pingpong.dir/pingpong.cpp.o: CMakeFiles/pingpong.dir/flags.make
CMakeFiles/pingpong.dir/pingpong.cpp.o: ../pingpong.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/hartmann/benchmark/sendrecv/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/pingpong.dir/pingpong.cpp.o"
	mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/pingpong.dir/pingpong.cpp.o -c /home/hartmann/benchmark/sendrecv/pingpong.cpp

CMakeFiles/pingpong.dir/pingpong.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pingpong.dir/pingpong.cpp.i"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/hartmann/benchmark/sendrecv/pingpong.cpp > CMakeFiles/pingpong.dir/pingpong.cpp.i

CMakeFiles/pingpong.dir/pingpong.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pingpong.dir/pingpong.cpp.s"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/hartmann/benchmark/sendrecv/pingpong.cpp -o CMakeFiles/pingpong.dir/pingpong.cpp.s

CMakeFiles/pingpong.dir/pingpong.cpp.o.requires:
.PHONY : CMakeFiles/pingpong.dir/pingpong.cpp.o.requires

CMakeFiles/pingpong.dir/pingpong.cpp.o.provides: CMakeFiles/pingpong.dir/pingpong.cpp.o.requires
	$(MAKE) -f CMakeFiles/pingpong.dir/build.make CMakeFiles/pingpong.dir/pingpong.cpp.o.provides.build
.PHONY : CMakeFiles/pingpong.dir/pingpong.cpp.o.provides

CMakeFiles/pingpong.dir/pingpong.cpp.o.provides.build: CMakeFiles/pingpong.dir/pingpong.cpp.o

CMakeFiles/pingpong.dir/buffer.cpp.o: CMakeFiles/pingpong.dir/flags.make
CMakeFiles/pingpong.dir/buffer.cpp.o: ../buffer.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/hartmann/benchmark/sendrecv/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/pingpong.dir/buffer.cpp.o"
	mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/pingpong.dir/buffer.cpp.o -c /home/hartmann/benchmark/sendrecv/buffer.cpp

CMakeFiles/pingpong.dir/buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pingpong.dir/buffer.cpp.i"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/hartmann/benchmark/sendrecv/buffer.cpp > CMakeFiles/pingpong.dir/buffer.cpp.i

CMakeFiles/pingpong.dir/buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pingpong.dir/buffer.cpp.s"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/hartmann/benchmark/sendrecv/buffer.cpp -o CMakeFiles/pingpong.dir/buffer.cpp.s

CMakeFiles/pingpong.dir/buffer.cpp.o.requires:
.PHONY : CMakeFiles/pingpong.dir/buffer.cpp.o.requires

CMakeFiles/pingpong.dir/buffer.cpp.o.provides: CMakeFiles/pingpong.dir/buffer.cpp.o.requires
	$(MAKE) -f CMakeFiles/pingpong.dir/build.make CMakeFiles/pingpong.dir/buffer.cpp.o.provides.build
.PHONY : CMakeFiles/pingpong.dir/buffer.cpp.o.provides

CMakeFiles/pingpong.dir/buffer.cpp.o.provides.build: CMakeFiles/pingpong.dir/buffer.cpp.o

CMakeFiles/pingpong.dir/classmpi3.cpp.o: CMakeFiles/pingpong.dir/flags.make
CMakeFiles/pingpong.dir/classmpi3.cpp.o: ../classmpi3.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/hartmann/benchmark/sendrecv/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/pingpong.dir/classmpi3.cpp.o"
	mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/pingpong.dir/classmpi3.cpp.o -c /home/hartmann/benchmark/sendrecv/classmpi3.cpp

CMakeFiles/pingpong.dir/classmpi3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pingpong.dir/classmpi3.cpp.i"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/hartmann/benchmark/sendrecv/classmpi3.cpp > CMakeFiles/pingpong.dir/classmpi3.cpp.i

CMakeFiles/pingpong.dir/classmpi3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pingpong.dir/classmpi3.cpp.s"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/hartmann/benchmark/sendrecv/classmpi3.cpp -o CMakeFiles/pingpong.dir/classmpi3.cpp.s

CMakeFiles/pingpong.dir/classmpi3.cpp.o.requires:
.PHONY : CMakeFiles/pingpong.dir/classmpi3.cpp.o.requires

CMakeFiles/pingpong.dir/classmpi3.cpp.o.provides: CMakeFiles/pingpong.dir/classmpi3.cpp.o.requires
	$(MAKE) -f CMakeFiles/pingpong.dir/build.make CMakeFiles/pingpong.dir/classmpi3.cpp.o.provides.build
.PHONY : CMakeFiles/pingpong.dir/classmpi3.cpp.o.provides

CMakeFiles/pingpong.dir/classmpi3.cpp.o.provides.build: CMakeFiles/pingpong.dir/classmpi3.cpp.o

CMakeFiles/pingpong.dir/print.cpp.o: CMakeFiles/pingpong.dir/flags.make
CMakeFiles/pingpong.dir/print.cpp.o: ../print.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/hartmann/benchmark/sendrecv/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/pingpong.dir/print.cpp.o"
	mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/pingpong.dir/print.cpp.o -c /home/hartmann/benchmark/sendrecv/print.cpp

CMakeFiles/pingpong.dir/print.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pingpong.dir/print.cpp.i"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/hartmann/benchmark/sendrecv/print.cpp > CMakeFiles/pingpong.dir/print.cpp.i

CMakeFiles/pingpong.dir/print.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pingpong.dir/print.cpp.s"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/hartmann/benchmark/sendrecv/print.cpp -o CMakeFiles/pingpong.dir/print.cpp.s

CMakeFiles/pingpong.dir/print.cpp.o.requires:
.PHONY : CMakeFiles/pingpong.dir/print.cpp.o.requires

CMakeFiles/pingpong.dir/print.cpp.o.provides: CMakeFiles/pingpong.dir/print.cpp.o.requires
	$(MAKE) -f CMakeFiles/pingpong.dir/build.make CMakeFiles/pingpong.dir/print.cpp.o.provides.build
.PHONY : CMakeFiles/pingpong.dir/print.cpp.o.provides

CMakeFiles/pingpong.dir/print.cpp.o.provides.build: CMakeFiles/pingpong.dir/print.cpp.o

CMakeFiles/pingpong.dir/parameters.cpp.o: CMakeFiles/pingpong.dir/flags.make
CMakeFiles/pingpong.dir/parameters.cpp.o: ../parameters.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/hartmann/benchmark/sendrecv/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/pingpong.dir/parameters.cpp.o"
	mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/pingpong.dir/parameters.cpp.o -c /home/hartmann/benchmark/sendrecv/parameters.cpp

CMakeFiles/pingpong.dir/parameters.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pingpong.dir/parameters.cpp.i"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/hartmann/benchmark/sendrecv/parameters.cpp > CMakeFiles/pingpong.dir/parameters.cpp.i

CMakeFiles/pingpong.dir/parameters.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pingpong.dir/parameters.cpp.s"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/hartmann/benchmark/sendrecv/parameters.cpp -o CMakeFiles/pingpong.dir/parameters.cpp.s

CMakeFiles/pingpong.dir/parameters.cpp.o.requires:
.PHONY : CMakeFiles/pingpong.dir/parameters.cpp.o.requires

CMakeFiles/pingpong.dir/parameters.cpp.o.provides: CMakeFiles/pingpong.dir/parameters.cpp.o.requires
	$(MAKE) -f CMakeFiles/pingpong.dir/build.make CMakeFiles/pingpong.dir/parameters.cpp.o.provides.build
.PHONY : CMakeFiles/pingpong.dir/parameters.cpp.o.provides

CMakeFiles/pingpong.dir/parameters.cpp.o.provides.build: CMakeFiles/pingpong.dir/parameters.cpp.o

CMakeFiles/pingpong.dir/results.cpp.o: CMakeFiles/pingpong.dir/flags.make
CMakeFiles/pingpong.dir/results.cpp.o: ../results.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/hartmann/benchmark/sendrecv/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/pingpong.dir/results.cpp.o"
	mpic++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/pingpong.dir/results.cpp.o -c /home/hartmann/benchmark/sendrecv/results.cpp

CMakeFiles/pingpong.dir/results.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pingpong.dir/results.cpp.i"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/hartmann/benchmark/sendrecv/results.cpp > CMakeFiles/pingpong.dir/results.cpp.i

CMakeFiles/pingpong.dir/results.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pingpong.dir/results.cpp.s"
	mpic++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/hartmann/benchmark/sendrecv/results.cpp -o CMakeFiles/pingpong.dir/results.cpp.s

CMakeFiles/pingpong.dir/results.cpp.o.requires:
.PHONY : CMakeFiles/pingpong.dir/results.cpp.o.requires

CMakeFiles/pingpong.dir/results.cpp.o.provides: CMakeFiles/pingpong.dir/results.cpp.o.requires
	$(MAKE) -f CMakeFiles/pingpong.dir/build.make CMakeFiles/pingpong.dir/results.cpp.o.provides.build
.PHONY : CMakeFiles/pingpong.dir/results.cpp.o.provides

CMakeFiles/pingpong.dir/results.cpp.o.provides.build: CMakeFiles/pingpong.dir/results.cpp.o

# Object files for target pingpong
pingpong_OBJECTS = \
"CMakeFiles/pingpong.dir/pingpong.cpp.o" \
"CMakeFiles/pingpong.dir/buffer.cpp.o" \
"CMakeFiles/pingpong.dir/classmpi3.cpp.o" \
"CMakeFiles/pingpong.dir/print.cpp.o" \
"CMakeFiles/pingpong.dir/parameters.cpp.o" \
"CMakeFiles/pingpong.dir/results.cpp.o"

# External object files for target pingpong
pingpong_EXTERNAL_OBJECTS =

pingpong: CMakeFiles/pingpong.dir/pingpong.cpp.o
pingpong: CMakeFiles/pingpong.dir/buffer.cpp.o
pingpong: CMakeFiles/pingpong.dir/classmpi3.cpp.o
pingpong: CMakeFiles/pingpong.dir/print.cpp.o
pingpong: CMakeFiles/pingpong.dir/parameters.cpp.o
pingpong: CMakeFiles/pingpong.dir/results.cpp.o
pingpong: CMakeFiles/pingpong.dir/build.make
pingpong: CMakeFiles/pingpong.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable pingpong"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pingpong.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pingpong.dir/build: pingpong
.PHONY : CMakeFiles/pingpong.dir/build

CMakeFiles/pingpong.dir/requires: CMakeFiles/pingpong.dir/pingpong.cpp.o.requires
CMakeFiles/pingpong.dir/requires: CMakeFiles/pingpong.dir/buffer.cpp.o.requires
CMakeFiles/pingpong.dir/requires: CMakeFiles/pingpong.dir/classmpi3.cpp.o.requires
CMakeFiles/pingpong.dir/requires: CMakeFiles/pingpong.dir/print.cpp.o.requires
CMakeFiles/pingpong.dir/requires: CMakeFiles/pingpong.dir/parameters.cpp.o.requires
CMakeFiles/pingpong.dir/requires: CMakeFiles/pingpong.dir/results.cpp.o.requires
.PHONY : CMakeFiles/pingpong.dir/requires

CMakeFiles/pingpong.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pingpong.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pingpong.dir/clean

CMakeFiles/pingpong.dir/depend:
	cd /home/hartmann/benchmark/sendrecv/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hartmann/benchmark/sendrecv /home/hartmann/benchmark/sendrecv /home/hartmann/benchmark/sendrecv/build /home/hartmann/benchmark/sendrecv/build /home/hartmann/benchmark/sendrecv/build/CMakeFiles/pingpong.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pingpong.dir/depend

