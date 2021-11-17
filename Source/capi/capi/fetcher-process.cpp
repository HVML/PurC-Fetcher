/*
 * @file fetcher-process.cpp
 * @author XueShuming
 * @date 2021/11/17
 * @brief The impl for fetcher process.
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

#include "fetcher-process.h"

#include "ProcessLauncher.h"

#if ENABLE(LINK_PURC_FETCHER)

using namespace PurCFetcher;

PcFetcherProcess::PcFetcherProcess(struct pcfetcher* fetcher)
    : m_fetcher(fetcher)
{
}

PcFetcherProcess::~PcFetcherProcess()
{
}

void PcFetcherProcess::connect()
{
    ProcessLauncher::LaunchOptions launchOptions;
    launchOptions.processType = ProcessLauncher::ProcessType::Network;
    m_processLauncher = ProcessLauncher::create(this, WTFMove(launchOptions));
}

void PcFetcherProcess::terminate()
{
    if (m_processLauncher)
        m_processLauncher->terminateProcess();
}

void PcFetcherProcess::didFinishLaunching(ProcessLauncher*,
            IPC::Connection::Identifier identifier)
{
    this->m_identifier = identifier;
}


#endif // ENABLE(LINK_PURC_FETCHER)
