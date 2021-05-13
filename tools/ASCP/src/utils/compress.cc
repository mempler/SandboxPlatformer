#include "compress.hh"

#include <zlib.h>

#include <iostream>
#include <iterator>

#define GZIP_ENCODING 16
#define CHUNK 0x400

std::vector<uint8_t> CompressDataGZIP(tcb::span<uint8_t> vData) {
    std::vector<uint8_t> compressed;
    compressed.resize(vData.size()); // Yup

    z_stream stream; // custom allocator
    stream.zalloc = nullptr;
    stream.zfree = nullptr;
    stream.opaque = nullptr;

    if (deflateInit2(&stream, Z_BEST_COMPRESSION, Z_DEFLATED, MAX_WBITS | GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        std::cerr << "Compressing failed: " << stream.msg << std::endl;
        exit(1);
    }

    stream.next_in = vData.data();
    stream.avail_in = vData.size();

    stream.next_out = compressed.data();
    stream.avail_out = compressed.size();
    deflate(&stream, Z_FINISH);
    deflateEnd(&stream);

    compressed.resize(stream.total_out); // Resize the output vector to the given output size

    return compressed;
}
