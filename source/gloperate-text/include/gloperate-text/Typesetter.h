
#pragma once


#include <string>

#include <glm/fwd.hpp>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{


enum class Alignment : unsigned char;

class GlyphSequence;
class FontFace;
class Glyph;


class GLOPERATE_TEXT_API Typesetter
{
public:
    Typesetter() = delete;
    virtual ~Typesetter() = delete;

    static const char32_t & lineFeed();

    static glm::vec2 extent(
        const GlyphSequence & sequence
    ,   const FontFace & fontFace
    ,   float fontSize);

    template <typename VertexIterator>
    static glm::vec2 typeset(
        const GlyphSequence & sequence
    ,   const FontFace & fontFace
    ,   const VertexIterator & begin
    ,   bool dryrun = false);


private:
    static bool typeset_wordwrap(
        const GlyphSequence & sequence
    ,   const FontFace & fontFace
    ,   const glm::vec2 & pen
    ,   const Glyph & glyph
    ,   const std::u32string::const_iterator & index
    ,   std::u32string::const_iterator & safe_forward);

    static std::u32string::const_iterator typeset_forward(
        const GlyphSequence & sequence
    ,   const FontFace & fontFace
    ,   const std::u32string::const_iterator & begin
    ,   float & width);

    template <typename VertexIterator>
    static void typeset_glyph(
        const FontFace & fontFace
    ,   const glm::vec2 & pen
    ,   const Glyph & glyph
    ,   const VertexIterator & vertex);

    static void typeset_extent(
        const FontFace & fontFace
    ,   std::u32string::const_iterator index
    ,   const std::u32string::const_iterator & begin
    ,   glm::vec2 & pen
    ,   glm::vec2 & extent);

    template <typename VertexIterator>
    static void typeset_align(
        const glm::vec2 & pen
    ,   const Alignment alignment
    ,   const VertexIterator & begin
    ,   const VertexIterator & end);

    template <typename VertexIterator>
    static void anchor_transform(
        const GlyphSequence & sequence
    ,   const FontFace & fontFace
    ,   const VertexIterator & begin
    ,   const VertexIterator & end);

    template <typename VertexIterator>
    static void vertex_transform(
        const glm::mat4 & sequence
    ,   const glm::vec4 & fontColor
    ,   const VertexIterator & begin
    ,   const VertexIterator & end);

    static glm::vec2 extent_transform(
        const GlyphSequence & sequence
    ,   const glm::vec2 & extent);
};


} // namespace gloperate_text


#include <gloperate-text/Typesetter.inl>
