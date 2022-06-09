@rem set PATH=%PATH%;bin

cl /c /EHsc /ZI /I include video_debugging.cpp

cl /c /EHsc /ZI /I include 0_hello_world.cpp
cl /DEBUG /Od /ZI lib\avcodec.lib lib\avformat.lib lib\avutil.lib 0_hello_world.obj

cl /c /EHsc /ZI /I include 3_transcoding.cpp
cl /DEBUG /Od /ZI lib\avcodec.lib lib\avformat.lib lib\avutil.lib 3_transcoding.obj video_debugging.obj
