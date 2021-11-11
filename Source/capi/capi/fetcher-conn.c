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
#include <errno.h>
#include <poll.h>

#include <sys/types.h>
#include <sys/socket.h>

struct pcfetcher_message_info {
    size_t body_size;
    size_t attachment_count;
    bool is_body_out_of_line;
};

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
    struct pcfetcher_message_info messageInfo = {size, 0, false};

    struct msghdr message;
    memset(&message, 0, sizeof(message));

    struct iovec iov[3];
    memset(&iov, 0, sizeof(iov));

    message.msg_iov = iov;
    int iovLength = 1;

    iov[0].iov_base = (void*)&messageInfo;
    iov[0].iov_len = sizeof(messageInfo);

    if (!messageInfo.is_body_out_of_line && messageInfo.body_size) {
        iov[iovLength].iov_base = (void*)data;
        iov[iovLength].iov_len = size;
        ++iovLength;
    }

    message.msg_iovlen = iovLength;

    while (sendmsg(conn->socket, &message, MSG_NOSIGNAL) == -1) {
        if (errno == EINTR)
            continue;
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            struct pollfd pollfd;
            pollfd.fd = conn->socket;
            pollfd.events = POLLOUT;
            pollfd.revents = 0;
            poll(&pollfd, 1, -1);
            continue;
        }

        return false;
    }
    return true;
}

ssize_t pcfetcher_conn_read_bytes_from_socket(struct pcfetcher_conn* conn)
{
    (void)conn;
    return 0;
}

bool pcfetcher_conn_process_message(struct pcfetcher_conn* conn)
{
    (void)conn;
    return false;
}

void pcfetcher_conn_read_socket(struct pcfetcher_conn* conn)
{
    while (true) {
        ssize_t bytesRead = pcfetcher_conn_read_bytes_from_socket(conn);

        if (bytesRead < 0) {
            return;
        }

        if (!bytesRead) {
            return;
        }

        // Process messages from data received.
        while (true) {
            if (!pcfetcher_conn_process_message(conn))
                break;
        }
    }
}
