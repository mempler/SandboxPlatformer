#pragma once

#include "span.hpp"

#include <vector>

std::vector<uint8_t> CompressDataGZIP( Kokoro::Memory::Span<uint8_t> vData );
std::vector<uint8_t> DecompressDataGZIP( Kokoro::Memory::Span<uint8_t> vData );
