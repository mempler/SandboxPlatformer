#pragma once

#include "span.hpp"

#include <vector>

std::vector<uint8_t> CompressDataGZIP(tcb::span<uint8_t> vData);
std::vector<uint8_t> DecompressDataGZIP(tcb::span<uint8_t> vData);
