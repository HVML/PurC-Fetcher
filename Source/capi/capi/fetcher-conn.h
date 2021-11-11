/*
 * @file fetcher-conn.h
 * @author XueShuming
 * @date 2021/11/11
 * @brief The interfaces for fetcher connection.
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

#ifndef PURC_FETCHER_CONN_H
#define PURC_FETCHER_CONN_H

#include "fetcher-msg.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


#define PCFETCHER_MSG_MAX_SIZE 4096

typedef void (*pcfetcher_on_recv_data_func)(const uint8_t* data, size_t size);

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

struct pcfetcher_conn_socket_pair {
    int client;
    int server;
};

struct pcfetcher_conn {
    bool is_server;
    int socket;
    uint8_t read_buffer[PCFETCHER_MSG_MAX_SIZE];
    pcfetcher_on_recv_data_func on_recv_data_callback;
};

struct pcfetcher_conn_socket_pair pcfetcher_conn_create_socket_pair(void);

struct pcfetcher_conn* pcfetcher_conn(int socket, bool is_server, 
        pcfetcher_on_recv_data_func func);

bool pcfetcher_conn_send_msg(struct pcfetcher_conn* conn, const uint8_t* data,
        size_t size);

void pcfetcher_conn_read_socket();

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* not defined PURC_FETCHER_CONN_H */


