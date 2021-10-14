/* 
 * Copyright (C) 2020 Beijing FMSoft Technologies Co., Ltd.
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
 * 
 * Or,
 * 
 * As this component is a program released under LGPLv3, which claims
 * explicitly that the program could be modified by any end user
 * even if the program is conveyed in non-source form on the system it runs.
 * Generally, if you distribute this program in embedded devices,
 * you might not satisfy this condition. Under this situation or you can
 * not accept any condition of LGPLv3, you need to get a commercial license
 * from FMSoft, along with a patent license for the patents owned by FMSoft.
 * 
 * If you have got a commercial/patent license of this program, please use it
 * under the terms and conditions of the commercial license.
 * 
 * For more information about the commercial license and patent license,
 * please refer to
 * <https://hybridos.fmsoft.cn/blog/hybridos-licensing-policy/>.
 * 
 * Also note that the LGPLv3 license does not apply to any entity in the
 * Exception List published by Beijing FMSoft Technologies Co., Ltd.
 * 
 * If you are or the entity you represent is listed in the Exception List,
 * the above open source or free software license does not apply to you
 * or the entity you represent. Regardless of the purpose, you should not
 * use the software in any way whatsoever, including but not limited to
 * downloading, viewing, copying, distributing, compiling, and running.
 * If you have already downloaded it, you MUST destroy all of its copies.
 * 
 * The Exception List is published by FMSoft and may be updated
 * from time to time. For more information, please see
 * <https://www.fmsoft.cn/exception-list>.
 */ 

#pragma once

#include "config.h"
#include "FilterBase.h"

namespace WebKit {

Vector<String> FilterBase::splitUTF8(const char* source, const char* sourceEnd)
{
    Vector<String> result;
    for (int sourceOffset = 0; sourceOffset < sourceEnd - source; ) {
        int begin = sourceOffset;

        UChar32 character;
        U8_NEXT(reinterpret_cast<const uint8_t*>(source), sourceOffset, sourceEnd - source, character);
        if (character < 0)
            break;
        result.append(String(source + begin, sourceOffset - begin));
    }
    return result;
}

#if 0
Vector<Row> FilterBase::doFilter(Vector<Row> rowVec, String param)
{
    return rowVec;
}
#endif

UCharBreaker::UCharBreaker(const char* text)
 : m_text(text)
 , m_uchar(NULL)
 , m_ucharLen(0)
 , m_breakOpps(NULL)
 , m_breakOppsCount(0)
{
}

UCharBreaker::~UCharBreaker()
{
    if (m_uchar)
        g_free(m_uchar);

    if (m_breakOpps)
        free(m_breakOpps);
}

void UCharBreaker::doUStrGetBreaks()
{
    if (!m_text)
        return;

    int textLen = strlen(m_text);
    if (textLen <= 0)
        return;

    m_uchar = g_utf8_to_ucs4_fast(m_text, -1, &m_ucharLen);
    if(!m_uchar || m_ucharLen <=0)
        return;

    m_breakOppsCount = UStrGetBreaks(LANGCODE_unknown, CTR_NONE, WBR_NORMAL, LBP_NORMAL, m_uchar, m_ucharLen, &m_breakOpps);
}

} // namespace WebKit
