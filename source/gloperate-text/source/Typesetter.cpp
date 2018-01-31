
#include <gloperate-text/Typesetter.h>

#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>

#include <gloperate-text/Alignment.h>
#include <gloperate-text/FontFace.h>
#include <gloperate-text/GlyphSequence.h>
#include <gloperate-text/GlyphVertexCloud.h>


namespace gloperate_text
{


const char32_t & Typesetter::lineFeed()
{
    static const auto LF = static_cast<char32_t>('\x0A');
    return LF;
}

glm::vec2 Typesetter::extent(
    const GlyphSequence & sequence
,   const FontFace & fontFace
,   const float fontSize)
{
    return typeset(sequence, fontFace
        , GlyphVertexCloud::Vertices::iterator(), true) * fontSize / fontFace.size();
}

bool Typesetter::typeset_wordwrap(
    const GlyphSequence & sequence
,   const FontFace & fontFace
,   const glm::vec2 & pen
,   const Glyph & glyph
,   const std::u32string::const_iterator & index
,   std::u32string::const_iterator & safe_forward)
{
    assert(sequence.wordWrap());

    const auto lineWidth = sequence.lineWidth();
    auto width_forward = 0.f;

    const auto pen_glyph = pen.x + glyph.advance()
        + (index != sequence.string().cbegin() ? fontFace.kerning(*(index - 1), *index) : 0.f);

    const auto wrap_glyph = glyph.depictable() && pen_glyph > lineWidth
        && (glyph.advance() <= lineWidth || pen.x > 0.f);

    auto wrap_forward = false;
    if (!wrap_glyph && index >= safe_forward)
    {
        safe_forward = typeset_forward(sequence, fontFace, index, width_forward);
        wrap_forward = width_forward <= lineWidth && (pen.x + width_forward) > lineWidth;
    }

    return wrap_forward || wrap_glyph;
}

std::u32string::const_iterator Typesetter::typeset_forward(
    const GlyphSequence & sequence
,   const FontFace & fontFace
,   const std::u32string::const_iterator & begin
,   float & width)
{
    // setup common delimiters 
    // Note: u32string::find outperforms set::count here (tested)

    static const auto delimiters = std::u32string({ '\x0A', ' ', ',', '.', '-', '/', '(', ')', '[', ']', '<', '>' });

    const auto iBegin = sequence.string().cbegin();
    const auto iEnd = sequence.string().cend();

    width = 0.f; // reset the width 

    // accumulate glyph advances (including kerning) up to the next
    // delimiter occurence starting at begin of the string.
    auto i = begin;
    while (i != iEnd && delimiters.find(*i) == delimiters.npos)
    {
        if (i != iBegin)
            width += fontFace.kerning(*(i - 1), *i);

        width += fontFace.glyph(*i++).advance();
    }
    return i;
}

void Typesetter::typeset_extent(
    const FontFace & fontFace
,   std::u32string::const_iterator index
,   const std::u32string::const_iterator & begin
,   glm::vec2 & pen
,   glm::vec2 & extent)
{
    // on line feed, revert advance of preceding, not depictable glyphs
    while (index > begin)
    {
        auto precedingGlyph = fontFace.glyph(*index);
        if (precedingGlyph.depictable())
            break;

        pen.x -= precedingGlyph.advance();
        --index;
    }
    extent.x = glm::max(pen.x, extent.x);
    extent.y += fontFace.lineHeight();
}

glm::vec2 Typesetter::extent_transform(
    const GlyphSequence & sequence
,   const glm::vec2 & extent)
{
    auto ll = sequence.transform() * glm::vec4(     0.f,      0.f, 0.f, 1.f);
    auto lr = sequence.transform() * glm::vec4(extent.x,      0.f, 0.f, 1.f);
    auto ul = sequence.transform() * glm::vec4(     0.f, extent.y, 0.f, 1.f);

    return glm::vec2(glm::distance(lr, ll), glm::distance(ul, ll));
}


} // namespace gloperate_text
