# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_SOURCE_DIR = /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build

# Utility rule file for app-flash.

# Include any custom commands dependencies for this target.
include esp-idf/esptool_py/CMakeFiles/app-flash.dir/compiler_depend.make

# Include the progress variables for this target.
include esp-idf/esptool_py/CMakeFiles/app-flash.dir/progress.make

esp-idf/esptool_py/CMakeFiles/app-flash:
	cd /home/joagz/esp/esp-idf/components/esptool_py && /usr/bin/cmake -D IDF_PATH=/home/joagz/esp/esp-idf -D "SERIAL_TOOL=/home/joagz/.espressif/python_env/idf5.2_py3.11_env/bin/python;;/home/joagz/esp/esp-idf/components/esptool_py/esptool/esptool.py;--chip;esp32" -D "SERIAL_TOOL_ARGS=--before=default_reset;--after=hard_reset;write_flash;@app-flash_args" -D WORKING_DIRECTORY=/home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build -P /home/joagz/esp/esp-idf/components/esptool_py/run_serial_tool.cmake

esp-idf/esptool_py/CMakeFiles/app-flash.dir/codegen:
.PHONY : esp-idf/esptool_py/CMakeFiles/app-flash.dir/codegen

app-flash: esp-idf/esptool_py/CMakeFiles/app-flash
app-flash: esp-idf/esptool_py/CMakeFiles/app-flash.dir/build.make
.PHONY : app-flash

# Rule to build all files generated by this target.
esp-idf/esptool_py/CMakeFiles/app-flash.dir/build: app-flash
.PHONY : esp-idf/esptool_py/CMakeFiles/app-flash.dir/build

esp-idf/esptool_py/CMakeFiles/app-flash.dir/clean:
	cd /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/esp-idf/esptool_py && $(CMAKE_COMMAND) -P CMakeFiles/app-flash.dir/cmake_clean.cmake
.PHONY : esp-idf/esptool_py/CMakeFiles/app-flash.dir/clean

esp-idf/esptool_py/CMakeFiles/app-flash.dir/depend:
	cd /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver /home/joagz/esp/esp-idf/components/esptool_py /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/esp-idf/esptool_py /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/esp-idf/esptool_py/CMakeFiles/app-flash.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : esp-idf/esptool_py/CMakeFiles/app-flash.dir/depend

