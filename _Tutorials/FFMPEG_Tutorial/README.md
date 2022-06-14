1. Download the "FFMPEG" folder from Moodle
    - The folder should contain: comp.cmd, decode.cpp and encode.cpp
    - Have FFMPEG installed i.d. download "FFMPEG for Windows" from Moodle and set path = %PATH%;<YOUR PATH TO THE FFMPEG DIRECTORY>\bin in system variables
    - Put the "FFMPEG for Windows" in the same directory as the FFMPEG folder
    - Change the variables in comü.cmd to match the name of the FFMPEG folder id necessary

2. Start the tutorial
    - Open x64 Native Tools Command Prompt for VS 2022
    - comp.cmd
    - encode out.mpg
    - decode out.mpg out
    - It will produce a number of PGM grayscale images (P5/binary)