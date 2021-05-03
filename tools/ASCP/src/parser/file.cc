#include "file.hh"

#include "Kokoro/FileSystem.hh"

#include <bimg/bimg.h>
#include <bimg/decode.h>
#include <bimg/encode.h>

namespace bgfx {
    // Hack for getting BGFX Allocator
    extern bx::AllocatorI *g_allocator;
} // namespace bgfx

void FileParser::performParse(const std::string_view &svFileName, Kokoro::Memory::Buffer &bInputBuffer) {
    std::string_view extension = svFileName.substr(svFileName.rfind('.') + 1);

    MOVE_TO_HEAP(std::vector<uint8_t>, data, bInputBuffer.data());

    setKData("data", HEAP_data);
}
