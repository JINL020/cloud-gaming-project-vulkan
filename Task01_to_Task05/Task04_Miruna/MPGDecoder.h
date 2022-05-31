#ifndef MPGENCODER_H
#define MPGENCODER_H

#include "PNGEncoder.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "libavutil/error.h"
}


class MPGDecoder {

protected:
	const char* filename;
	int num_frame = 0;
public:
	MPGDecoder() {};
	~MPGDecoder() {};

	void decode(uint8_t* inbuf, size_t data_sizes);
	void decodeFrame(AVCodecContext* dec_ctx, AVFrame* frame, AVPacket* pkt, const char* filename);
	void png_save(unsigned char* buf, int wrap, int xsize, int ysize, char* filename);

};

#endif
