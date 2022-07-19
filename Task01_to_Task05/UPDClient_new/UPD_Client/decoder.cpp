/*
    2  * copyright (c) 2001 Fabrice Bellard
    3  *
    4  * This file is part of Libav.
    5  *
    6  * Libav is free software; you can redistribute it and/or
    7  * modify it under the terms of the GNU Lesser General Public
    8  * License as published by the Free Software Foundation; either
    9  * version 2.1 of the License, or (at your option) any later version.
   10  *
   11  * Libav is distributed in the hope that it will be useful,
   12  * but WITHOUT ANY WARRANTY; without even the implied warranty of
   13  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   14  * Lesser General Public License for more details.
   15  *
   16  * You should have received a copy of the GNU Lesser General Public
   17  * License along with Libav; if not, write to the Free Software
   18  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
   19  */

#define _CRT_SECURE_NO_DEPRECATE
#define INBUF_SIZE 4096

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h";

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libavutil/common.h"
#include "libavutil/mathematics.h"
#include <libswscale/swscale.h>
}

#include <opencv2/highgui.hpp>


#include "decoder.h"

Decoder::Decoder() {
    codec = avcodec_find_decoder(AV_CODEC_ID_MPEG2VIDEO);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }

    // Allocate an AVCodecContext and set its fields to default values.
    ctx = avcodec_alloc_context3(codec);
    if (!ctx) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(2);
    }
    ctx->bit_rate = 15000000; // 40000
    ctx->height = 600;
    ctx->width = 800;
    ctx->time_base.num = 1;
    ctx->time_base.den = 60; // 25
    ctx->framerate.num = 60; // 25
    ctx->framerate.den = 1;
    ctx->gop_size = 15; // Emit one intra frame every ten frames.
    ctx->max_b_frames = 5; // 1
    ctx->pix_fmt = AV_PIX_FMT_RGBA;
    ctx->flags |= AV_CODEC_FLAG_TRUNCATED;

    // Initialize the AVCodecContext to use the given AVCodec (open it)
    // For some codecs, such as msmpeg4 and mpeg4, width and height
    // MUST be initialized there because this information is not
    // available in the bitstream.
    // open it
    if (avcodec_open2(ctx, codec, NULL) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }

    parser = av_parser_init(codec->id);
    if (!parser) {
        fprintf(stderr, "parser not found\n");
        exit(1);
    }
    parser->flags = PARSER_FLAG_COMPLETE_FRAMES;

    // Allocate an AVPacket and set its fields to default values.
    pkt = av_packet_alloc();
    if (!pkt) {
        fprintf(stderr, "Can not alloc packet\n");
        exit(1);
    }

    // Allocate an AVFrame and set its fields to default values.
    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(5);
    }
    frame->format = ctx->pix_fmt;
    frame->height = ctx->height;
    frame->width = ctx->width;

    // Allocate new buffer(s) for audio or video data.
    if (av_frame_get_buffer(frame, 32) < 0) {
        fprintf(stderr, "could not alloc the frame data\n");
        exit(1);
    }

    // Make sure the frame data is writable.
    if (av_frame_make_writable(frame) < 0) {
        fprintf(stderr, "Cannot make frame writeable\n");
        exit(1);
    }
}


Decoder::~Decoder() {
    // Free all the stuff.
    avcodec_free_context(&ctx);
    av_parser_close(parser);
    av_frame_free(&frame);
    av_packet_free(&pkt);
}


void Decoder::decode(uint8_t* data, int data_size) {

    if (data_size > 0) {
        int ret = av_parser_parse2(parser, ctx, &pkt->data, &pkt->size, (uint8_t*)data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
        decode(ctx, frame, pkt, outfilename);
    }

}


void Decoder::decode(AVCodecContext* dec_ctx, AVFrame* frame, AVPacket* pkt, const char* filename) {

    int ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0) {
        fprintf(stderr, "Error sending a packet for decoding\n");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        }
        else if (ret < 0) {
            fprintf(stderr, "Error during decoding\n");
            exit(1);
        }

        SwsContext* sws_ctx = sws_getContext(frame->width, frame->height, dec_ctx->pix_fmt, frame->width, frame->height, AV_PIX_FMT_RGBA, SWS_BILINEAR, 0, 0, 0);
        if (!sws_ctx) {
            printf("Could not initialize sw scaler\n");
            exit(1);
        }


        //uint8_t* outData[1] = {(uint8_t*)buf};
        //int outLinesize[1] = { 4 * dec_ctx->width }; // RGBA stride
        //sws_scale(sws_ctx, frame->data, frame->linesize, 0, frame->height, outData, outLinesize);

        /*uint8_t* inData[1] = {frame->data[0]}; // RGBA32 have one plane
        int inLinesize[1] = { 4 * dec_ctx->width }; // RGBA stride
        sws_scale(sws_ctx, inData, inLinesize, 0, dec_ctx->height, frame->data, frame->linesize);*/


        printf("saving frame %3d\n", dec_ctx->frame_number);
        fflush(stdout);

        //png_save(frame->data[0], dec_ctx->width, dec_ctx->height, "D:/2022SS/2022SS_CGA/Task01_to_Task05/UPDClient_new/UPD_Client/video.jpg");
        pgm_save(frame->data[0], frame->linesize[0], frame->width, frame->height, filename);


        //std::string path = "D:/2022SS/2022SS_CGA/Task01_to_Task05/UPDClient_new/UPD_Client/";
        cv::Mat image = cv::imread(filename);

        if (image.empty()) {
            puts("Image File not found");
            return;
        }
        imshow("Is that a cockroach I see?...", image);
        cv::waitKey(1);
    }
}


void Decoder::pgm_save(unsigned char* buf, int wrap, int xsize, int ysize, const char* filename) {
    FILE* f = fopen(filename, "w");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    for (int i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}

void Decoder::png_save(uint8_t* data, int width, int height, const char* filename) {
    stbi_write_jpg(filename, width, height, 4, data, 4 * width);
}

