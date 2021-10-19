/*
 * Copyright (C) 2003, 2006, 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2020 Beijing FMSoft Technologies Co., Ltd.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "IntPoint.h"
#include "LayoutUnit.h"

#if USE(CG)
typedef struct CGRect CGRect;
#endif

#if PLATFORM(MAC)
#ifdef NSGEOMETRY_TYPES_SAME_AS_CGGEOMETRY_TYPES
typedef struct CGRect NSRect;
#else
typedef struct _NSRect NSRect;
#endif
#endif

#if PLATFORM(IOS_FAMILY)
#ifndef NSRect
#define NSRect CGRect
#endif
#endif

#if PLATFORM(WIN)
typedef struct tagRECT RECT;

struct D2D_RECT_U;
typedef D2D_RECT_U D2D1_RECT_U;

struct D2D_RECT_F;
typedef D2D_RECT_F D2D1_RECT_F;
#endif

#if PLATFORM(HBD)
typedef struct _RECT RECT;
#endif

#if USE(CAIRO)
typedef struct _cairo_rectangle_int cairo_rectangle_int_t;
#endif

namespace WTF {
class TextStream;
}

namespace WebCore {

class FloatRect;
class LayoutRect;

class IntRect {
    WTF_MAKE_FAST_ALLOCATED;
public:
    IntRect() { }
    IntRect(const IntPoint& location, const IntSize& size)
        : m_location(location), m_size(size) { }
    IntRect(int x, int y, int width, int height)
        : m_location(IntPoint(x, y)), m_size(IntSize(width, height)) { }

    WTF_EXPORT_DECLARATION explicit IntRect(const FloatRect&); // don't do this implicitly since it's lossy
    WTF_EXPORT_DECLARATION explicit IntRect(const LayoutRect&); // don't do this implicitly since it's lossy
        
    IntPoint location() const { return m_location; }
    IntSize size() const { return m_size; }

    void setLocation(const IntPoint& location) { m_location = location; }
    void setSize(const IntSize& size) { m_size = size; }

    int x() const { return m_location.x(); }
    int y() const { return m_location.y(); }
    int maxX() const { return x() + width(); }
    int maxY() const { return y() + height(); }
    int width() const { return m_size.width(); }
    int height() const { return m_size.height(); }

    template <typename T = WTF::CrashOnOverflow>
    Checked<unsigned, T> area() const { return m_size.area<T>(); }

    void setX(int x) { m_location.setX(x); }
    void setY(int y) { m_location.setY(y); }
    void setWidth(int width) { m_size.setWidth(width); }
    void setHeight(int height) { m_size.setHeight(height); }

    bool isEmpty() const { return m_size.isEmpty(); }

    // NOTE: The result is rounded to integer values, and thus may be not the exact
    // center point.
    IntPoint center() const { return IntPoint(x() + width() / 2, y() + height() / 2); }

    void move(const IntSize& size) { m_location += size; } 
    void moveBy(const IntPoint& offset) { m_location.move(offset.x(), offset.y()); }
    void move(int dx, int dy) { m_location.move(dx, dy); } 

    void expand(const IntSize& size) { m_size += size; }
    void expand(int dw, int dh) { m_size.expand(dw, dh); }
    void contract(const IntSize& size) { m_size -= size; }
    void contract(int dw, int dh) { m_size.expand(-dw, -dh); }

    void shiftXEdgeTo(int edge)
    {
        int delta = edge - x();
        setX(edge);
        setWidth(std::max(0, width() - delta));
    }

    void shiftMaxXEdgeTo(int edge)
    {
        int delta = edge - maxX();
        setWidth(std::max(0, width() + delta));
    }

    void shiftYEdgeTo(int edge)
    {
        int delta = edge - y();
        setY(edge);
        setHeight(std::max(0, height() - delta));
    }

    void shiftMaxYEdgeTo(int edge)
    {
        int delta = edge - maxY();
        setHeight(std::max(0, height() + delta));
    }

    void shiftXEdgeBy(int delta)
    {
        move(delta, 0);
        setWidth(std::max(0, width() - delta));
    }

    void shiftYEdgeBy(int delta)
    {
        move(0, delta);
        setHeight(std::max(0, height() - delta));
    }

    IntPoint minXMinYCorner() const { return m_location; } // typically topLeft
    IntPoint maxXMinYCorner() const { return IntPoint(m_location.x() + m_size.width(), m_location.y()); } // typically topRight
    IntPoint minXMaxYCorner() const { return IntPoint(m_location.x(), m_location.y() + m_size.height()); } // typically bottomLeft
    IntPoint maxXMaxYCorner() const { return IntPoint(m_location.x() + m_size.width(), m_location.y() + m_size.height()); } // typically bottomRight
    
    WTF_EXPORT_DECLARATION bool intersects(const IntRect&) const;
    WTF_EXPORT_DECLARATION bool contains(const IntRect&) const;

    // This checks to see if the rect contains x,y in the traditional sense.
    // Equivalent to checking if the rect contains a 1x1 rect below and to the right of (px,py).
    bool contains(int px, int py) const
        { return px >= x() && px < maxX() && py >= y() && py < maxY(); }
    bool contains(const IntPoint& point) const { return contains(point.x(), point.y()); }

    WTF_EXPORT_DECLARATION void intersect(const IntRect&);
    WTF_EXPORT_DECLARATION void unite(const IntRect&);
    void uniteIfNonZero(const IntRect&);

    void inflateX(int dx)
    {
        m_location.setX(m_location.x() - dx);
        m_size.setWidth(m_size.width() + dx + dx);
    }
    void inflateY(int dy)
    {
        m_location.setY(m_location.y() - dy);
        m_size.setHeight(m_size.height() + dy + dy);
    }
    void inflate(int d) { inflateX(d); inflateY(d); }
    void inflate(IntSize size) { inflateX(size.width()); inflateY(size.height()); }
    WTF_EXPORT_DECLARATION void scale(float s);

    IntSize differenceToPoint(const IntPoint&) const;
    int distanceSquaredToPoint(const IntPoint& p) const { return differenceToPoint(p).diagonalLengthSquared(); }

    IntRect transposedRect() const { return IntRect(m_location.transposedPoint(), m_size.transposedSize()); }

    // Return false if x + width or y + height overflows.
    bool isValid() const;

#if PLATFORM(WIN)
    IntRect(const RECT&);
    operator RECT() const;
    explicit IntRect(const D2D1_RECT_F&);
    IntRect(const D2D1_RECT_U&);
    operator D2D1_RECT_F() const;
    operator D2D1_RECT_U() const;
#endif

#if USE(CAIRO)
    IntRect(const cairo_rectangle_int_t&);
    operator cairo_rectangle_int_t() const;
#endif

#if USE(CG)
    WTF_EXPORT_DECLARATION operator CGRect() const;
#endif

#if PLATFORM(MAC) && !defined(NSGEOMETRY_TYPES_SAME_AS_CGGEOMETRY_TYPES)
    WTF_EXPORT_DECLARATION operator NSRect() const;
#endif

#if PLATFORM(HBD)
    IntRect(const RECT&);
    operator RECT() const;
#endif

private:
    IntPoint m_location;
    IntSize m_size;
};

inline IntRect intersection(const IntRect& a, const IntRect& b)
{
    IntRect c = a;
    c.intersect(b);
    return c;
}

inline IntRect unionRect(const IntRect& a, const IntRect& b)
{
    IntRect c = a;
    c.unite(b);
    return c;
}

inline bool operator==(const IntRect& a, const IntRect& b)
{
    return a.location() == b.location() && a.size() == b.size();
}

inline bool operator!=(const IntRect& a, const IntRect& b)
{
    return a.location() != b.location() || a.size() != b.size();
}

inline IntRect& operator-=(IntRect& r, const IntPoint& offset)
{
    r.move(-offset.x(), -offset.y());
    return r;
}

inline IntRect operator-(const IntRect& r, const IntPoint& offset)
{
    IntRect t = r;
    return t -= offset;
}

#if USE(CG)
WTF_EXPORT_DECLARATION IntRect enclosingIntRect(const CGRect&);
#endif

#if PLATFORM(MAC) && !defined(NSGEOMETRY_TYPES_SAME_AS_CGGEOMETRY_TYPES)
WTF_EXPORT_DECLARATION IntRect enclosingIntRect(const NSRect&);
#endif

WTF_EXPORT_DECLARATION WTF::TextStream& operator<<(WTF::TextStream&, const IntRect&);

#ifdef __OBJC__
WTF_EXPORT_DECLARATION id makeNSArrayElement(const IntRect&);
#endif

} // namespace WebCore
