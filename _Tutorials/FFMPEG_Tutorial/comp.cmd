set INC1=ffmpeg-n5.0-latest-win64-gpl-shared-5.0\include
set LIB1=ffmpeg-n5.0-latest-win64-gpl-shared-5.0\lib

cl /c /EHsc /I %INC1% encode.cpp
cl %LIB1%\avcodec.lib %LIB1%\avutil.lib encode.obj

cl /c /EHsc /I %INC1% decode.cpp
cl %LIB1%\avcodec.lib %LIB1%\avutil.lib %LIB1%\avformat.lib decode.obj


