/*
 * @file fetcher-internal.h
 * @author XueShuming
 * @date 2021/11/16
 * @brief The interfaces for fetcher internal.
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

#ifndef PURC_FETCHER_INTERNAL_H
#define PURC_FETCHER_INTERNAL_H

#include "fetcher.h"

struct pcfetcher;

typedef struct pcfetcher* (*pcfetcher_init_fn)(size_t max_conns,
        size_t cache_quota);

typedef int (*pcfetcher_term_fn)(struct pcfetcher* fetcher);

typedef const char* (*pcfetcher_set_base_url_fn)(struct pcfetcher* fetcher,
        const char* base_url);

typedef void (*pcfetcher_cookie_set_fn)(struct pcfetcher* fetcher,
        const char* domain, const char* path, const char* name,
        const char* content, time_t expire_time, bool secure);

typedef const char* (*pcfetcher_cookie_get_fn)(struct pcfetcher* fetcher,
        const char* domain, const char* path, const char* name,
        time_t *expire, bool *secure);

typedef const char* (*pcfetcher_cookie_remove_fn)(struct pcfetcher* fetcher,
        const char* domain, const char* path, const char* name);

typedef purc_variant_t (*pcfetcher_request_async_fn)(
        struct pcfetcher* fetcher,
        const char* url,
        enum pcfetcher_request_method method,
        purc_variant_t params,
        uint32_t timeout,
        response_handler handler,
        void* ctxt);

typedef purc_rwstream_t (*pcfetcher_request_sync_fn)(
        struct pcfetcher* fetcher,
        const char* url,
        enum pcfetcher_request_method method,
        purc_variant_t params,
        uint32_t timeout,
        struct pcfetcher_resp_header *resp_header);

typedef int (*pcfetcher_check_response_fn)(struct pcfetcher* fetcher,
        uint32_t timeout_ms);

struct pcfetcher {
    size_t max_conns;
    size_t cache_quota;

    pcfetcher_init_fn init;
    pcfetcher_term_fn term;
    pcfetcher_set_base_url_fn set_base_url;
    pcfetcher_cookie_set_fn cookie_set;
    pcfetcher_cookie_get_fn cookie_get;
    pcfetcher_cookie_remove_fn cookie_remove;
    pcfetcher_request_async_fn request_async;
    pcfetcher_request_sync_fn request_sync;
    pcfetcher_check_response_fn check_response;
};

#endif /* not defined PURC_FETCHER_INTERNAL_H */

