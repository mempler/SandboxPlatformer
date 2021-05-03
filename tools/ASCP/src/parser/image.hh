#pragma once

#include "parser.hh"

#include <string>

class ImageParser : public IParser {
public:
    void performParse(const std::string_view &svFileName, Kokoro::Memory::Buffer &bInputBuffer) override;
};