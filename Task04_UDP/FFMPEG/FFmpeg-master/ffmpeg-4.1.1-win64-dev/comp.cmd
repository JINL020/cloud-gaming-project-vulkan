set INC1=ffmpeg-4.1.1-win64-dev\include
set LIB1=ffmpeg-4.1.1-win64-dev\lib

cl /c /EHsc /I %INC1% encode.cpp
cl %LIB1%\avcodec.lib %LIB1%\avutil.lib encode.obj

cl /c /EHsc /I %INC1% decode.cpp
cl %LIB1%\avcodec.lib %LIB1%\avutil.lib %LIB1%\avformat.lib decode.obj



