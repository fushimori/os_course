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
CMAKE_SOURCE_DIR = /home/maksim/course2/sem3/os/lab1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maksim/course2/sem3/os/lab1/build

# Include any dependencies generated for this target.
include CMakeFiles/flread.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/flread.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/flread.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/flread.dir/flags.make

CMakeFiles/flread.dir/src/filereader.cpp.o: CMakeFiles/flread.dir/flags.make
CMakeFiles/flread.dir/src/filereader.cpp.o: ../src/filereader.cpp
CMakeFiles/flread.dir/src/filereader.cpp.o: CMakeFiles/flread.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maksim/course2/sem3/os/lab1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/flread.dir/src/filereader.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/flread.dir/src/filereader.cpp.o -MF CMakeFiles/flread.dir/src/filereader.cpp.o.d -o CMakeFiles/flread.dir/src/filereader.cpp.o -c /home/maksim/course2/sem3/os/lab1/src/filereader.cpp

CMakeFiles/flread.dir/src/filereader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/flread.dir/src/filereader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maksim/course2/sem3/os/lab1/src/filereader.cpp > CMakeFiles/flread.dir/src/filereader.cpp.i

CMakeFiles/flread.dir/src/filereader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/flread.dir/src/filereader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maksim/course2/sem3/os/lab1/src/filereader.cpp -o CMakeFiles/flread.dir/src/filereader.cpp.s

# Object files for target flread
flread_OBJECTS = \
"CMakeFiles/flread.dir/src/filereader.cpp.o"

# External object files for target flread
flread_EXTERNAL_OBJECTS =

flread: CMakeFiles/flread.dir/src/filereader.cpp.o
flread: CMakeFiles/flread.dir/build.make
flread: CMakeFiles/flread.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/maksim/course2/sem3/os/lab1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable flread"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/flread.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/flread.dir/build: flread
.PHONY : CMakeFiles/flread.dir/build

CMakeFiles/flread.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/flread.dir/cmake_clean.cmake
.PHONY : CMakeFiles/flread.dir/clean

CMakeFiles/flread.dir/depend:
	cd /home/maksim/course2/sem3/os/lab1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maksim/course2/sem3/os/lab1 /home/maksim/course2/sem3/os/lab1 /home/maksim/course2/sem3/os/lab1/build /home/maksim/course2/sem3/os/lab1/build /home/maksim/course2/sem3/os/lab1/build/CMakeFiles/flread.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/flread.dir/depend

