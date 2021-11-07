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

#include <stdio.h>

struct pcfetcher_encoder
{
};

struct pcfetcher_decoder
{
};

struct pcfetcher_encoder* encode_data(const uint8_t* data, size_t size)
{
    fprintf(stderr, "..........................data=%s|size=%ld\n", data, size);
    return NULL;
}



