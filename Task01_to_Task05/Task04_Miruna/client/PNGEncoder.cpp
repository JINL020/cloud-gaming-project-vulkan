#include "PNGEncoder.h"


void PNGEncoder::png_save(unsigned char* buf, int wrap, int xsize, int ysize, const char* filename)
{
	FILE* f = fopen(filename, "wb");
	fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
	for (int i = 0; i < ysize; i++) {
		fwrite(buf + i * wrap, 1, xsize, f);
		// unsigned char* ch = (buf + i * wrap);
		//ProcessArray(ch, xsize);
		// fwrite(ch, 1, xsize * 3, f);
	}
	fclose(f);
}

void PNGEncoder::encodeFrame(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt, const char* filename)
{
	/*
	struct SwsContext* sws_ctx = NULL;
	char buf[1024];

	int ret = avcodec_send_packet(dec_ctx, pkt);
	if (ret < 0) {
		fprintf(stderr, "Error sending a packet for decoding\n");
		exit(1);
	}

	sws_ctx = sws_getContext(dec_ctx->width,
		dec_ctx->height,
		dec_ctx->pix_fmt,
		dec_ctx->width,
		dec_ctx->height,
		AV_PIX_FMT_RGB24,
		SWS_BICUBIC,
		NULL,
		NULL,
		NULL);

	if (sws_ctx == nullptr)
	{
		return;  //Error!
	}

	AVFrame* pRGBFrame = av_frame_alloc();

	pRGBFrame->format = AV_PIX_FMT_RGB24;
	pRGBFrame->width = dec_ctx->width;
	pRGBFrame->height = dec_ctx->height;

	int sts = av_frame_get_buffer(pRGBFrame, 0);

	if (sts < 0)
	{
		return;  //Error!
	}

	while (ret >= 0) {
		ret = avcodec_receive_frame(dec_ctx, frame);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			return;
		else if (ret < 0) {
			fprintf(stderr, "Error during decoding\n");
			exit(1);
		}

		printf("saving frame %3d\n", dec_ctx->frame_number);
		fflush(stdout);

		sts = sws_scale(sws_ctx,                //struct SwsContext* c,
			frame->data,            //const uint8_t* const srcSlice[],
			frame->linesize,        //const int srcStride[],
			0,                      //int srcSliceY,
			frame->height,          //int srcSliceH,
			pRGBFrame->data,        //uint8_t* const dst[],
			pRGBFrame->linesize);   //const int dstStride[]);

		if (sts != frame->height)
		{
			return;  //Error!
		}

		// the picture is allocated by the decoder. no need to
		//      free it
		snprintf(buf, sizeof(buf), filename, dec_ctx->frame_number);
		//png_save(pRGBFrame->data[0], pRGBFrame->linesize[0], pRGBFrame->width, pRGBFrame->height, buf);

	}

	sws_freeContext(sws_ctx);
	av_frame_free(&pRGBFrame);
	*/

	int ret;

	// send the frame to the encoder
	ret = avcodec_send_frame(enc_ctx, frame);
	if (ret < 0) {
		fprintf(stderr, "error sending a frame for encoding\n");
		exit(1);
	}

	while (ret >= 0) {
		int ret = avcodec_receive_packet(enc_ctx, pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			return;
		else if (ret < 0) {
			fprintf(stderr, "error during encoding\n");
			exit(1);
		}

		printf("encoded frame %lld (size=%5d)\n", pkt->pts, pkt->size);
		// png_save(frame->data[0], frame->linesize[0], frame->width, frame->height, filename);

		FILE* file = fopen(filename, "wb");
		fwrite(pkt->data, 1, pkt->size, file);
		fclose(file);
	}
}

void PNGEncoder::encode(uint8_t* dataImage, int width, int height) {
	int i = 0;
	char outname[200];

	avcodec_register_all();

	// Find a registered encoder with a matching codec ID.
	const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_PNG);
	if (!codec) {
		fprintf(stderr, "codec not found\n");
		exit(1);
	}

	// Allocate an AVCodecContext and set its fields to default values.
	AVCodecContext* context = avcodec_alloc_context3(codec);
	if (!context) {
		fprintf(stderr, "Could not allocate video codec context\n");
		exit(2);
	}
	context->bit_rate = 400000;
	// resolution must be a multiple of two
	context->width = width;
	context->height = height;
	// frames per second
	context->time_base.num = 1;
	context->time_base.den = 25;
	context->framerate.num = 25;
	context->framerate.den = 1;
	context->gop_size = 10; // emit one intra frame every ten frames
	context->max_b_frames = 1;
	context->pix_fmt = AV_PIX_FMT_RGB24;

	// Initialize the AVCodecContext to use the given AVCodec (open it)
	if (avcodec_open2(context, codec, NULL) < 0) {
		fprintf(stderr, "could not open codec\n");
		exit(1);
	}

	// Allocate an AVPacket and set its fields to default values.
	AVPacket* pkt = av_packet_alloc();
	if (!pkt) {
		fprintf(stderr, "Cannot alloc packet\n");
		exit(1);
	}

	// Allocate an AVFrame and set its fields to default values.
	AVFrame* frame = av_frame_alloc();
	if (!frame) {
		fprintf(stderr, "Could not allocate video frame\n");
		exit(5);
	}
	frame->format = context->pix_fmt;
	frame->width = context->width;
	frame->height = context->height;


	// Allocate new buffer(s) for audio or video data.
	if (av_frame_get_buffer(frame, 32) < 0) {
		fprintf(stderr, "could not alloc the frame data\n");
		exit(1);
	}

	fflush(stdout);

	// make sure the frame data is writable
	if (av_frame_make_writable(frame) < 0) {
		fprintf(stderr, "Cannot make frame writeable\n");
		exit(1);
	}

	for(int i = 0; i < 25; i++)
		frame->pts = 0;

	// Encode the image.
	sprintf(outname, "%s%d.png", outfilename, num_frame++);
	encodeFrame(context, frame, pkt, outname);
	av_packet_unref(pkt);

	// Flush the encoder.
	encodeFrame(context, NULL, pkt, outname);
	av_packet_unref(pkt);

	// Free all the stuff.
	avcodec_free_context(&context);
	av_frame_free(&frame);
	av_packet_free(&pkt);

	printf("\n");
}