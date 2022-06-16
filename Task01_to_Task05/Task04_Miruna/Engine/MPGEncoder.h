#ifndef MPGENCODER_H
#define MPGENCODER_H

#include "UDPSender.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "libavutil/error.h"
}

namespace ve {
	class MPGEncoder {

	protected:
		const char* filename;
		FILE* file;
		UDPSender sender;
	public:
		MPGEncoder() {
			filename = "video.mpg";
			file = fopen(filename, "wb");

			if (!file) {
				fprintf(stderr, "could not open %s\n", filename);
				exit(1);
			}
			sender.startWinsock();
			char address[] = "127.0.0.1";
			sender.init(address, 8088);
		};

		~MPGEncoder() {
			sender.closeSock();

			uint8_t endcode[] = { 0, 0, 1, 0xb7 };
			fwrite(endcode, 1, sizeof(endcode), file);
			fclose(file);
		};
		
		void encodeFrame(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt);
		void encode(uint8_t* dataImage, int width, int height);
	};
}

#endif
