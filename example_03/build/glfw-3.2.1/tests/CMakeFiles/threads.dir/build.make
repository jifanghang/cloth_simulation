# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.8.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.8.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/Jamie/Desktop/CIS materials 2017/example_03"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/Jamie/Desktop/CIS materials 2017/example_03/build"

# Include any dependencies generated for this target.
include glfw-3.2.1/tests/CMakeFiles/threads.dir/depend.make

# Include the progress variables for this target.
include glfw-3.2.1/tests/CMakeFiles/threads.dir/progress.make

# Include the compile flags for this target's objects.
include glfw-3.2.1/tests/CMakeFiles/threads.dir/flags.make

glfw-3.2.1/tests/CMakeFiles/threads.dir/threads.c.o: glfw-3.2.1/tests/CMakeFiles/threads.dir/flags.make
glfw-3.2.1/tests/CMakeFiles/threads.dir/threads.c.o: ../glfw-3.2.1/tests/threads.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/Jamie/Desktop/CIS materials 2017/example_03/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object glfw-3.2.1/tests/CMakeFiles/threads.dir/threads.c.o"
	cd "/Users/Jamie/Desktop/CIS materials 2017/example_03/build/glfw-3.2.1/tests" && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/threads.dir/threads.c.o   -c "/Users/Jamie/Desktop/CIS materials 2017/example_03/glfw-3.2.1/tests/threads.c"

glfw-3.2.1/tests/CMakeFiles/threads.dir/threads.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/threads.dir/threads.c.i"
	cd "/Users/Jamie/Desktop/CIS materials 2017/example_03/build/glfw-3.2.1/tests" && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/Jamie/Desktop/CIS materials 2017/example_03/glfw-3.2.1/tests/threads.c" > CMakeFiles/threads.dir/threads.c.i

glfw-3.2.1/tests/CMakeFiles/threads.dir/threads.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/threads.dir/threads.c.s"
	cd "/Users/Jamie/Desktop/CIS materials 2017/example_03/build/glfw-3.2.1/tests" && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/Jamie/Desktop/CIS materials 2017/example_03/glfw-3.2.1/tests/threads.c" -o CMakeFiles/threads.dir/threads.c.s

glfw-3.2.1/tests/CMakeFiles/threads.dir/threads.c.o.requires:

.PHONY : glfw-3.2.1/tests/CMakeFiles/threads.dir/threads.c.o.requires

glfw-3.2.1/tests/CMakeFiles/threads.dir/threads.c.o.provides: glfw-3.2.1/tests/CMakeFiles/threads.dir/threads.c.o.requires
	$(MAKE) -f glfw-3.2.1/tests/CMakeFiles/threads.dir/build.make glfw-3.2.1/tests/CMakeFiles/threads.dir/threads.c.o.provides.build
.PHONY : glfw-3.2.1/tests/CMakeFiles/threads.dir/threads.c.o.provides

glfw-3.2.1/tests/CMakeFiles/threads.dir/threads.c.o.provides.build: glfw-3.2.1/tests/CMakeFiles/threads.dir/threads.c.o


glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o: glfw-3.2.1/tests/CMakeFiles/threads.dir/flags.make
glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o: ../glfw-3.2.1/deps/tinycthread.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/Jamie/Desktop/CIS materials 2017/example_03/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o"
	cd "/Users/Jamie/Desktop/CIS materials 2017/example_03/build/glfw-3.2.1/tests" && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/threads.dir/__/deps/tinycthread.c.o   -c "/Users/Jamie/Desktop/CIS materials 2017/example_03/glfw-3.2.1/deps/tinycthread.c"

glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/threads.dir/__/deps/tinycthread.c.i"
	cd "/Users/Jamie/Desktop/CIS materials 2017/example_03/build/glfw-3.2.1/tests" && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/Jamie/Desktop/CIS materials 2017/example_03/glfw-3.2.1/deps/tinycthread.c" > CMakeFiles/threads.dir/__/deps/tinycthread.c.i

glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/threads.dir/__/deps/tinycthread.c.s"
	cd "/Users/Jamie/Desktop/CIS materials 2017/example_03/build/glfw-3.2.1/tests" && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/Jamie/Desktop/CIS materials 2017/example_03/glfw-3.2.1/deps/tinycthread.c" -o CMakeFiles/threads.dir/__/deps/tinycthread.c.s

glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.requires:

.PHONY : glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.requires

glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.provides: glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.requires
	$(MAKE) -f glfw-3.2.1/tests/CMakeFiles/threads.dir/build.make glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.provides.build
.PHONY : glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.provides

glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.provides.build: glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o


glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/glad.c.o: glfw-3.2.1/tests/CMakeFiles/threads.dir/flags.make
glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/glad.c.o: ../glfw-3.2.1/deps/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/Jamie/Desktop/CIS materials 2017/example_03/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building C object glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/glad.c.o"
	cd "/Users/Jamie/Desktop/CIS materials 2017/example_03/build/glfw-3.2.1/tests" && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/threads.dir/__/deps/glad.c.o   -c "/Users/Jamie/Desktop/CIS materials 2017/example_03/glfw-3.2.1/deps/glad.c"

glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/threads.dir/__/deps/glad.c.i"
	cd "/Users/Jamie/Desktop/CIS materials 2017/example_03/build/glfw-3.2.1/tests" && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/Jamie/Desktop/CIS materials 2017/example_03/glfw-3.2.1/deps/glad.c" > CMakeFiles/threads.dir/__/deps/glad.c.i

glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/threads.dir/__/deps/glad.c.s"
	cd "/Users/Jamie/Desktop/CIS materials 2017/example_03/build/glfw-3.2.1/tests" && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/Jamie/Desktop/CIS materials 2017/example_03/glfw-3.2.1/deps/glad.c" -o CMakeFiles/threads.dir/__/deps/glad.c.s

glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.requires:

.PHONY : glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.requires

glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.provides: glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.requires
	$(MAKE) -f glfw-3.2.1/tests/CMakeFiles/threads.dir/build.make glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.provides.build
.PHONY : glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.provides

glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.provides.build: glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/glad.c.o


# Object files for target threads
threads_OBJECTS = \
"CMakeFiles/threads.dir/threads.c.o" \
"CMakeFiles/threads.dir/__/deps/tinycthread.c.o" \
"CMakeFiles/threads.dir/__/deps/glad.c.o"

# External object files for target threads
threads_EXTERNAL_OBJECTS =

glfw-3.2.1/tests/threads.app/Contents/MacOS/threads: glfw-3.2.1/tests/CMakeFiles/threads.dir/threads.c.o
glfw-3.2.1/tests/threads.app/Contents/MacOS/threads: glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o
glfw-3.2.1/tests/threads.app/Contents/MacOS/threads: glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/glad.c.o
glfw-3.2.1/tests/threads.app/Contents/MacOS/threads: glfw-3.2.1/tests/CMakeFiles/threads.dir/build.make
glfw-3.2.1/tests/threads.app/Contents/MacOS/threads: glfw-3.2.1/src/libglfw3.a
glfw-3.2.1/tests/threads.app/Contents/MacOS/threads: glfw-3.2.1/tests/CMakeFiles/threads.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/Jamie/Desktop/CIS materials 2017/example_03/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable threads.app/Contents/MacOS/threads"
	cd "/Users/Jamie/Desktop/CIS materials 2017/example_03/build/glfw-3.2.1/tests" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/threads.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
glfw-3.2.1/tests/CMakeFiles/threads.dir/build: glfw-3.2.1/tests/threads.app/Contents/MacOS/threads

.PHONY : glfw-3.2.1/tests/CMakeFiles/threads.dir/build

glfw-3.2.1/tests/CMakeFiles/threads.dir/requires: glfw-3.2.1/tests/CMakeFiles/threads.dir/threads.c.o.requires
glfw-3.2.1/tests/CMakeFiles/threads.dir/requires: glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/tinycthread.c.o.requires
glfw-3.2.1/tests/CMakeFiles/threads.dir/requires: glfw-3.2.1/tests/CMakeFiles/threads.dir/__/deps/glad.c.o.requires

.PHONY : glfw-3.2.1/tests/CMakeFiles/threads.dir/requires

glfw-3.2.1/tests/CMakeFiles/threads.dir/clean:
	cd "/Users/Jamie/Desktop/CIS materials 2017/example_03/build/glfw-3.2.1/tests" && $(CMAKE_COMMAND) -P CMakeFiles/threads.dir/cmake_clean.cmake
.PHONY : glfw-3.2.1/tests/CMakeFiles/threads.dir/clean

glfw-3.2.1/tests/CMakeFiles/threads.dir/depend:
	cd "/Users/Jamie/Desktop/CIS materials 2017/example_03/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/Jamie/Desktop/CIS materials 2017/example_03" "/Users/Jamie/Desktop/CIS materials 2017/example_03/glfw-3.2.1/tests" "/Users/Jamie/Desktop/CIS materials 2017/example_03/build" "/Users/Jamie/Desktop/CIS materials 2017/example_03/build/glfw-3.2.1/tests" "/Users/Jamie/Desktop/CIS materials 2017/example_03/build/glfw-3.2.1/tests/CMakeFiles/threads.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : glfw-3.2.1/tests/CMakeFiles/threads.dir/depend

