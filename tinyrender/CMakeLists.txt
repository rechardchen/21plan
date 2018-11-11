set (TARGET_NAME TinyRender)

file (GLOB src_files code/*.cpp)
add_executable(${TARGET_NAME} ${src_files})