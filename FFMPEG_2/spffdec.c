/*
 * BMP image format decoder
 * Copyright (c) 2005 Mans Rullgard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <inttypes.h>

#include "avcodec.h"
#include "bytestream.h"
#include "spff.h"
#include "internal.h"
#include "msrledec.h"


/*
 * This method takes in a file in the spff file format from an AVPacket, and converts
 * the picture into image data and stores the information into a data pointer.
 * The image data is stored in the BGR8 format. 
 */
static int spff_decode_frame(AVCodecContext *avctx,
                            void *data, int *got_frame,
                            AVPacket *avpkt)
{
    //initialize member variable at the beginning of the method
    const uint8_t *data_buf = avpkt->data;
    int buf_size       = avpkt->size;
    AVFrame *image_ptr     = data;
    unsigned int file_size;
    int width, height, ret;
    uint8_t *data_ptr;
    const uint8_t *buf_start = data_buf;
    int bytes_per_line;


    //check that data buffer contains at least the header file for the spff format
    if (buf_size < SIZE_HEADER) {
        av_log(avctx, AV_LOG_ERROR, "buffer size too small (%d)\n", buf_size);
        return AVERROR_INVALIDDATA;
    }

    //make sure that the data file contains the spff indicator at the start of header
    if (bytestream_get_byte(&data_buf) != 'S' ||
        bytestream_get_byte(&data_buf) != 'P' ||
        bytestream_get_byte(&data_buf) != 'F' ||
        bytestream_get_byte(&data_buf) != 'F') {
        av_log(avctx, AV_LOG_ERROR, "bad magic number\n");
        return AVERROR_INVALIDDATA;
    }
    // get the size of the file from the header
    file_size = bytestream_get_le32(&data_buf);
    
    //check to make sure that buffer contains all picture info
    if (buf_size < file_size) {
        av_log(avctx, AV_LOG_ERROR, "not enough data (%d < %u), trying to decode anyway\n",
	       buf_size, file_size);
        file_size = buf_size;
    }

    if (file_size <= SIZE_HEADER) {
        av_log(avctx, AV_LOG_ERROR,
               "Declared file size is less than header size (%u < %u)\n",
               file_size, SIZE_HEADER);
        return AVERROR_INVALIDDATA;
    }


    //get width and height of picture from the header
    width  = bytestream_get_le32(&data_buf);
    height = bytestream_get_le32(&data_buf);

    //set codec contex to be consistent with picture format 
    avctx->width  = width;
    avctx->height = height > 0 ? height : -height;

    //set picture format to BGR8
    avctx->pix_fmt = AV_PIX_FMT_BGR8;

    //make sure memory is properly allocated
    if ((ret = ff_get_buffer(avctx, image_ptr, 0)) < 0)
        return ret;

    //set up variables for writing data to memory
    bytes_per_line = width;
    //move buffer to picture data
    data_buf   = buf_start + SIZE_HEADER;
    //point to the first frame since the data is one picture
    data_ptr   = image_ptr->data[0];
  
    //write pixel data from data into memory 
    for (int i = 0; i < avctx->height; i++) {
                memcpy(data_ptr, data_buf, bytes_per_line);
                data_buf += bytes_per_line;
                data_ptr += image_ptr->linesize[0];
     }
   
    *got_frame = 1;
    return buf_size;
}

AVCodec ff_spff_decoder = {
    .name           = "spff",
    .long_name      = NULL_IF_CONFIG_SMALL("SPFF"),
    .type           = AVMEDIA_TYPE_VIDEO,
    .id             = AV_CODEC_ID_SPFF,
    .decode         = spff_decode_frame,
    .capabilities   = AV_CODEC_CAP_DR1,
};

