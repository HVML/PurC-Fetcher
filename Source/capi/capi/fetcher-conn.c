/*
 * @file fetcher-conn.c
 * @author XueShuming
 * @date 2021/11/17
 * @brief The impl for fetcher connection.
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

#include "fetcher-conn.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

struct pcfetcher_conn_socket_pair pcfetcher_conn_create_socket_pair(void)
{
    int sockets[2];
    if (socketpair(AF_UNIX, SOCK_SEQPACKET, 0, sockets) == -1) {
        struct pcfetcher_conn_socket_pair socket_pair = { -1, -1 };
        return socket_pair;
    };

    struct pcfetcher_conn_socket_pair socket_pair = { sockets[0], sockets[1] };
    return socket_pair;
}

struct pcfetcher_conn* pcfetcher_conn(int socket, bool is_server,
        pcfetcher_on_recv_data_func func)
{
    struct pcfetcher_conn* conn = (struct pcfetcher_conn*) calloc(
            sizeof(struct pcfetcher_conn), 1);
    conn->is_server = is_server;
    conn->socket = socket;
    conn->on_recv_data_callback = func;
    return conn;
}

bool pcfetcher_conn_send_msg(struct pcfetcher_conn* conn, const uint8_t* data,
        size_t size)
{
    (void)conn;
    (void)data;
    (void)size;
    return false;
}

void pcfetcher_conn_read_socket()
{
}
