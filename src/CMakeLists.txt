# 递归地查找 src 目录及其子目录中的所有 .cpp 文件，并将它们的路径存储在变量 pine_sources 中。
file(GLOB_RECURSE pine_sources ${PROJECT_SOURCE_DIR}/src/*.cpp)

# 将 pine_sources 中的源文件编译成一个共享库，库的名称为 pine_shared。
add_library(pine_shared SHARED ${pine_sources})