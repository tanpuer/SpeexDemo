cmake_minimum_required(VERSION 3.10.2)

find_library(log-lib log)

aux_source_directory(src/main/cpp srcs)
aux_source_directory(src/main/cpp/speexdsp srcs)
aux_source_directory(src/main/cpp/speexdsp/speex srcs)

add_library(
        native-lib
        SHARED
        ${srcs}
)

set(JNI_LIBS_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs)

include_directories(
        src/main/cpp
        src/main/cpp/speexdsp
        src/main/cpp/speexdsp/speex
)

target_link_libraries(native-lib ${log-lib})