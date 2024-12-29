cmake_minimum_required ( VERSION 3.21 )
project ( c_oop )
enable_language ( C )

add_compile_definitions ( GEN_LOGFILE=1 )

# ##############################################################################
# Custom Include
# ##############################################################################
include ( cmake/api.cmake )
add_library ( oop_include OBJECT ./include/debug.c )
target_include_directories ( oop_include BEFORE PUBLIC ./include )
link_libraries ( oop_include )

set ( CMAKE_C_STANDARD 11 )
set ( CMAKE_C_STANDARD_REQUIRED ON )
set ( CMAKE_C_EXTENSIONS OFF )
set ( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# ##############################################################################
# Debug Mode
# ##############################################################################
# You can add the `-Wno-incompatible-pointer-types` flag
set ( CMAKE_C_FLAGS_DEBUG "-g3 -ggdb3 -O0 -Wall" )

# ##############################################################################
# Release Mode
# ##############################################################################
set ( CMAKE_C_FLAGS_RELEASE "-O2 -Wall" )

# ##############################################################################
# Export C_OOP library
# ##############################################################################
set ( srcs
    ./src/c_oop.c
    ./src/hashtable.c
)
add_library ( c_oop STATIC )
target_sources ( c_oop PRIVATE ${srcs} )
target_include_directories ( c_oop PUBLIC
    ./src
    ./src/reflection )

# ##############################################################################
# Test Target
# ##############################################################################
add_executable ( test_poly ./test/test_poly.c )
target_link_libraries ( test_poly PRIVATE c_oop )