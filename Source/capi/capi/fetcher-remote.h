/*
 * @file fetcher-remote.h
 * @author XueShuming
 * @date 2021/11/16
 * @brief The interfaces for fetcher remote.
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

#ifndef PURC_FETCHER_REMOTE_H
#define PURC_FETCHER_REMOTE_H

#if ENABLE(LINK_PURC_FETCHER)

#include "purc/purc.h"
#include "fetcher-internal.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


struct pcfetcher* pcfetcher_remote_init(size_t max_conns, size_t cache_quota);

int pcfetcher_remote_term(struct pcfetcher* fetcher);

const char* pcfetcher_remote_set_base_url(struct pcfetcher* fetcher,
        const char* base_url);

void pcfetcher_cookie_remote_set(struct pcfetcher* fetcher,
        const char* domain, const char* path, const char* name,
        const char* content, time_t expire_time, bool secure);

const char* pcfetcher_cookie_remote_get(struct pcfetcher* fetcher,
        const char* domain, const char* path, const char* name,
        time_t *expire, bool *secure);

const char* pcfetcher_cookie_remote_remove(struct pcfetcher* fetcher,
        const char* domain, const char* path, const char* name);


purc_variant_t pcfetcher_remote_request_async(
        struct pcfetcher* fetcher,
        const char* url,
        enum pcfetcher_request_method method,
        purc_variant_t params,
        uint32_t timeout,
        response_handler handler,
        void* ctxt);

purc_rwstream_t pcfetcher_remote_request_sync(
        struct pcfetcher* fetcher,
        const char* url,
        enum pcfetcher_request_method method,
        purc_variant_t params,
        uint32_t timeout,
        struct pcfetcher_resp_header *resp_header);

int pcfetcher_remote_check_response(struct pcfetcher* fetcher,
        uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif // ENABLE(LINK_PURC_FETCHER)

#endif /* not defined PURC_FETCHER_REMOTE_H */


