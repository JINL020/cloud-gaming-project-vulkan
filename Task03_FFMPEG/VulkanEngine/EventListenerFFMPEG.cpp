/**
    @author Jin-Jin Lee
*/

#include "VEInclude.h"

namespace ve {
    void EventListenerFFMPEG::onFrameEnded(veEvent event) {

        timePassed += event.dt;

        if (timePassed >= 0.30) {
            // std::cout << timePassed;
            // std::cout << "\n";

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
			FILE* file = fopen("media/frames/task03_video.mpg", "wb");

			int got_output, i;

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
			c->bit_rate = 400000;

			// resolution must be a multiple of two
			c->width = 352;
			c->height = 288;
			// frames per second
			c->time_base.num = 1;
			c->time_base.den = 25;
			c->framerate.num = 25;
			c->framerate.den = 1;

			c->gop_size = 10; // emit one intra frame every ten frames
			c->max_b_frames = 1;
			c->pix_fmt = AV_PIX_FMT_YUV420P;

			// open it
			if (avcodec_open2(c, codec, NULL) < 0) {
				fprintf(stderr, "could not open codec\n");
				exit(1);
			}

			AVFrame* frame = av_frame_alloc();

			if (!frame) {
				fprintf(stderr, "Could not allocate video frame\n");
				exit(5);
			}
			frame->format = c->pix_fmt;
			frame->width = c->width;
			frame->height = c->height;

			AVPacket pkt;
			/*if (!pkt) {
				fprintf(stderr, "Cannot alloc packet\n");
				exit(1);
			}*/

			/* the image can be allocated by any means and av_image_alloc() is
			* just the most convenient way if av_malloc() is to be used
			*/
			int ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height,
				c->pix_fmt, 32);
			if (ret < 0) {
				fprintf(stderr, "Could not allocate raw picture buffer\n");
				exit(6);
			}

			SwsContext* ctx = sws_getContext(c->width, c->height,
				AV_PIX_FMT_RGBA, c->width, c->height,
				AV_PIX_FMT_YUV420P, 0, 0, 0, 0);


			// encode 1 second of video
			for (i = 0; i < 25; i++) {
				av_init_packet(&pkt);
				pkt.data = NULL;    // packet data will be allocated by the encoder
				pkt.size = 0;

				fflush(stdout);

				uint8_t* pos = dataImage;

				// Y
				for (int y = 0; y < c->height; y++) {
					for (int x = 0; x < c->width; x++) {
						pos[0] = i / (float)25 * 255;
						pos[1] = 0;
						pos[2] = x / (float)(c->width) * 255;
						pos[3] = 255;
						pos += 4;
					}
				}

				uint8_t* inData[1] = { dataImage }; // RGBA32 have one plane
				int inLinesize[1] = { 4 * c->width }; // RGBA stride
				sws_scale(ctx, inData, inLinesize, 0, c->height, frame->data, frame->linesize);

				frame->pts = i;

				ret = avcodec_encode_video2(c, &pkt, frame, &got_output);
				if (ret < 0) {
					fprintf(stderr, "Error encoding frame\n");
					exit(7);
				}

				if (got_output) {
					printf("Write frame %3d (size=%5d)\n", i, pkt.size);
					fwrite(pkt.data, 1, pkt.size, file);
					av_free_packet(&pkt);
				}
			}

			/* get the delayed frames */
			for (got_output = 1; got_output; i++) {
				fflush(stdout);

				ret = avcodec_encode_video2(c, &pkt, NULL, &got_output);
				if (ret < 0) {
					fprintf(stderr, "Error encoding frame\n");
					exit(8);
				}

				if (got_output) {
					printf("Write frame %3d (size=%5d)\n", i, pkt.size);
					fwrite(pkt.data, 1, pkt.size, file);
					av_free_packet(&pkt);
				}
			}

			// add sequence end code to have a real MPEG file
			uint8_t endcode[] = { 0, 0, 1, 0xb7 };
			fwrite(endcode, 1, sizeof(endcode), file);
			fclose(file);

			avcodec_close(c);
			av_free(c);
			av_freep(&frame->data[0]);
			av_frame_free(&frame);
			printf("\n");

			//avcodec_free_context(&c);
			//////////////////////////////////////////////////////////////
            delete[] dataImage;

            timePassed = 0;
        }
    }
}