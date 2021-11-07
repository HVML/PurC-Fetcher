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

struct pcfetcher_encoder;
struct pcfetcher_decoder;

struct pcfetcher_msg {
    uint8_t flags;
    uint16_t name;
    uint64_t dest_id;
};


struct pcfetcher_encoder* encode_data(const uint8_t* data, size_t size);

#define encode_bool(v) encode_data((const uint8_t*)&v, sizeof(bool))


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* not defined PURC_FETCHER_MESSAGE_H */


