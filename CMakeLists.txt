cmake_minimum_required ( VERSION 3.20 )
project ( c_oop )
enable_language ( C )

include ( cmake/api.cmake )
include_directories ( ./include )
set ( CMAKE_C_STANDARD 11 )
set ( CMAKE_C_STANDARD_REQUIRED ON )
set ( CMAKE_C_EXTENSIONS OFF )
set ( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# ##############################################################################
# Debug Mode
# ##############################################################################
set ( CMAKE_C_FLAGS_DEBUG "-g3 -ggdb3 -O0 -Wall" )

# ##############################################################################
# Release Mode
# ##############################################################################
set ( CMAKE_C_FLAGS_RELEASE "-O2 -Wall" )

add_executable ( test src/c_oop.c include/debug.c)
