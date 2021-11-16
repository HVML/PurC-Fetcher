/*
 * @file fetcher.c
 * @author XueShuming
 * @date 2021/11/16
 * @brief The impl for fetcher api.
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

#include "config.h"

#include "fetcher.h"
#include "fetcher-internal.h"

#include <wtf/RunLoop.h>

static struct pcfetcher* s_pcfetcher;

int pcfetcher_init(size_t max_conns, size_t cache_quota)
{
    if (s_pcfetcher) {
        return 0;
    }
    s_pcfetcher = (struct pcfetcher*)malloc(sizeof(struct pcfetcher));
    s_pcfetcher->max_conns = max_conns;
    s_pcfetcher->cache_quota = cache_quota;

// TODO


    s_pcfetcher->init(s_pcfetcher, max_conns, cache_quota);
    return 0;
}

int pcfetcher_term(void)
{
    if (!s_pcfetcher) {
        return 0;
    }

    int ret = s_pcfetcher->term(s_pcfetcher);

    free(s_pcfetcher);
    s_pcfetcher = NULL;
    return ret;
}

//pcfetcher_connid pcfetcher_get_connid(void);

void pcfetcher_set_cookie(const char* url, const char* cookie, double expires,
        bool secure)
{
    if (s_pcfetcher) {
        s_pcfetcher->set_cookie(s_pcfetcher, url, cookie, expires, secure);
    }
}

const char* pcfetcher_get_cookie(const char* url)
{
    return s_pcfetcher ? s_pcfetcher->get_cookie(s_pcfetcher, url) : NULL;
}

void pcfetcher_remove_cookie(const char* url)
{
    if (s_pcfetcher) {
        s_pcfetcher->remove_cookie(s_pcfetcher, url);
    }
}

purc_variant_t pcfetcher_request_async(
        const char* url,
        enum pcfetcher_request_method method,
        purc_variant_t params,
        uint32_t timeout,
        response_handler handler,
        void* ctxt)
{
    return s_pcfetcher ? s_pcfetcher->request_async(s_pcfetcher, url, method,
            params, timeout, handler, ctxt) : PURC_VARIANT_INVALID;
}

purc_rwstream_t pcfetcher_request_sync(
        const char* url,
        enum pcfetcher_request_method method,
        purc_variant_t params,
        uint32_t timeout,
        struct pcfetcher_resp_header *resp_header)
{
    return s_pcfetcher ? s_pcfetcher->request_sync(s_pcfetcher, url, method,
            params, timeout, resp_header) : NULL;
}


int pcfetcher_check_response(void)
{
    return s_pcfetcher ? s_pcfetcher->check_response(s_pcfetcher) : 0;
}



