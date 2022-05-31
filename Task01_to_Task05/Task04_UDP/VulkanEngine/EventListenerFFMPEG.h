/**
	@author Jin-Jin Lee
*/


#ifndef EVENTLISTENERFFMPEG_H
#define EVENTLISTENERFFMPEG_H

/**
*
* \brief An event listener for screenshots
*
* 
*
*/
namespace ve {
	class EventListenerFFMPEG : public VEEventListener {
	private:
		double timePassed = 0;
		const char* filename;
		FILE* file;

	protected:
		virtual void onFrameEnded(veEvent event);


	public:
		EventListenerFFMPEG(std::string name) : VEEventListener(name) {
			filename = "media/video/task03_video_25Framerate_noLoop.mpg";
			file = fopen(filename, "wb");
			if (!file) {
				fprintf(stderr, "could not open %s\n", filename);
				exit(1);
			}
			//uint8_t startcode[] = { 0, 0, 1, 0xb0};
			//fwrite(startcode, 1, sizeof(startcode), file);
		};

		virtual ~EventListenerFFMPEG() {
			// Add sequence end code to have a real MPEG file.
			uint8_t endcode[] = { 0, 0, 1, 0xb7 }; // byte 0, byte 1, byte 2, byte 3, StreamID
			fwrite(endcode, 1, sizeof(endcode), file);
			fclose(file);
		};
	};
}


#endif
