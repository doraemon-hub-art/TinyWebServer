# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xuanxuan/my_files/TinyWebServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xuanxuan/my_files/TinyWebServer

# Include any dependencies generated for this target.
include CMakeFiles/TinyWebServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TinyWebServer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TinyWebServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TinyWebServer.dir/flags.make

CMakeFiles/TinyWebServer.dir/main.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/main.cpp.o: main.cpp
CMakeFiles/TinyWebServer.dir/main.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xuanxuan/my_files/TinyWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TinyWebServer.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/main.cpp.o -MF CMakeFiles/TinyWebServer.dir/main.cpp.o.d -o CMakeFiles/TinyWebServer.dir/main.cpp.o -c /home/xuanxuan/my_files/TinyWebServer/main.cpp

CMakeFiles/TinyWebServer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xuanxuan/my_files/TinyWebServer/main.cpp > CMakeFiles/TinyWebServer.dir/main.cpp.i

CMakeFiles/TinyWebServer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xuanxuan/my_files/TinyWebServer/main.cpp -o CMakeFiles/TinyWebServer.dir/main.cpp.s

CMakeFiles/TinyWebServer.dir/log/log.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/log/log.cpp.o: log/log.cpp
CMakeFiles/TinyWebServer.dir/log/log.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xuanxuan/my_files/TinyWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/TinyWebServer.dir/log/log.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/log/log.cpp.o -MF CMakeFiles/TinyWebServer.dir/log/log.cpp.o.d -o CMakeFiles/TinyWebServer.dir/log/log.cpp.o -c /home/xuanxuan/my_files/TinyWebServer/log/log.cpp

CMakeFiles/TinyWebServer.dir/log/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/log/log.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xuanxuan/my_files/TinyWebServer/log/log.cpp > CMakeFiles/TinyWebServer.dir/log/log.cpp.i

CMakeFiles/TinyWebServer.dir/log/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/log/log.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xuanxuan/my_files/TinyWebServer/log/log.cpp -o CMakeFiles/TinyWebServer.dir/log/log.cpp.s

CMakeFiles/TinyWebServer.dir/epoller/epoller.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/epoller/epoller.cpp.o: epoller/epoller.cpp
CMakeFiles/TinyWebServer.dir/epoller/epoller.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xuanxuan/my_files/TinyWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/TinyWebServer.dir/epoller/epoller.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/epoller/epoller.cpp.o -MF CMakeFiles/TinyWebServer.dir/epoller/epoller.cpp.o.d -o CMakeFiles/TinyWebServer.dir/epoller/epoller.cpp.o -c /home/xuanxuan/my_files/TinyWebServer/epoller/epoller.cpp

CMakeFiles/TinyWebServer.dir/epoller/epoller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/epoller/epoller.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xuanxuan/my_files/TinyWebServer/epoller/epoller.cpp > CMakeFiles/TinyWebServer.dir/epoller/epoller.cpp.i

CMakeFiles/TinyWebServer.dir/epoller/epoller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/epoller/epoller.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xuanxuan/my_files/TinyWebServer/epoller/epoller.cpp -o CMakeFiles/TinyWebServer.dir/epoller/epoller.cpp.s

CMakeFiles/TinyWebServer.dir/server/server.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/server/server.cpp.o: server/server.cpp
CMakeFiles/TinyWebServer.dir/server/server.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xuanxuan/my_files/TinyWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/TinyWebServer.dir/server/server.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/server/server.cpp.o -MF CMakeFiles/TinyWebServer.dir/server/server.cpp.o.d -o CMakeFiles/TinyWebServer.dir/server/server.cpp.o -c /home/xuanxuan/my_files/TinyWebServer/server/server.cpp

CMakeFiles/TinyWebServer.dir/server/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/server/server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xuanxuan/my_files/TinyWebServer/server/server.cpp > CMakeFiles/TinyWebServer.dir/server/server.cpp.i

CMakeFiles/TinyWebServer.dir/server/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/server/server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xuanxuan/my_files/TinyWebServer/server/server.cpp -o CMakeFiles/TinyWebServer.dir/server/server.cpp.s

CMakeFiles/TinyWebServer.dir/http/http_conn/http_conn.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/http/http_conn/http_conn.cpp.o: http/http_conn/http_conn.cpp
CMakeFiles/TinyWebServer.dir/http/http_conn/http_conn.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xuanxuan/my_files/TinyWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/TinyWebServer.dir/http/http_conn/http_conn.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/http/http_conn/http_conn.cpp.o -MF CMakeFiles/TinyWebServer.dir/http/http_conn/http_conn.cpp.o.d -o CMakeFiles/TinyWebServer.dir/http/http_conn/http_conn.cpp.o -c /home/xuanxuan/my_files/TinyWebServer/http/http_conn/http_conn.cpp

CMakeFiles/TinyWebServer.dir/http/http_conn/http_conn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/http/http_conn/http_conn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xuanxuan/my_files/TinyWebServer/http/http_conn/http_conn.cpp > CMakeFiles/TinyWebServer.dir/http/http_conn/http_conn.cpp.i

