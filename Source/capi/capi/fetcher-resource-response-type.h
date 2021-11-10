/*
 * @file fetcher-retype-response-type.h
 * @author XueShuming
 * @date 2021/11/10
 * @brief The resource response type enum.
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

#ifndef PURC_FETCHER_RETYPE_RESPONSE_TYPE_H
#define PURC_FETCHER_RETYPE_RESPONSE_TYPE_H

enum pcfetcher_resource_response_type {
    type_basic,
    type_cors,
    type_default,
    type_error,
    type_opaque,
    type_opaqueredirect
};

#endif /* not defined PURC_FETCHER_RETYPE_RESPONSE_TYPE_H */


