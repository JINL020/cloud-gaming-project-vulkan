1. Download the "FFMPEG" and "Example C++ file using FFMPEG" folder from Moodle
    - Put the folders content (bin, doc, include and lib) into the same folder as "Example C++ file using FFMPEG"
    - Have FFMPEG installed i.d. download "FFMPEG for Windows" from Moodle and set path = %PATH%;<YOUR PATH TO THE FFMPEG DIRECTORY>\bin in system variables
    - Put the "FFMPEG for Windows" in the same directory as the FFMPEG folder
    - Change the variables in comü.cmd to match the name of the FFMPEG folder id necessary


2. Start the tutorial
    - Open x64 Native Tools Command Prompt for VS 2022
    - Compile with compile.bat
    - Run transcoding with 3_transcoding.exe small_bunny_1080p_60fps.mp4 sm.mp4
        This transcodes the mp4 file into H.265, leading to a much smaller file size. This means that the original mp4 files is opened, the frames are read, decoded, then re-encoded with the new codec, then stored into another mp4 file.
    - Alternatively, start MSVC debugger with 
      devenv /debugexe 3_transcoding.exe small_bunny_1080p_60fps.mp4 sm.mp4
      Then press F10 to start debugging.
    - For extracting information from the MP4 file and single frames, run 0_hello_world.exe small_bunny_1080p_60fps.mp4