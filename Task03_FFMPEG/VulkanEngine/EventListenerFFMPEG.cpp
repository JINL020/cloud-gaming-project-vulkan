/**
    @author Jin-Jin Lee
*/

#include "VEInclude.h"

static void encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt, FILE* outfile)
{
	int ret;

	// send the frame to the encoder */
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
		fwrite(pkt->data, 1, pkt->size, outfile);
		av_packet_unref(pkt);
	}
}

namespace ve {


	void EventListenerFFMPEG::onFrameEnded(veEvent event) {

		timePassed += event.dt;

		if (timePassed >= 0.30) {
			VkExtent2D extent = getWindowPointer()->getExtent();
			uint32_t imageSize = extent.width * extent.height * 4;
			VkImage image = getRendererPointer()->getSwapChainImage();

			uint8_t* dataImage = new uint8_t[imageSize];

			vh::vhBufCopySwapChainImageToHost(getRendererPointer()->getDevice(),
				getRendererPointer()->getVmaAllocator(),
				getRendererPointer()->getGraphicsQueue(),
				getRendererPointer()->getCommandPool(),
				image, VK_FORMAT_R8G8B8A8_UNORM,
				VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				dataImage, extent.width, extent.height, imageSize);

			//////////////////////////////////////////////////////////
			avcodec_register_all();

			const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_MPEG4);
			if (!codec) {
				fprintf(stderr, "codec not found\n");
				exit(1);
			}

			AVCodecContext* c = avcodec_alloc_context3(codec);
			if (!c) {
				fprintf(stderr, "Could not allocate video codec context\n");
				exit(2);
			}
			c->bit_rate = 4000;

			// resolution must be a multiple of two
			c->width = extent.width;
			c->height = extent.height;
			// frames per second
			c->time_base.num = 1;
			c->time_base.den = 25;
			c->framerate.num = 25;
			c->framerate.den = 1;

			c->gop_size = 10; // emit one intra frame every ten frames
			c->max_b_frames = 1;
			c->pix_fmt = AV_PIX_FMT_YUV420P;

			AVFrame* frame = av_frame_alloc();
			if (!frame) {
				fprintf(stderr, "Could not allocate video frame\n");
				exit(5);
			}
			frame->format = c->pix_fmt;
			frame->width = c->width;
			frame->height = c->height;

			AVPacket* pkt = av_packet_alloc();
			if (!pkt) {
				fprintf(stderr, "Cannot alloc packet\n");
				exit(1);
			}

			// open it
			if (avcodec_open2(c, codec, NULL) < 0) {
				fprintf(stderr, "could not open codec\n");
				exit(1);
			}

			const char* filename = "media/frames/task03_video.mpg";
			FILE* file = fopen(filename, "wb");
			if (!file) {
				fprintf(stderr, "could not open %s\n", filename);
				exit(1);
			}

			int ret = av_frame_get_buffer(frame, 32);
			if (ret < 0) {
				fprintf(stderr, "could not alloc the frame data\n");
				exit(1);
			}

			SwsContext* ctx = sws_getContext(c->width, c->height,
				AV_PIX_FMT_RGBA, c->width, c->height,
				AV_PIX_FMT_YUV420P, 0, 0, 0, 0);

			uint8_t* inData[1] = { dataImage }; // RGBA32 have one plane
			int inLinesize[1] = { 4 * c->width }; // RGBA stride
			sws_scale(ctx, inData, inLinesize, 0, c->height, frame->data, frame->linesize);


			// encode 1 second of video
			for (int i = 0; i < 25; i++) {
				fflush(stdout);

				// make sure the frame data is writable
				ret = av_frame_make_writable(frame);
				if (ret < 0) {
					fprintf(stderr, "Cannot make frame writeable\n");
					exit(1);
				}

				for (int y = 0; y < c->height; y++) {
					for (int x = 0; x < c->width; x++) {
						frame->data[0][y * frame->linesize[0] + x] = x + y + i * 3;
					}
				}

				// Cb and Cr
				for (int y = 0; y < c->height / 2; y++) {
					for (int x = 0; x < c->width / 2; x++) {
						frame->data[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
						frame->data[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
					}
				}

				frame->pts = i;

				// encode the image
				encode(c, frame, pkt, file);
			}

			encode(c, NULL, pkt, file);
				
			// add sequence end code to have a real MPEG file
			uint8_t endcode[] = { 0, 0, 1, 0xb7 };
			fwrite(endcode, 1, sizeof(endcode), file);
			fclose(file);

			avcodec_free_context(&c);
			av_frame_free(&frame);
			av_packet_free(&pkt);
			//////////////////////////////////////////////////////////////
			delete[] dataImage;

			timePassed = 0;
		
		}
	}
}