CMakeFiles/TinyWebServer.dir/http/http_conn/http_conn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/http/http_conn/http_conn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xuanxuan/my_files/TinyWebServer/http/http_conn/http_conn.cpp -o CMakeFiles/TinyWebServer.dir/http/http_conn/http_conn.cpp.s

CMakeFiles/TinyWebServer.dir/buffer/buffer.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/buffer/buffer.cpp.o: buffer/buffer.cpp
CMakeFiles/TinyWebServer.dir/buffer/buffer.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xuanxuan/my_files/TinyWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/TinyWebServer.dir/buffer/buffer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/buffer/buffer.cpp.o -MF CMakeFiles/TinyWebServer.dir/buffer/buffer.cpp.o.d -o CMakeFiles/TinyWebServer.dir/buffer/buffer.cpp.o -c /home/xuanxuan/my_files/TinyWebServer/buffer/buffer.cpp

CMakeFiles/TinyWebServer.dir/buffer/buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/buffer/buffer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xuanxuan/my_files/TinyWebServer/buffer/buffer.cpp > CMakeFiles/TinyWebServer.dir/buffer/buffer.cpp.i

CMakeFiles/TinyWebServer.dir/buffer/buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/buffer/buffer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xuanxuan/my_files/TinyWebServer/buffer/buffer.cpp -o CMakeFiles/TinyWebServer.dir/buffer/buffer.cpp.s

CMakeFiles/TinyWebServer.dir/heap_timer/heap_timer.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/heap_timer/heap_timer.cpp.o: heap_timer/heap_timer.cpp
CMakeFiles/TinyWebServer.dir/heap_timer/heap_timer.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xuanxuan/my_files/TinyWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/TinyWebServer.dir/heap_timer/heap_timer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/heap_timer/heap_timer.cpp.o -MF CMakeFiles/TinyWebServer.dir/heap_timer/heap_timer.cpp.o.d -o CMakeFiles/TinyWebServer.dir/heap_timer/heap_timer.cpp.o -c /home/xuanxuan/my_files/TinyWebServer/heap_timer/heap_timer.cpp

CMakeFiles/TinyWebServer.dir/heap_timer/heap_timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/heap_timer/heap_timer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xuanxuan/my_files/TinyWebServer/heap_timer/heap_timer.cpp > CMakeFiles/TinyWebServer.dir/heap_timer/heap_timer.cpp.i

CMakeFiles/TinyWebServer.dir/heap_timer/heap_timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/heap_timer/heap_timer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xuanxuan/my_files/TinyWebServer/heap_timer/heap_timer.cpp -o CMakeFiles/TinyWebServer.dir/heap_timer/heap_timer.cpp.s

CMakeFiles/TinyWebServer.dir/sql_conn_pool/sql_conn_pool.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/sql_conn_pool/sql_conn_pool.cpp.o: sql_conn_pool/sql_conn_pool.cpp
CMakeFiles/TinyWebServer.dir/sql_conn_pool/sql_conn_pool.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xuanxuan/my_files/TinyWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/TinyWebServer.dir/sql_conn_pool/sql_conn_pool.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/sql_conn_pool/sql_conn_pool.cpp.o -MF CMakeFiles/TinyWebServer.dir/sql_conn_pool/sql_conn_pool.cpp.o.d -o CMakeFiles/TinyWebServer.dir/sql_conn_pool/sql_conn_pool.cpp.o -c /home/xuanxuan/my_files/TinyWebServer/sql_conn_pool/sql_conn_pool.cpp

CMakeFiles/TinyWebServer.dir/sql_conn_pool/sql_conn_pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/sql_conn_pool/sql_conn_pool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xuanxuan/my_files/TinyWebServer/sql_conn_pool/sql_conn_pool.cpp > CMakeFiles/TinyWebServer.dir/sql_conn_pool/sql_conn_pool.cpp.i

CMakeFiles/TinyWebServer.dir/sql_conn_pool/sql_conn_pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/sql_conn_pool/sql_conn_pool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xuanxuan/my_files/TinyWebServer/sql_conn_pool/sql_conn_pool.cpp -o CMakeFiles/TinyWebServer.dir/sql_conn_pool/sql_conn_pool.cpp.s

CMakeFiles/TinyWebServer.dir/http/http_request/http_request.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/http/http_request/http_request.cpp.o: http/http_request/http_request.cpp
CMakeFiles/TinyWebServer.dir/http/http_request/http_request.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xuanxuan/my_files/TinyWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/TinyWebServer.dir/http/http_request/http_request.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/http/http_request/http_request.cpp.o -MF CMakeFiles/TinyWebServer.dir/http/http_request/http_request.cpp.o.d -o CMakeFiles/TinyWebServer.dir/http/http_request/http_request.cpp.o -c /home/xuanxuan/my_files/TinyWebServer/http/http_request/http_request.cpp

