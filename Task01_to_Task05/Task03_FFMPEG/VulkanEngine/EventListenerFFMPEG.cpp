/**
	@author Jin-Jin Lee
*/

#include "VEInclude.h"

namespace ve {
	static void encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt, FILE* outfile, UDPSend udpSender)
	{
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
			fwrite(pkt->data, 1, pkt->size, outfile);

			printf("send packet using udp\n");
			udpSender.send(pkt->data,pkt->size);

			av_packet_unref(pkt);
		}
	}


	 EventListenerFFMPEG::EventListenerFFMPEG(std::string name) : VEEventListener(name) {
		 file = fopen(filename, "wb");
		 if (!file) {
			 fprintf(stderr, "could not open %s\n", filename);
			 exit(1);
		 }
		 //uint8_t startcode[] = { 0, 0, 1, 0xb0};
		 //fwrite(startcode, 1, sizeof(startcode), file);
		 udpSender = UDPSend();
		 char address[] = "127.0.0.1";
		 udpSender.init(address, 8088);
	}

	 EventListenerFFMPEG::~EventListenerFFMPEG() {
		 // Add sequence end code to have a real MPEG file.
		 uint8_t endcode[] = { 0, 0, 1, 0xb7 }; // byte 0, byte 1, byte 2, byte 3, StreamID
		 fwrite(endcode, 1, sizeof(endcode), file);
		 fclose(file);
	 };

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

			//-----------------------------BEGIN-----------------------------------//
			avcodec_register_all();

			// Find a registered encoder with a matching codec ID.
			const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_MPEG2VIDEO);
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
			// Set context values.
			context->bit_rate = 40000;
			// Resolution must be a multiple of two!
			context->width = extent.width;
			context->height = extent.height;
			// Frames per second
			context->time_base.num = 1;
			context->time_base.den = 25; // change to 1 because no for loop anymore?
			context->framerate.num = 25; // change to 1 because no for loop anymore?
			context->framerate.den = 1;
			context->gop_size = 10; // Emit one intra frame every ten frames.
			context->max_b_frames = 1;
			context->pix_fmt = AV_PIX_FMT_YUV420P;

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
			//frame->pict_type = AV_PICTURE_TYPE_NONE; // delete it?

			// Allocate new buffer(s) for audio or video data.
			if (av_frame_get_buffer(frame, 32) < 0) {
				fprintf(stderr, "could not alloc the frame data\n");
				exit(1);
			}

			// encode 1 second of video
			fflush(stdout);

			// Make sure the frame data is writable.
			if (av_frame_make_writable(frame) < 0) {
				fprintf(stderr, "Cannot make frame writeable\n");
				exit(1);
			}

			SwsContext* ctx = sws_getContext(context->width, context->height, AV_PIX_FMT_RGBA, context->width, context->height, AV_PIX_FMT_YUV420P, 0, 0, 0, 0);
			uint8_t* inData[1] = { dataImage }; // RGBA32 have one plane
			int inLinesize[1] = { 4 * context->width }; // RGBA stride
			sws_scale(ctx, inData, inLinesize, 0, context->height, frame->data, frame->linesize);

			//frame->pts = i; // What to do with that?

			// Encode the image.
			encode(context, frame, pkt, file, udpSender);

			// Flush the encoder.
			encode(context, NULL, pkt, file, udpSender);

			// Free all the stuff.
			avcodec_free_context(&context);
			av_frame_free(&frame);
			av_packet_free(&pkt);

			printf("\n");
			//-----------------------------END-----------------------------------//

			delete[] dataImage;

			timePassed = 0;

		}
	}
}