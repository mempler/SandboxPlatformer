#pragma once

#include "parser.hh"

#include <string>

class FileParser : public IParser {
public:
    void performParse(const std::string_view &svFileName, Kokoro::Memory::Buffer &bInputBuffer) override;
};
