#include "VEInclude.h"

namespace ve {
	void encode(AVCodecContext* enc_ctx, AVFrame* frame, AVPacket* pkt, FILE* outfile)
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
			av_packet_unref(pkt);
		}
	}

	void VEEventListenerFrame::onFrameEnded(veEvent event) {
		totalTime += event.dt;
		if (totalTime >= 0.033) {
			encodeFrame = true;
			totalTime = 0;
		}

		if(encodeFrame){
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
	
			encoder.encode(dataImage, extent.width, extent.height);

			encodeFrame = false;
			delete[] dataImage;
			
		}
			
	};

}