# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_COMMAND = /snap/cmake/1082/bin/cmake

# The command to remove a file.
RM = /snap/cmake/1082/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/elsa/libmirai

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/elsa/libmirai/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/MiraiLib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/MiraiLib.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/MiraiLib.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/MiraiLib.dir/flags.make

tests/CMakeFiles/MiraiLib.dir/__/src/utils/encode.cpp.o: tests/CMakeFiles/MiraiLib.dir/flags.make
tests/CMakeFiles/MiraiLib.dir/__/src/utils/encode.cpp.o: ../src/utils/encode.cpp
tests/CMakeFiles/MiraiLib.dir/__/src/utils/encode.cpp.o: tests/CMakeFiles/MiraiLib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/elsa/libmirai/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/MiraiLib.dir/__/src/utils/encode.cpp.o"
	cd /home/elsa/libmirai/build/tests && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/MiraiLib.dir/__/src/utils/encode.cpp.o -MF CMakeFiles/MiraiLib.dir/__/src/utils/encode.cpp.o.d -o CMakeFiles/MiraiLib.dir/__/src/utils/encode.cpp.o -c /home/elsa/libmirai/src/utils/encode.cpp

tests/CMakeFiles/MiraiLib.dir/__/src/utils/encode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MiraiLib.dir/__/src/utils/encode.cpp.i"
	cd /home/elsa/libmirai/build/tests && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/elsa/libmirai/src/utils/encode.cpp > CMakeFiles/MiraiLib.dir/__/src/utils/encode.cpp.i

tests/CMakeFiles/MiraiLib.dir/__/src/utils/encode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MiraiLib.dir/__/src/utils/encode.cpp.s"
	cd /home/elsa/libmirai/build/tests && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/elsa/libmirai/src/utils/encode.cpp -o CMakeFiles/MiraiLib.dir/__/src/utils/encode.cpp.s

# Object files for target MiraiLib
MiraiLib_OBJECTS = \
"CMakeFiles/MiraiLib.dir/__/src/utils/encode.cpp.o"

# External object files for target MiraiLib
MiraiLib_EXTERNAL_OBJECTS =

tests/libMiraiLib.a: tests/CMakeFiles/MiraiLib.dir/__/src/utils/encode.cpp.o
tests/libMiraiLib.a: tests/CMakeFiles/MiraiLib.dir/build.make
tests/libMiraiLib.a: tests/CMakeFiles/MiraiLib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/elsa/libmirai/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libMiraiLib.a"
	cd /home/elsa/libmirai/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/MiraiLib.dir/cmake_clean_target.cmake
	cd /home/elsa/libmirai/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MiraiLib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/MiraiLib.dir/build: tests/libMiraiLib.a
.PHONY : tests/CMakeFiles/MiraiLib.dir/build

tests/CMakeFiles/MiraiLib.dir/clean:
	cd /home/elsa/libmirai/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/MiraiLib.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/MiraiLib.dir/clean

tests/CMakeFiles/MiraiLib.dir/depend:
	cd /home/elsa/libmirai/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/elsa/libmirai /home/elsa/libmirai/tests /home/elsa/libmirai/build /home/elsa/libmirai/build/tests /home/elsa/libmirai/build/tests/CMakeFiles/MiraiLib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/MiraiLib.dir/depend

