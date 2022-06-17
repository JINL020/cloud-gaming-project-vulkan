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
protected:
	//const char* filename;
	int num_frame = 0;
public:
	Decoder() {};
	~Decoder() {};

    void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize, char *filename);
    void decode(char *filename);
	void decode(AVCodecContext* dec_ctx, AVFrame* frame, AVPacket* pkt, const char* filename);
};

#endif
