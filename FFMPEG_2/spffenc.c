/*
 * BMP image format encoder
 * Copyright (c) 2006, 2007 Michel Bardiaux
 * Copyright (c) 2009 Daniel Verkamp <daniel at drv.nu>
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

#include "libavutil/imgutils.h"
#include "libavutil/avassert.h"
#include "avcodec.h"
#include "bytestream.h"
#include "spff.h"
#include "internal.h"


/*
 * This method takes in image data from an AVPacket, converts it into spff
 * format, and saves it to an AVFrame. The image must arrive in BGR8 format.
 */
static int spff_encode_frame(AVCodecContext *avctx, AVPacket *pkt,
                            const AVFrame *pict, int *got_packet)
{
    // Save a constant pointer to the image data.
    const AVFrame * const image_ptr = pict;

    // Initialize necessary variables
    int n_bytes_image, n_bytes_per_row, n_bytes, i, ret;
    uint8_t *data_ptr, *data_buf;
    
    // spff files store one byte per pixel, so each sample contains 8 bits
    avctx->bits_per_coded_sample = 8;

    // Again, one byte per pixel so storage width = pixel width
    n_bytes_per_row = ((int64_t)avctx->width);

    // Total bytes in image = number of rows * bytes per row
    n_bytes_image = avctx->height * n_bytes_per_row;
    
    // Total bytes in file == bytes in the image + bytes in the header
    n_bytes = n_bytes_image + SIZE_HEADER;

    // Ensure necessary memory
    if ((ret = ff_alloc_packet2(avctx, pkt, n_bytes, 0)) < 0)
        return ret;
    
    // Store a pointer to the start of the packet data
    data_buf = pkt->data;
    
    // Write identification bits at start of header
    bytestream_put_byte(&data_buf, 'S');                  
    bytestream_put_byte(&data_buf, 'P');                  
    bytestream_put_byte(&data_buf, 'F');                  
    bytestream_put_byte(&data_buf, 'F'); 

    // Write file size and image width/height to header
    bytestream_put_le32(&data_buf, n_bytes);               
    bytestream_put_le32(&data_buf, avctx->width);          
    bytestream_put_le32(&data_buf, avctx->height);   

    // Store a pointer to the start of memory for the file
    data_ptr = image_ptr->data[0];

    // Copy the bytes from the input buffer to the output packet
    for(i = 0; i < avctx->height; i++) {
        memcpy(data_buf, data_ptr, n_bytes_per_row);
	
	// Prgress the buffer by the number of bytes per line
        data_buf += n_bytes_per_row;

	// Progress the data pointer by the data line size
        data_ptr += image_ptr->linesize[0];
    }
    
    // Set appreopriate flags
    pkt->flags |= AV_PKT_FLAG_KEY;
    *got_packet = 1;
    return 0;
}

AVCodec ff_spff_encoder = {
    .name           = "spff",
    .long_name      = NULL_IF_CONFIG_SMALL("SPFF"),
    .type           = AVMEDIA_TYPE_VIDEO,
    .id             = AV_CODEC_ID_SPFF,
    .encode2        = spff_encode_frame,
    .pix_fmts       = (const enum AVPixelFormat[]){
        AV_PIX_FMT_BGR8,
        AV_PIX_FMT_NONE
    },
};

