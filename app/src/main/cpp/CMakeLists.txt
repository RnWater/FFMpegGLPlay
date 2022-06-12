cmake_minimum_required(VERSION 3.18.1)
project("ffmpegglplay")
include_directories(include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../../../libs/${ANDROID_ABI}")
add_library(
        ffmpegglplay
        SHARED
        native-lib.cpp
        XLog.cpp
        XData.cpp
        XParameter.cpp
        XThread.cpp
        IObserver.cpp
        IDeMux.cpp
        FFDeMux.cpp
        IDecode.cpp
        FFDecode.cpp
        IResample.cpp
        FFResample.cpp
        IOpenGLVideoView.cpp
        OpenGLVideoView.cpp
        XTexture.cpp
        OpenGLShader.cpp
        OpenEGL.cpp
        IAudioPlay.cpp
        SLAudioPlay.cpp
        IPlayer.cpp
        IPlayerBuilder.cpp
        FFPlayerBuilder.cpp
        IPlayerProxy.cpp
)
find_library(
        log-lib
        log)

target_link_libraries(
        ffmpegglplay
        GLESv2 EGL
        OpenSLES
        android
        avcodec avformat avutil swscale swresample
        ${log-lib})