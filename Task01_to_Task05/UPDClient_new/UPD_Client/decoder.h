extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
#include "libavutil/error.h"
}

#ifndef DECODER_H
#define DECODER_H

class Decoder {
private:
	const AVCodec* codec;
	AVCodecContext* ctx;
	AVCodecParserContext* parser;
	AVPacket* pkt;
	AVFrame* frame;

	//int num_frame = 0;
	//char outname[200];
	const char* outfilename = "video.pgm";

public:
	Decoder();
	~Decoder();
	void decode(uint8_t* data, int data_size);
	void decode(AVCodecContext* dec_ctx, AVFrame* frame, AVPacket* pkt, const char* filename);
	void pgm_save(unsigned char* buf, int wrap, int xsize, int ysize, const char* filename);
	void png_save(uint8_t* data, int width, int height, const char* filename);
};

#endif
