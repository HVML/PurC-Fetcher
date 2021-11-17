/*
 * @file fetcher-remote.cpp
 * @author XueShuming
 * @date 2021/11/16
 * @brief The impl for fetcher remote.
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
 * but WITHOUT ANY WARRANTY
{
}
 without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "fetcher-remote.h"

#include "ProcessLauncher.h"

#if ENABLE(LINK_PURC_FETCHER)

using namespace PurCFetcher;

class ProcessLauncherClient :  public ProcessLauncher::Client {
    void didFinishLaunching(ProcessLauncher*,
            IPC::Connection::Identifier identifier)
    {
        this->identifier = identifier;
    }

public:
    IPC::Connection::Identifier identifier;
    struct pcfetcher* fetcher;
};


struct pcfetcher_remote {
    struct pcfetcher base;
    RefPtr<ProcessLauncher> process_launcher;
    ProcessLauncherClient* process_launcher_client;
};

struct pcfetcher* pcfetcher_remote_init(size_t max_conns, size_t cache_quota)
{
    struct pcfetcher_remote* remote = (struct pcfetcher_remote*)malloc(
            sizeof(struct pcfetcher_remote));

    struct pcfetcher* fetcher = (struct pcfetcher*) remote;
    fetcher->max_conns = max_conns;
    fetcher->cache_quota = cache_quota;
    fetcher->init = pcfetcher_remote_init;
    fetcher->term = pcfetcher_remote_term;
    fetcher->set_base_url = pcfetcher_remote_set_base_url;
    fetcher->cookie_set = pcfetcher_cookie_remote_set;
    fetcher->cookie_get = pcfetcher_cookie_remote_get;
    fetcher->cookie_remove = pcfetcher_cookie_remote_remove;
    fetcher->request_async = pcfetcher_remote_request_async;
    fetcher->request_sync = pcfetcher_remote_request_sync;
    fetcher->check_response = pcfetcher_remote_check_response;

    remote->process_launcher_client = new ProcessLauncherClient();
    remote->process_launcher_client->fetcher = fetcher;

    ProcessLauncher::LaunchOptions launchOptions;
    launchOptions.processType = ProcessLauncher::ProcessType::Network;
    remote->process_launcher = ProcessLauncher::create(
            remote->process_launcher_client, WTFMove(launchOptions));

    return (struct pcfetcher*)remote;
}

int pcfetcher_remote_term(struct pcfetcher* fetcher)
{
    struct pcfetcher_remote* remote = (struct pcfetcher_remote*)fetcher;
    remote->process_launcher->terminateProcess();
    remote->process_launcher->invalidate();
    remote->process_launcher = nullptr;

    delete remote->process_launcher_client;
    free(remote);

    return 0;
}

const char* pcfetcher_remote_set_base_url(struct pcfetcher* fetcher,
        const char* base_url)
{
    UNUSED_PARAM(fetcher);
    UNUSED_PARAM(base_url);
    return NULL;
}

void pcfetcher_cookie_remote_set(struct pcfetcher* fetcher,
        const char* domain, const char* path, const char* name,
        const char* content, time_t expire_time, bool secure)
{
    UNUSED_PARAM(fetcher);
    UNUSED_PARAM(domain);
    UNUSED_PARAM(path);
    UNUSED_PARAM(name);
    UNUSED_PARAM(content);
    UNUSED_PARAM(expire_time);
    UNUSED_PARAM(secure);
}

const char* pcfetcher_cookie_remote_get(struct pcfetcher* fetcher,
        const char* domain, const char* path, const char* name,
        time_t *expire, bool *secure)
{
    UNUSED_PARAM(fetcher);
    UNUSED_PARAM(domain);
    UNUSED_PARAM(path);
    UNUSED_PARAM(name);
    UNUSED_PARAM(expire);
    UNUSED_PARAM(secure);
    return NULL;
}

const char* pcfetcher_cookie_remote_remove(struct pcfetcher* fetcher,
        const char* domain, const char* path, const char* name)
{
    UNUSED_PARAM(fetcher);
    UNUSED_PARAM(domain);
    UNUSED_PARAM(path);
    UNUSED_PARAM(name);
    return NULL;
}


purc_variant_t pcfetcher_remote_request_async(
        struct pcfetcher* fetcher,
        const char* url,
        enum pcfetcher_request_method method,
        purc_variant_t params,
        uint32_t timeout,
        response_handler handler,
        void* ctxt)
{
    UNUSED_PARAM(fetcher);
    UNUSED_PARAM(url);
    UNUSED_PARAM(method);
    UNUSED_PARAM(params);
    UNUSED_PARAM(timeout);
    UNUSED_PARAM(handler);
    UNUSED_PARAM(ctxt);
    return PURC_VARIANT_INVALID;
}


purc_rwstream_t pcfetcher_remote_request_sync(
        struct pcfetcher* fetcher,
        const char* url,
        enum pcfetcher_request_method method,
        purc_variant_t params,
        uint32_t timeout,
        struct pcfetcher_resp_header *resp_header)
{
    UNUSED_PARAM(fetcher);
    UNUSED_PARAM(url);
    UNUSED_PARAM(method);
    UNUSED_PARAM(params);
    UNUSED_PARAM(timeout);
    UNUSED_PARAM(resp_header);
    return NULL;
}


int pcfetcher_remote_check_response(struct pcfetcher* fetcher,
        uint32_t timeout_ms)
{
    UNUSED_PARAM(fetcher);
    UNUSED_PARAM(timeout_ms);
    return 0;
}


#endif // ENABLE(LINK_PURC_FETCHER)
