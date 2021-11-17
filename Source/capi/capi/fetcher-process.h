/*
 * @file fetcher-process.h
 * @author XueShuming
 * @date 2021/11/17
 * @brief The fetcher process class.
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

#ifndef PURC_FETCHER_PROCESS_H
#define PURC_FETCHER_PROCESS_H

#if ENABLE(LINK_PURC_FETCHER)

#include "purc/purc.h"
#include "fetcher-remote.h"
#include "ProcessLauncher.h"

class PcFetcherProcess :  public PurCFetcher::ProcessLauncher::Client {
public:
    PcFetcherProcess(struct pcfetcher* fetcher);
    ~PcFetcherProcess();

    void connect();
    void terminate();

    void didFinishLaunching(PurCFetcher::ProcessLauncher*,
            IPC::Connection::Identifier identifier);

private:
    struct pcfetcher* m_fetcher;
    IPC::Connection::Identifier m_identifier;

    RefPtr<PurCFetcher::ProcessLauncher> m_processLauncher;
};


#endif // ENABLE(LINK_PURC_FETCHER)

#endif /* not defined PURC_FETCHER_PROCESS_H */


