#include "MPGDecoder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#pragma comment(lib,"ws2_32.lib")

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libavutil/common.h"
#include "libavutil/mathematics.h"
#include "libavutil/avassert.h"
#include "libavutil/crc.h"
}

#define INBUF_SIZE 4096


void MPGDecoder::png_save(unsigned char* buf, int wrap, int xsize, int ysize, char* filename)
{
    FILE* f = fopen(filename, "wb");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    for (int i = 0; i < ysize; i++){
        fwrite(buf + i * wrap, 1, xsize, f);
        // unsigned char* ch = (buf + i * wrap);
        //ProcessArray(ch, xsize);
        // fwrite(ch, 1, xsize * 3, f);
    }
    fclose(f);
}

void MPGDecoder::decodeFrame(AVCodecContext* dec_ctx, AVFrame* frame, AVPacket* pkt, const char* filename)
{
    char buf[1024];

    int ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0) {
        fprintf(stderr, "Error sending a packet for decoding\n");
        exit(1);
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

        // the picture is allocated by the decoder. no need to
        //      free it
        snprintf(buf, sizeof(buf), filename, dec_ctx->frame_number);
        //png_save(frame->data[0], frame->linesize[0],
            //frame->width, frame->height, buf);
    }
}

void MPGDecoder::decode(uint8_t inbuf[], size_t data_size) {
    const char* outfilename = "out";
    char outname[200];

    avcodec_register_all();

    AVPacket* pkt = av_packet_alloc();
    if (!pkt)
        exit(1);

    // set end of buffer to 0 (this ensures that no overreading happens for damaged MPEG streams)
    memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);

    const AVCodec* codec = avcodec_find_decoder(AV_CODEC_ID_MPEG2VIDEO);
    if (!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }

    AVCodecParserContext* parser = av_parser_init(codec->id);
    if (!parser) {
        fprintf(stderr, "parser not found\n");
        exit(1);
    }

    AVCodecContext* ctx = avcodec_alloc_context3(codec);
    ctx->height = 600;
    ctx->width = 800;
    ctx->pix_fmt = AV_PIX_FMT_YUV420P;

    AVFrame* picture = av_frame_alloc();
    picture->height = 600;
    picture->width = 800;

    // For some codecs, such as msmpeg4 and mpeg4, width and height
    //    MUST be initialized there because this information is not
    //    available in the bitstream.

        // open it
    if (avcodec_open2(ctx, codec, NULL) < 0) {
        fprintf(stderr, "could not open codec\n");
        exit(1);
    }

    

    // use the parser to split the data into frames
    uint8_t* data = inbuf;
    /*
    while (data_size > 0) {
        printf("data size was in while %d", data_size);

        int ret = av_parser_parse2(parser, ctx, &pkt->data, &pkt->size,
            data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
        if (ret < 0) {
            fprintf(stderr, "Error while parsing\n");
            exit(1);
        }
        data += ret;
        data_size -= ret;
        
        printf("data is %d", *inbuf);
        printf("pkt->size was %d", pkt->size);
        if (pkt->size) {
            printf("pkt->size was %d", pkt->size);
            sprintf(outname, "%s%d.pgm", outfilename, num_frame++);
            printf("Outname %s\n", outname);
            decodeFrame(ctx, picture, pkt, outname);
        }
    }
    */

    pkt->data = inbuf;
    pkt->size = data_size;

    if (data_size) {
        printf("pkt->size was %d", pkt->size);
        sprintf(outname, "%s%d.png", outfilename, num_frame++);
        printf("Outname %s\n", outname);
        decodeFrame(ctx, picture, pkt, outname);
    }
   
    // flush the decoder
    sprintf(outname, "%s%d.png", outfilename, num_frame++);
    printf("Outname %s\n", outname);
    decodeFrame(ctx, picture, NULL, outname);

    av_parser_close(parser);
    avcodec_free_context(&ctx);
    av_frame_free(&picture);
    av_packet_free(&pkt);

}

