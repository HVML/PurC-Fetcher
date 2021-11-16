/*
 * @file fetcher.h
 * @author XueShuming
 * @date 2021/11/16
 * @brief The interfaces for fetcher.
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

#ifndef PURC_FETCHER_H
#define PURC_FETCHER_H

#include "purc/purc.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


//typedef int pcfetcher_connid;

enum pcfetcher_request_method {
    PCFETCHER_REQUEST_METHOD_GET = 0,
    PCFETCHER_REQUEST_METHOD_POST,
    PCFETCHER_REQUEST_METHOD_DELETE,
};


struct pcfetcher_resp_header {
    int ret_code;
    char* mime_type;
    size_t sz_resp;
};

typedef void (*response_handler)(
        purc_variant_t request_id, void* ctxt,
        const struct pcfetcher_resp_header *resp_header,
        purc_rwstream_t resp);


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

int pcfetcher_init(size_t max_conns, size_t cache_quota);

int pcfetcher_term(void);

//pcfetcher_connid pcfetcher_get_connid(void);

void pcfetcher_set_cookie(const char* url, const char* cookie, double expires,
        bool secure);
const char* pcfetcher_get_cookie(const char* url);
void pcfetcher_remove_cookie(const char* url);

purc_variant_t pcfetcher_request_async(
        const char* url,
        enum pcfetcher_request_method method,
        purc_variant_t params,
        uint32_t timeout,
        response_handler handler,
        void* ctxt);

purc_rwstream_t pcfetcher_request_sync(
        const char* url,
        enum pcfetcher_request_method method,
        purc_variant_t params,
        uint32_t timeout,
        struct pcfetcher_resp_header *resp_header);

int pcfetch_check_response(void);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* not defined PURC_FETCHER_H */


