#ifndef PNGENCODER_H
#define PNGENCODER_H

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "libavutil/error.h"
}

class PNGEncoder
{
protected:
	const char* outfilename = "out";
	int num_frame = 0;
public:
	PNGEncoder() {};
	~PNGEncoder() {};

	void encode(uint8_t* dataImage, int width, int height);
	void encodeFrame(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt, const char* filename);
	void png_save(unsigned char* buf, int wrap, int xsize, int ysize, const char* filename);
};

#endif