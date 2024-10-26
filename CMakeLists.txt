cmake_minimum_required(VERSION 3.10)
project(MyCProject C)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Specify the source directory
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Find all .c files in the src directory
file(GLOB SOURCES "${SOURCE_DIR}/*.c")


# Specify the source directory
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/GL")

# Find all .c files in the src directory
file(GLOB HEADERS "${INCLUDE_DIR}/*.h")
set(FILES ${SOURCES}) #${HEADERS} 

# Create a directory for the executable
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/../bin")

# Include directories for header files
include_directories("${CMAKE_CURRENT_BINARY_DIR}/../include")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/../include/GL")

# Find FreeGLUT 
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Check if FreeGLUT is found
if (NOT GLUT_FOUND)
    message(FATAL_ERROR "GLUT not found!")
endif()

# Add the executable target
add_executable(MyExecutable ${SOURCES})

# Specify output path for the executable
set_target_properties(MyExecutable PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}"
)

# Link the libraries
target_link_libraries(MyExecutable ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

# Generate assembly files for each source file
set(ASM_FILES)  # To hold the assembly files

foreach(SOURCE ${FILES})
    # Get the filename without the extension
    get_filename_component(FILENAME ${SOURCE} NAME_WE)

    # Set the output assembly file name
    set(ASSEMBLY_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/asm/${FILENAME}.s")
    list(APPEND ASM_FILES ${ASSEMBLY_OUTPUT})  # Add to the list of assembly files

    # Add a custom command to generate assembly
    add_custom_command(
        OUTPUT ${ASSEMBLY_OUTPUT}
        COMMAND ${CMAKE_C_COMPILER} -g -O0 -S ${SOURCE} -o ${ASSEMBLY_OUTPUT}
        DEPENDS ${SOURCE}
    )

    # Add a custom target for each assembly file
    add_custom_target(
        ${FILENAME}_asm ALL
        DEPENDS ${ASSEMBLY_OUTPUT}
    )
endforeach()

# Create a target to build all assembly files together
add_custom_target(generate_asm ALL DEPENDS ${ASM_FILES})