CMakeFiles/TinyWebServer.dir/http/http_request/http_request.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/http/http_request/http_request.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xuanxuan/my_files/TinyWebServer/http/http_request/http_request.cpp > CMakeFiles/TinyWebServer.dir/http/http_request/http_request.cpp.i

CMakeFiles/TinyWebServer.dir/http/http_request/http_request.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/http/http_request/http_request.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xuanxuan/my_files/TinyWebServer/http/http_request/http_request.cpp -o CMakeFiles/TinyWebServer.dir/http/http_request/http_request.cpp.s

CMakeFiles/TinyWebServer.dir/http/http_response/http_response.cpp.o: CMakeFiles/TinyWebServer.dir/flags.make
CMakeFiles/TinyWebServer.dir/http/http_response/http_response.cpp.o: http/http_response/http_response.cpp
CMakeFiles/TinyWebServer.dir/http/http_response/http_response.cpp.o: CMakeFiles/TinyWebServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xuanxuan/my_files/TinyWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/TinyWebServer.dir/http/http_response/http_response.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TinyWebServer.dir/http/http_response/http_response.cpp.o -MF CMakeFiles/TinyWebServer.dir/http/http_response/http_response.cpp.o.d -o CMakeFiles/TinyWebServer.dir/http/http_response/http_response.cpp.o -c /home/xuanxuan/my_files/TinyWebServer/http/http_response/http_response.cpp

CMakeFiles/TinyWebServer.dir/http/http_response/http_response.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TinyWebServer.dir/http/http_response/http_response.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xuanxuan/my_files/TinyWebServer/http/http_response/http_response.cpp > CMakeFiles/TinyWebServer.dir/http/http_response/http_response.cpp.i

CMakeFiles/TinyWebServer.dir/http/http_response/http_response.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TinyWebServer.dir/http/http_response/http_response.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xuanxuan/my_files/TinyWebServer/http/http_response/http_response.cpp -o CMakeFiles/TinyWebServer.dir/http/http_response/http_response.cpp.s

# Object files for target TinyWebServer
TinyWebServer_OBJECTS = \
"CMakeFiles/TinyWebServer.dir/main.cpp.o" \
"CMakeFiles/TinyWebServer.dir/log/log.cpp.o" \
"CMakeFiles/TinyWebServer.dir/epoller/epoller.cpp.o" \
"CMakeFiles/TinyWebServer.dir/server/server.cpp.o" \
"CMakeFiles/TinyWebServer.dir/http/http_conn/http_conn.cpp.o" \
"CMakeFiles/TinyWebServer.dir/buffer/buffer.cpp.o" \
"CMakeFiles/TinyWebServer.dir/heap_timer/heap_timer.cpp.o" \
"CMakeFiles/TinyWebServer.dir/sql_conn_pool/sql_conn_pool.cpp.o" \
"CMakeFiles/TinyWebServer.dir/http/http_request/http_request.cpp.o" \
"CMakeFiles/TinyWebServer.dir/http/http_response/http_response.cpp.o"

# External object files for target TinyWebServer
TinyWebServer_EXTERNAL_OBJECTS =

TinyWebServer: CMakeFiles/TinyWebServer.dir/main.cpp.o
TinyWebServer: CMakeFiles/TinyWebServer.dir/log/log.cpp.o
TinyWebServer: CMakeFiles/TinyWebServer.dir/epoller/epoller.cpp.o
TinyWebServer: CMakeFiles/TinyWebServer.dir/server/server.cpp.o
TinyWebServer: CMakeFiles/TinyWebServer.dir/http/http_conn/http_conn.cpp.o
TinyWebServer: CMakeFiles/TinyWebServer.dir/buffer/buffer.cpp.o
TinyWebServer: CMakeFiles/TinyWebServer.dir/heap_timer/heap_timer.cpp.o
TinyWebServer: CMakeFiles/TinyWebServer.dir/sql_conn_pool/sql_conn_pool.cpp.o
TinyWebServer: CMakeFiles/TinyWebServer.dir/http/http_request/http_request.cpp.o
TinyWebServer: CMakeFiles/TinyWebServer.dir/http/http_response/http_response.cpp.o
TinyWebServer: CMakeFiles/TinyWebServer.dir/build.make
TinyWebServer: CMakeFiles/TinyWebServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xuanxuan/my_files/TinyWebServer/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable TinyWebServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TinyWebServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TinyWebServer.dir/build: TinyWebServer
.PHONY : CMakeFiles/TinyWebServer.dir/build

CMakeFiles/TinyWebServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TinyWebServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TinyWebServer.dir/clean

CMakeFiles/TinyWebServer.dir/depend:
	cd /home/xuanxuan/my_files/TinyWebServer && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xuanxuan/my_files/TinyWebServer /home/xuanxuan/my_files/TinyWebServer /home/xuanxuan/my_files/TinyWebServer /home/xuanxuan/my_files/TinyWebServer /home/xuanxuan/my_files/TinyWebServer/CMakeFiles/TinyWebServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TinyWebServer.dir/depend

