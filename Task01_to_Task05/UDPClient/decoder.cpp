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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decoder.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libavutil/common.h"
#include "libavutil/mathematics.h"
}

#define INBUF_SIZE 4096

void Decoder::pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
                    char *filename){
   FILE *f=fopen(filename,"w");
   fprintf(f,"P5\n%d %d\n%d\n",xsize,ysize,255);
   for(int i=0;i<ysize;i++)
       fwrite(buf + i * wrap,1,xsize,f);
       fclose(f);
   }

void Decoder::decode(char *filename){

    char outname[200];
    const char *outfilename = "out";
    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];

    AVPacket *pkt = av_packet_alloc();
     if (!pkt){
         exit(1);   
    }

    // set end of buffer to 0 (this ensures that no overreading happens for damaged MPEG streams)
     memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);

     const AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_MPEG2VIDEO);//!!!!change to mep2???
     if (!codec) {
         fprintf(stderr, "codec not found\n");
         exit(1);
     }

     AVCodecParserContext *parser = av_parser_init(codec->id);
     if (!parser) {
         fprintf(stderr, "parser not found\n");
         exit(1);
     }

    AVCodecContext *c = avcodec_alloc_context3(codec);
    AVFrame *picture = av_frame_alloc();

    // open it
     if (avcodec_open2(c, codec, NULL) < 0) {
         fprintf(stderr, "could not open codec\n");
         exit(1);
     }

     FILE *f = fopen(filename, "rb");
     if (!f) {
         fprintf(stderr, "could not open %s\n", filename);
         exit(1);
     }

     while (!feof(f)) {
     // read raw data from the input file
         size_t data_size = fread(inbuf, 1, INBUF_SIZE, f);
         if (!data_size)
             break;

         // use the parser to split the data into frames
         uint8_t *data = inbuf;
         while (data_size > 0) {
             int ret = av_parser_parse2(parser, c, &pkt->data, &pkt->size,
                                        data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
             if (ret < 0) {
                 fprintf(stderr, "Error while parsing\n");
                 exit(1);
             }
             data += ret;
             data_size -= ret;

             if (pkt->size) {
                 sprintf(outname,"%s%d.pgm", outfilename, num_frame++);
                 printf("Outname %s\n", outname);
                 decode(c, picture, pkt, outname);
             }
         }
     }

     // Flush the decoder
     sprintf(outname,"%s%d.pgm", outfilename, num_frame++);
     printf("Outname %s\n", outname);
     decode(c, picture, NULL, outname);

     fclose(f);

     av_parser_close(parser);
     avcodec_free_context(&c);
     av_frame_free(&picture);
     av_packet_free(&pkt);
}


void Decoder::decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt,
                    const char *filename)
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
         pgm_save(frame->data[0], frame->linesize[0],
                  frame->width, frame->height, buf);
     }
  }