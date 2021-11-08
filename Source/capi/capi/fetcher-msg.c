/*
 * @file fetcher-msg.c
 * @author XueShuming
 * @date 2021/11/07
 * @brief The impl for fetcher message.
 *
 * Copyright (C) 2021 FMSoft <https://www.fmsoft.cn>
 *
 * This file is a part of PurC (short for Purring Cat), an HVML interpreter.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "fetcher-msg.h"
#include "purc/purc-arraylist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_BUFFER_SIZE 512

struct pcfetcher_encoder
{
    uint8_t* buffer;
    uint8_t* buffer_pos;
    size_t buffer_size;
    size_t buffer_capacity;
};

struct pcfetcher_decoder
{
    bool free_buffer;
    const uint8_t* buffer;
    const uint8_t* buffer_pos;
    const uint8_t* buffer_end;
};

struct pcfetcher_encoder* pcfetcher_encoder_create()
{
    struct pcfetcher_encoder* encoder = (struct pcfetcher_encoder*)malloc(
            sizeof(struct pcfetcher_encoder));
    encoder->buffer = (uint8_t*)calloc(INIT_BUFFER_SIZE, 1);
    encoder->buffer_pos = encoder->buffer;
    encoder->buffer_size = 0;
    encoder->buffer_capacity = INIT_BUFFER_SIZE;
    return encoder;
}

void pcfetcher_encoder_destroy(struct pcfetcher_encoder* encoder)
{
    if (encoder) {
        free(encoder->buffer);
        free(encoder);
    }
}

const uint8_t* pcfetcher_encoder_get_buffer(struct pcfetcher_encoder* encoder,
        size_t* size)
{
    if (size) {
        *size = encoder->buffer_size;
    }
    return encoder->buffer;
}

static inline size_t round_up_to_alignment(size_t value, size_t alignment)
{
    return ((value + alignment - 1) / alignment) * alignment;
}

uint8_t* pcfetcher_encode_grow_buffer(struct pcfetcher_encoder* encoder,
        size_t size, size_t alignment)
{
    size_t align_size = round_up_to_alignment(encoder->buffer_size, alignment);
    size_t sz = align_size + size;
    if (sz >= encoder->buffer_capacity) {
        // TODO calc new_size fibonacci
        size_t new_size = sz + 10;
        // grow buffer
        uint8_t* newbuf = (uint8_t*) realloc(encoder->buffer, new_size + 1);
        encoder->buffer = newbuf;
        encoder->buffer_capacity = new_size;
    }
    encoder->buffer_pos = encoder->buffer + sz;
    encoder->buffer_size = sz;
    return encoder->buffer + align_size;
}

struct pcfetcher_decoder* pcfetcher_decoder_create(const uint8_t* buffer,
        size_t size, bool free_buffer)
{
    struct pcfetcher_decoder* decoder = (struct pcfetcher_decoder*)malloc(
            sizeof(struct pcfetcher_decoder));
    decoder->free_buffer = free_buffer;
    decoder->buffer = buffer;
    decoder->buffer_pos = buffer;
    decoder->buffer_end = buffer + size;
    return decoder;
}

void pcfetcher_decoder_destroy(struct pcfetcher_decoder* decoder)
{
    if (decoder) {
        if (decoder->free_buffer) {
            free((uint8_t*)(decoder->buffer));
        }
        free(decoder);
    }
}

void pcfetcher_encode_data(struct pcfetcher_encoder* encoder,
        const uint8_t* data, size_t size, size_t alignment)
{
    uint8_t* buffer = pcfetcher_encode_grow_buffer(encoder, size, alignment);
    memcpy(buffer, data, size);
}

static inline const uint8_t* decoder_round_up_to_alignment(const uint8_t* ptr,
        size_t alignment)
{
    uintptr_t alignment_mask = alignment - 1;
    return (const uint8_t*)(
            (((uintptr_t)ptr) + alignment_mask) & ~alignment_mask
            );
}

bool pcfetcher_decode_data(struct pcfetcher_decoder* decoder,
        uint8_t* data, size_t size, size_t alignment)
{
    decoder->buffer_pos = decoder_round_up_to_alignment(decoder->buffer_pos,
            alignment);
    if (decoder->buffer_pos + size > decoder->buffer_end) {
        return false;
    }
    memcpy(data, decoder->buffer_pos, size);
    decoder->buffer_pos += size;
    return true;
}

void pcfetcher_encode_msg_header(struct pcfetcher_encoder* encoder,
        struct pcfetcher_msg_header* s)
{
    pcfetcher_encode_basic(encoder, s->flags);
    pcfetcher_encode_basic(encoder, s->name);
    pcfetcher_encode_basic(encoder, s->dest_id);
}

bool pcfetcher_decode_msg_header(struct pcfetcher_decoder* decoder,
        struct pcfetcher_msg_header* s)
{
    pcfetcher_decode_basic(decoder, s->flags);
    pcfetcher_decode_basic(decoder, s->name);
    pcfetcher_decode_basic(decoder, s->dest_id);
    return true;
}

void pcfetcher_encode_string(struct pcfetcher_encoder* encoder,
        struct pcfetcher_string* s)
{
    if (encoder->buffer == NULL || s->length == 0) {
        uint32_t length = UINT32_MAX;
        pcfetcher_encode_basic(encoder, length);
        return;
    }

    pcfetcher_encode_basic(encoder, s->length);
    pcfetcher_encode_basic(encoder, s->is_8bit);
    if (s->is_8bit) {
        pcfetcher_encode_data(encoder, s->buffer, s->length, 1);
    }
    else {
        pcfetcher_encode_data(encoder, s->buffer, s->length * 2, 1);
    }
}

bool pcfetcher_decode_string(struct pcfetcher_decoder* decoder,
        struct pcfetcher_string** str)
{
    uint32_t length = 0;
    pcfetcher_decode_basic(decoder, length);
    if (length == UINT32_MAX) {
        *str= NULL;
        return true;
    }
    struct pcfetcher_string *s = (struct pcfetcher_string*) malloc(
            sizeof(struct pcfetcher_string));
    s->length = length;
    pcfetcher_decode_basic(decoder, s->is_8bit);
    if (s->is_8bit) {
        s->buffer = (uint8_t*) malloc(s->length + 1);
        pcfetcher_decode_data(decoder, s->buffer, s->length, 1);
    }
    else {
        s->buffer = (uint8_t*) malloc(s->length*2 + 1);
        pcfetcher_decode_data(decoder, s->buffer, s->length * 2, 1);
    }
    *str = s;
    return true;
}

void pcfetcher_destory_string(struct pcfetcher_string* s)
{
    if (s) {
        if (s->buffer) {
            free(s->buffer);
        }
        free(s);
    }
}

