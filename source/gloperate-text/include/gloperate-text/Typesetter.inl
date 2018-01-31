

#pragma once


#include <gloperate-text/Typesetter.h>

#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>

#include <gloperate-text/Alignment.h>
#include <gloperate-text/FontFace.h>
#include <gloperate-text/GlyphSequence.h>


namespace gloperate_text
{


template <typename VertexIterator>
glm::vec2 Typesetter::typeset(
    const GlyphSequence & sequence
,   const FontFace & fontFace
,   const VertexIterator & begin
,   bool dryrun)
{
    //const auto & padding = fontFace.glyphTexturePadding();

    auto pen = glm::vec2(0.f);
    auto vertex = begin;
    auto extent = glm::vec2(0.f);

    const auto iBegin = sequence.string().cbegin();
    const auto iEnd = sequence.string().cend();

    // iterator used to reduce the number of wordwrap forward passes
    auto safe_forward = iBegin;

    auto feedLine = false;
    auto feedVertex = vertex;

    for (auto i = iBegin; i != iEnd; ++i)
    {
        const auto & glyph = fontFace.glyph(*i);

        // handle line feeds as well as word wrap for next word (or
        // next glyph if word width exceeds the max line width)
        feedLine = *i == lineFeed() || (sequence.wordWrap() &&
            typeset_wordwrap(sequence, fontFace, pen, glyph, i, safe_forward));

        if (feedLine)
        {
            assert(i != iBegin);
            typeset_extent(fontFace, i - 1, iBegin, pen, extent);

            // handle alignment (when line feed occurs)
            if (!dryrun)
                typeset_align(pen, sequence.alignment(), feedVertex, vertex);

            pen.x = 0.f;
            pen.y -= fontFace.lineHeight();

            feedLine = false;
            feedVertex = vertex;
        }
        else if (i != iBegin) // apply kerning
            pen.x += fontFace.kerning(*(i - 1), *i);

        // typeset glyphs in vertex cloud (only if renderable)
        if (!dryrun && glyph.depictable())
            typeset_glyph(fontFace, pen, glyph, vertex++);

        pen.x += glyph.advance();

        if (i + 1 == iEnd) // handle alignment (when last line of sequence is processed)
        {
            typeset_extent(fontFace, i, iBegin, pen, extent);

            if (!dryrun)
                typeset_align(pen, sequence.alignment(), feedVertex, vertex);
        }
    }

    if (!dryrun)
    {
        anchor_transform(sequence, fontFace, begin, vertex);
        vertex_transform(sequence.transform(), sequence.fontColor(), begin, vertex);
    }

    return extent_transform(sequence, extent);
}

template <typename VertexIterator>
void Typesetter::typeset_glyph(
    const FontFace & fontFace
,   const glm::vec2 & pen
,   const Glyph & glyph
,   const VertexIterator & vertex)
{
    const auto & padding = fontFace.glyphTexturePadding();
    vertex->origin    = glm::vec3(pen, 0.f);
    vertex->origin.x += glyph.bearing().x - padding[3];
    vertex->origin.y += glyph.bearing().y - glyph.extent().y - padding[2];

    vertex->vtan   = glm::vec3(glyph.extent().x + padding[1] + padding[3], 0.f, 0.f);
    vertex->vbitan = glm::vec3(0.f, glyph.extent().y + padding[0] + padding[2], 0.f);

    const auto extentScale = 1.f / glm::vec2(fontFace.glyphTextureExtent());
    const auto ll = glyph.subTextureOrigin()
        - glm::vec2(padding[3], padding[2]) * extentScale;
    const auto ur = glyph.subTextureOrigin() + glyph.subTextureExtent()
        + glm::vec2(padding[1], padding[0]) * extentScale;
    vertex->uvRect = glm::vec4(ll, ur);
}

template <typename VertexIterator>
void Typesetter::typeset_align(
    const glm::vec2 & pen
,   const Alignment alignment
,   const VertexIterator & begin
,   const VertexIterator & end)
{
    if (alignment == Alignment::LeftAligned)
        return;

    auto penOffset = -pen.x;

    if (alignment == Alignment::Centered)
        penOffset *= 0.5f;

    // origin is expected to be in 'font face space' (not transformed)
    for (auto v = begin; v != end; ++v)
        v->origin.x += penOffset;
}

template <typename VertexIterator>
void Typesetter::anchor_transform(
    const GlyphSequence & sequence
,   const FontFace & fontFace
,   const VertexIterator & begin
,   const VertexIterator & end)
{
    auto offset = 0.f;

    switch (sequence.lineAnchor())
    {
    case LineAnchor::Ascent:
        offset = fontFace.ascent();
        break;
    case LineAnchor::Center:
        offset = fontFace.size() * 0.5f + fontFace.descent();
        break;
    case LineAnchor::Descent:
        offset = fontFace.descent();
        break;
    case LineAnchor::Baseline:
    default:
        return;
    }

    for (auto v = begin; v != end; ++v)
        v->origin.y -= offset;
}

template <typename VertexIterator>
void Typesetter::vertex_transform(
    const glm::mat4 & transform
,   const glm::vec4 & fontColor
,   const VertexIterator & begin
,   const VertexIterator & end)
{
    for (auto v = begin; v != end; ++v)
    {
        auto ll = transform * glm::vec4(v->origin, 1.f);
        auto lr = transform * glm::vec4(v->origin + v->vtan, 1.f);
        auto ul = transform * glm::vec4(v->origin + v->vbitan, 1.f);


        v->origin = glm::vec3(ll);
        v->vtan   = glm::vec3(lr - ll);
        v->vbitan = glm::vec3(ul - ll);
        v->fontColor = fontColor;
    }
}


} // namespace gloperate_text
