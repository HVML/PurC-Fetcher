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

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct pcfetcher_encoder;
struct pcfetcher_decoder;

struct pcfetcher_msg {
    uint8_t flags;
    uint16_t name;
    uint64_t dest_id;
};

// bool, char, int, float, double (signed, unsigned)
#define pcfetcher_encoder_encode_basic(encoder, v) \
    pcfetcher_encoder_encode_data(encoder, (const uint8_t*)&v, sizeof(v))
#define pcfetcher_decoder_decode_basic(decoder, v) \
    pcfetcher_decoder_decode_data(decoder, (uint8_t*)&v, sizeof(v))

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct pcfetcher_encoder* pcfetcher_encoder_create(void);
void pcfetcher_encoder_destroy(struct pcfetcher_encoder* encoder);
const uint8_t* pcfetcher_encoder_get_buffer(struct pcfetcher_encoder* encoder,
        size_t* size);

struct pcfetcher_decoder* pcfetcher_decoder_create(const uint8_t* buffer,
        size_t size, bool free_buffer);
void pcfetcher_decoder_destroy(struct pcfetcher_decoder* encoder);

void pcfetcher_encoder_encode_data(struct pcfetcher_encoder* encoder,
        const uint8_t* data, size_t size);

bool pcfetcher_decoder_decode_data(struct pcfetcher_decoder* decoder,
        uint8_t* data, size_t size);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* not defined PURC_FETCHER_MESSAGE_H */


