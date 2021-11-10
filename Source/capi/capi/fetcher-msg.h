/*
 * @file fetcher-msg.h
 * @author XueShuming
 * @date 2021/11/07
 * @brief The interfaces for fetcher message.
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

#ifndef PURC_FETCHER_MESSAGE_H
#define PURC_FETCHER_MESSAGE_H

#include "purc/purc.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// bool, char, int, float, double (signed, unsigned)
#define pcfetcher_basic_encode(encoder, v) \
    pcfetcher_encode_data(encoder, (const uint8_t*)&v,\
            sizeof(v), sizeof(v))
#define pcfetcher_basic_decode(decoder, v) \
    pcfetcher_decode_data(decoder, (uint8_t*)&v, sizeof(v), sizeof(v))

struct pcfetcher_encoder;
struct pcfetcher_decoder;

struct pcfetcher_msg_header {
    uint8_t flags;
    uint16_t name;
    uint64_t dest_id;
};

// base type
struct pcfetcher_string {
    uint32_t length;
    bool is_8bit;
    uint8_t* buffer;
};

struct pcfetcher_data_reference {
    const uint8_t* data;
    size_t size;
};

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

typedef void (*PCFETCHER_ENCODE_FUNC)(struct pcfetcher_encoder*, void*);
typedef bool (*PCFETCHER_DECODE_FUNC)(struct pcfetcher_decoder*, void**);
typedef struct pcutils_arrlist* (*PCFETCHER_ARRAY_CREATE_FUNC)(void);

struct pcfetcher_encoder* pcfetcher_encoder_create(void);
void pcfetcher_encoder_destroy(struct pcfetcher_encoder* encoder);
const uint8_t* pcfetcher_encoder_get_buffer(struct pcfetcher_encoder* encoder,
        size_t* size);

struct pcfetcher_decoder* pcfetcher_decoder_create(const uint8_t* buffer,
        size_t size, bool free_buffer);
void pcfetcher_decoder_destroy(struct pcfetcher_decoder* encoder);

void pcfetcher_encode_data(struct pcfetcher_encoder* encoder,
        const uint8_t* data, size_t size, size_t alignment);

bool pcfetcher_decode_data(struct pcfetcher_decoder* decoder,
        uint8_t* data, size_t size, size_t alignment);

// array
struct pcutils_arrlist* pcfetcher_array_create(array_list_free_fn *free_fn);

void pcfetcher_array_destroy(struct pcutils_arrlist*);

void pcfetcher_array_encode(struct pcfetcher_encoder* encoder,
        struct pcutils_arrlist* array,
        PCFETCHER_ENCODE_FUNC func, uint8_t header_bytes);

void pcfetcher_array_decode(struct pcfetcher_decoder* decoder,
        struct pcutils_arrlist** array,
        PCFETCHER_ARRAY_CREATE_FUNC creator,
        PCFETCHER_DECODE_FUNC func, uint8_t header_bytes);

// msg header
void pcfetcher_msg_header_encode(struct pcfetcher_encoder* encoder,
        struct pcfetcher_msg_header* s);

bool pcfetcher_msg_header_decode(struct pcfetcher_decoder* decoder,
        struct pcfetcher_msg_header* s);

// base type
struct pcfetcher_string* pcfetcher_string_create(void);
void pcfetcher_string_destroy(struct pcfetcher_string* s);
void pcfetcher_string_encode(struct pcfetcher_encoder* encoder, void* s);
bool pcfetcher_string_decode(struct pcfetcher_decoder* decoder, void** s);

static inline void pcfetcher_string_array_free_fn(void* v)
{
    pcfetcher_string_destroy((struct pcfetcher_string*)v);
}

static inline struct pcutils_arrlist* pcfetcher_string_array_create(void)
{
    return pcfetcher_array_create(pcfetcher_string_array_free_fn);
}

static inline void pcfetcher_string_array_destroy(struct pcutils_arrlist* array)
{
    pcfetcher_array_destroy(array);
}

static inline void pcfetcher_string_array_encode(
        struct pcfetcher_encoder* encoder,
        struct pcutils_arrlist* array,
        uint8_t header_bytes)
{
    pcfetcher_array_encode(encoder, array, pcfetcher_string_encode,
            header_bytes);
}

static inline void pcfetcher_string_array_decode(
        struct pcfetcher_decoder* decoder,
        struct pcutils_arrlist** array,
        uint8_t header_bytes)
{
    pcfetcher_array_decode(decoder, array, pcfetcher_string_array_create,
            pcfetcher_string_decode, header_bytes);
}


struct pcfetcher_data_reference* pcfetcher_data_reference_create(void);
void pcfetcher_data_reference_destroy(struct pcfetcher_data_reference* s);
void pcfetcher_data_reference_encode(struct pcfetcher_encoder* encoder, void* s);
bool pcfetcher_data_reference_decode(struct pcfetcher_decoder* decoder, void** s);

static inline void pcfetcher_data_reference_array_free_fn(void* v)
{
    pcfetcher_data_reference_destroy((struct pcfetcher_data_reference*)v);
}

static inline struct pcutils_arrlist* pcfetcher_data_reference_array_create(void)
{
    return pcfetcher_array_create(pcfetcher_data_reference_array_free_fn);
}

static inline void pcfetcher_data_reference_array_destroy(struct pcutils_arrlist* array)
{
    pcfetcher_array_destroy(array);
}

static inline void pcfetcher_data_reference_array_encode_ex(
        struct pcfetcher_encoder* encoder, struct pcutils_arrlist* array,
        uint8_t header_bytes)
{
    pcfetcher_array_encode(encoder, array, pcfetcher_data_reference_encode,
            header_bytes);
}

static inline void pcfetcher_data_reference_array_decode_ex(
        struct pcfetcher_decoder* decoder, struct pcutils_arrlist** array,
        uint8_t header_bytes)
{
    pcfetcher_array_decode(decoder, array,
            pcfetcher_data_reference_array_create,
            pcfetcher_data_reference_decode, header_bytes);
}


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* not defined PURC_FETCHER_MESSAGE_H */


