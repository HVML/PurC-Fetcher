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

#if ENABLE(LINK_PURC_FETCHER)
#include "fetcher-remote.h"
#else
#include "fetcher-local.h"
#endif


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

#if ENABLE(LINK_PURC_FETCHER)
    s_pcfetcher->init = pcfetcher_remote_init;
    s_pcfetcher->term = pcfetcher_remote_term;
    s_pcfetcher->set_base_url = pcfetcher_remote_set_base_url;
    s_pcfetcher->cookie_set = pcfetcher_cookie_remote_set;
    s_pcfetcher->cookie_get = pcfetcher_cookie_remote_get;
    s_pcfetcher->cookie_remove = pcfetcher_cookie_remote_remove;
    s_pcfetcher->request_async = pcfetcher_remote_request_async;
    s_pcfetcher->request_sync = pcfetcher_remote_request_sync;
    s_pcfetcher->check_response = pcfetcher_remote_check_response;
#else
    s_pcfetcher->init = pcfetcher_local_init;
    s_pcfetcher->term = pcfetcher_local_term;
    s_pcfetcher->set_base_url = pcfetcher_local_set_base_url;
    s_pcfetcher->cookie_set = pcfetcher_cookie_loccal_set;
    s_pcfetcher->cookie_get = pcfetcher_cookie_loccal_get;
    s_pcfetcher->cookie_remove = pcfetcher_cookie_loccal_remove;
    s_pcfetcher->request_async = pcfetcher_local_request_async;
    s_pcfetcher->request_sync = pcfetcher_local_request_sync;
    s_pcfetcher->check_response = pcfetcher_local_check_response;
#endif

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

const char* pcfetcher_set_base_url(const char* base_url)
{
    return s_pcfetcher ? s_pcfetcher->set_base_url(s_pcfetcher, base_url) : NULL;
}

void pcfetcher_cookie_set(const char* domain,
        const char* path, const char* name, const char* content,
        time_t expire_time, bool secure)
{
    if (s_pcfetcher) {
        s_pcfetcher->cookie_set(s_pcfetcher, domain, path, name, content,
                expire_time, secure);
    }
}

const char* pcfetcher_cookie_get(const char* domain,
        const char* path, const char* name, time_t *expire, bool *secure)
{
    return s_pcfetcher ? s_pcfetcher->cookie_get(s_pcfetcher, domain, path,
            name, expire, secure) : NULL;
}

const char* pcfetcher_cookie_remove(const char* domain,
        const char* path, const char* name)
{
    if (s_pcfetcher) {
        return s_pcfetcher->cookie_remove(s_pcfetcher, domain, path, name);
    }
    return NULL;
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


int pcfetcher_check_response(uint32_t timeout_ms)
{
    return s_pcfetcher ? s_pcfetcher->check_response(s_pcfetcher,
            timeout_ms) : 0;
}



