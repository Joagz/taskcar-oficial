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

# Utility rule file for bootloader.

# Include any custom commands dependencies for this target.
include CMakeFiles/bootloader.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/bootloader.dir/progress.make

CMakeFiles/bootloader: CMakeFiles/bootloader-complete

CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-install
CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-mkdir
CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-download
CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-update
CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-patch
CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-configure
CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-build
CMakeFiles/bootloader-complete: bootloader-prefix/src/bootloader-stamp/bootloader-install
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'bootloader'"
	/usr/bin/cmake -E make_directory /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/CMakeFiles
	/usr/bin/cmake -E touch /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/CMakeFiles/bootloader-complete
	/usr/bin/cmake -E touch /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/bootloader-prefix/src/bootloader-stamp/bootloader-done

bootloader-prefix/src/bootloader-stamp/bootloader-build: bootloader-prefix/src/bootloader-stamp/bootloader-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing build step for 'bootloader'"
	cd /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/bootloader && $(MAKE)

bootloader-prefix/src/bootloader-stamp/bootloader-configure: bootloader-prefix/tmp/bootloader-cfgcmd.txt
bootloader-prefix/src/bootloader-stamp/bootloader-configure: bootloader-prefix/src/bootloader-stamp/bootloader-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Performing configure step for 'bootloader'"
	cd /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/bootloader && /usr/bin/cmake -DSDKCONFIG=/home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/sdkconfig -DIDF_PATH=/home/joagz/esp/esp-idf -DIDF_TARGET=esp32 -DPYTHON_DEPS_CHECKED=1 -DPYTHON=/home/joagz/.espressif/python_env/idf5.2_py3.11_env/bin/python -DEXTRA_COMPONENT_DIRS=/home/joagz/esp/esp-idf/components/bootloader -DPROJECT_SOURCE_DIR=/home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver -DIGNORE_EXTRA_COMPONENT= "-GUnix Makefiles" -S /home/joagz/esp/esp-idf/components/bootloader/subproject -B /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/bootloader
	cd /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/bootloader && /usr/bin/cmake -E touch /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/bootloader-prefix/src/bootloader-stamp/bootloader-configure

bootloader-prefix/src/bootloader-stamp/bootloader-download: bootloader-prefix/src/bootloader-stamp/bootloader-source_dirinfo.txt
bootloader-prefix/src/bootloader-stamp/bootloader-download: bootloader-prefix/src/bootloader-stamp/bootloader-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "No download step for 'bootloader'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/bootloader-prefix/src/bootloader-stamp/bootloader-download

bootloader-prefix/src/bootloader-stamp/bootloader-install: bootloader-prefix/src/bootloader-stamp/bootloader-build
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No install step for 'bootloader'"
	cd /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/bootloader && /usr/bin/cmake -E echo_append

bootloader-prefix/src/bootloader-stamp/bootloader-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'bootloader'"
	/usr/bin/cmake -Dcfgdir= -P /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/bootloader-prefix/tmp/bootloader-mkdirs.cmake
	/usr/bin/cmake -E touch /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/bootloader-prefix/src/bootloader-stamp/bootloader-mkdir

bootloader-prefix/src/bootloader-stamp/bootloader-patch: bootloader-prefix/src/bootloader-stamp/bootloader-patch-info.txt
bootloader-prefix/src/bootloader-stamp/bootloader-patch: bootloader-prefix/src/bootloader-stamp/bootloader-update
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'bootloader'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/bootloader-prefix/src/bootloader-stamp/bootloader-patch

bootloader-prefix/src/bootloader-stamp/bootloader-update: bootloader-prefix/src/bootloader-stamp/bootloader-update-info.txt
bootloader-prefix/src/bootloader-stamp/bootloader-update: bootloader-prefix/src/bootloader-stamp/bootloader-download
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No update step for 'bootloader'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/bootloader-prefix/src/bootloader-stamp/bootloader-update

CMakeFiles/bootloader.dir/codegen:
.PHONY : CMakeFiles/bootloader.dir/codegen

bootloader: CMakeFiles/bootloader
bootloader: CMakeFiles/bootloader-complete
bootloader: bootloader-prefix/src/bootloader-stamp/bootloader-build
bootloader: bootloader-prefix/src/bootloader-stamp/bootloader-configure
bootloader: bootloader-prefix/src/bootloader-stamp/bootloader-download
bootloader: bootloader-prefix/src/bootloader-stamp/bootloader-install
bootloader: bootloader-prefix/src/bootloader-stamp/bootloader-mkdir
bootloader: bootloader-prefix/src/bootloader-stamp/bootloader-patch
bootloader: bootloader-prefix/src/bootloader-stamp/bootloader-update
bootloader: CMakeFiles/bootloader.dir/build.make
.PHONY : bootloader

# Rule to build all files generated by this target.
CMakeFiles/bootloader.dir/build: bootloader
.PHONY : CMakeFiles/bootloader.dir/build

CMakeFiles/bootloader.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bootloader.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bootloader.dir/clean

CMakeFiles/bootloader.dir/depend:
	cd /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build /home/joagz/esp/esp-idf/projects/taskcar-oficial/taskcar-motor-driver/build/CMakeFiles/bootloader.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/bootloader.dir/depend

