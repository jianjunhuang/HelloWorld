cmake_minimum_required(VERSION 3.4.1)

set(FFMPEG_LIBS_PATH ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
set(FFMPEG_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/include)


add_library( # Sets the name of the library.
        native-codec

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        decoder/audio_codec_jni.cpp
        decoder/ffmpeg_audio_decoder.cpp
        )

add_library(
         # Sets the name of the library.
        native-extractor

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        extractor/audio_codec_jni.cpp
        extractor/ffmpeg_audio_extractor.cpp
)

add_library(
        # Sets the name of the library.
        native-retriever

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        retriever/retriever_jni.cpp
        retriever/ffmpeg_retriever.cpp
)

target_include_directories(native-codec PRIVATE ${FFMPEG_INCLUDE_PATH})
target_include_directories(native-extractor PRIVATE ${FFMPEG_INCLUDE_PATH})
target_include_directories(native-retriever PRIVATE ${FFMPEG_INCLUDE_PATH})

foreach (libname IN ITEMS avcodec avformat avutil swresample swscale yuv)
    find_library(
            ${libname}
            ${libname}
            PATHS ${FFMPEG_LIBS_PATH}
            NO_CMAKE_FIND_ROOT_PATH
    )
endforeach ()

#add_library(
#        avcodec
#        SHARED
#        IMPORTED)
#set_target_properties(
#        avcodec
#        PROPERTIES IMPORTED_LOCATION
#        ${FFMPEG_LIBS_PATH}/libavcodec.so)
#
#add_library(
#        avformat
#        SHARED
#        IMPORTED)
#set_target_properties(
#        avformat
#        PROPERTIES IMPORTED_LOCATION
#        ${FFMPEG_LIBS_PATH}/libavformat.so)
#
#add_library(
#        avutil
#        SHARED
#        IMPORTED)
#set_target_properties(
#        avutil
#        PROPERTIES IMPORTED_LOCATION
#        ${FFMPEG_LIBS_PATH}/libavutil.so)
#
#add_library(
#        swresample
#        SHARED
#        IMPORTED)
#set_target_properties(
#        swresample
#        PROPERTIES IMPORTED_LOCATION
#        ${FFMPEG_LIBS_PATH}/libswresample.so)


#find_library(
#        avcodec
#        avcodec
#        PATHS ${FFMPEG_LIBS_PATH}
#        NO_CMAKE_FIND_ROOT_PATH
#)
#
#find_library(
#        avformat
#        avformat
#        PATHS ${FFMPEG_LIBS_PATH}
#        NO_CMAKE_FIND_ROOT_PATH
#)
#
#find_library(
#        avutil
#        avutil
#        PATHS ${FFMPEG_LIBS_PATH}
#        NO_CMAKE_FIND_ROOT_PATH
#)
#
#find_library(
#        swresample
#        swresample
#        PATHS ${FFMPEG_LIBS_PATH}
#        NO_CMAKE_FIND_ROOT_PATH
#)

find_library( # Sets the name of the path variable.
        log-lib
        log)
find_library(jnigraphics-lib jnigraphics)

target_link_libraries( # Specifies the target library.
        native-codec
        ${avcodec}
        ${avformat}
        ${avutil}
        ${swresample}
        ${swscale}
        ${log-lib})

target_link_libraries( # Specifies the target library.
        native-extractor
        ${avcodec}
        ${avformat}
        ${avutil}
        ${swresample}
        ${swscale}
        ${log-lib})

target_link_libraries( # Specifies the target library.
        native-retriever
        ${avcodec}
        ${avformat}
        ${avutil}
        ${swresample}
        ${swscale}
        ${yuv}
        ${jnigraphics-lib}
        ${log-lib})

