#include "image.hh"

#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <bimg/encode.h>

namespace bgfx
{
    // Hack for getting BGFX Allocator
    extern bx::AllocatorI *g_allocator;
}  // namespace bgfx

void ImageParser::performParse( const std::string_view &svFileName,
                                Kokoro::Memory::Buffer &bInputBuffer )
{
    std::string_view extension =
        svFileName.substr( svFileName.rfind( '.' ) + 1 );

    auto &data = bInputBuffer.data();

    auto *image =
        bimg::imageParse( bgfx::g_allocator, data.data(), data.size() );

    MOVE_TO_HEAP( int, width, image->m_width );
    MOVE_TO_HEAP( int, height, image->m_height );
    MOVE_TO_HEAP( int, format, image->m_format );
    MOVE_TO_HEAP(
        std::vector<uint8_t>, data,
        std::vector<uint8_t>( (uint8_t *) image->m_data,
                              (uint8_t *) image->m_data + image->m_size ) );

    setKData( "data", HEAP_data );
    setKData( "width", HEAP_width );
    setKData( "height", HEAP_height );
    setKData( "format", HEAP_format );

    // Cleanup
    bimg::imageFree( image );
}
