#include <string_view>
#include <unordered_map>

#include "parser/audio.hh"
#include "parser/file.hh"
#include "parser/image.hh"
#include "parser/parser.hh"

#include <Kokoro/FileSystem.hh>
#include <Kokoro/Memory.hh>
#include <Kokoro/Utility.hh>

#define JSON5_C 1
#include <json5.h>

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#define LOG_WARN std::cout << "WARN> "

struct MetadataInfo {
    std::string m_sStructName;
    std::string m_sUID;
    std::string m_sExt;
    std::string m_sParser;

    uint32_t m_uVersion;

    std::vector<std::string> m_vsFlags;
    std::vector<std::string> m_vsExtensions;

    // Version > Key > Value
    std::vector<std::unordered_map<std::string, std::string>> m_vSchema;
};

static std::unordered_map<std::string, IParser *> g_umParsers;
static std::vector<MetadataInfo> g_vMetadata;

template <class T>
void registerParser(const std::string_view &svName) {
    g_umParsers.insert(std::make_pair(svName, (IParser *)new T));
}

static std::string generateCodeForMeta(MetadataInfo metaData) {
    std::stringstream code{};
    std::stringstream parser_code;

    // Generate header
    code << "// !!! This code is generated and might be changed DO NOT EDIT !!!" << std::endl << std::endl;

    code << "#include \"Kokoro/Memory.hh\"" << std::endl << std::endl;
    code << "struct " << metaData.m_sStructName << std::endl << "{" << std::endl;

    parser_code << "    void unpack(Kokoro::Memory::Buffer& buff)" << std::endl << "    {" << std::endl;

    for (size_t i = 0; i < metaData.m_vSchema.size(); i++) {
        code << "    // since v" << i << std::endl;
        parser_code << "        // since v" << i << std::endl;

        auto schema = metaData.m_vSchema[i];

        int si = 0;
        for (auto s : schema) {
            code << "    ";
            parser_code << "        ";

            auto c = s.first;
            c[0] = toupper(c[0]);

            if (s.second == "int8") {
                code << "int8_t m_i";

                parser_code << "m_i" << c << " = "
                            << "buff.Pop<int8_t>(1);" << std::endl;
            } else if (s.second == "int16") {
                code << "int16_t m_i";

                parser_code << "m_i" << c << " = "
                            << "buff.Pop<int16_t>(2);" << std::endl;
            } else if (s.second == "int32") {
                code << "int32_t m_i";

                parser_code << "m_i" << c << " = "
                            << "buff.Pop<int32_t>(4);" << std::endl;
            } else if (s.second == "int64") {
                code << "int64_t m_i";

                parser_code << "m_i" << c << " = "
                            << "buff.Pop<int64_t>(8);" << std::endl;
            }

            else if (s.second == "uint8") {
                code << "uint8_t m_u";

                parser_code << "m_u" << c << " = "
                            << "buff.Pop<uint8_t>(1);" << std::endl;
            } else if (s.second == "uint16") {
                code << "uint16_t m_u";

                parser_code << "m_u" << c << " = "
                            << "buff.Pop<uint16_t>(2);" << std::endl;
            } else if (s.second == "uint32") {
                code << "uint32_t m_u";

                parser_code << "m_u" << c << " = "
                            << "buff.Pop<uint32_t>(4);" << std::endl;
            } else if (s.second == "uint64") {
                code << "uint64_t m_u";

                parser_code << "m_u" << c << " = "
                            << "buff.Pop<uint64_t>(8);" << std::endl;
            }

            else if (s.second == "float32") {
                code << "float m_f";

                parser_code << "m_f" << c << " = "
                            << "buff.Pop<float>(4);" << std::endl;
            } else if (s.second == "float64") {
                code << "double m_d";

                parser_code << "m_d" << c << " = "
                            << "buff.Pop<double>(8);" << std::endl;
            }

            else if (s.second == "[..]byte") {
                code << "Kokoro::Memory::Buffer m_";

                // TODO:
                parser_code << "size_t sSize_" << si << " = buff.Pop<size_t>(4);" << std::endl;
                parser_code << "        ";
                parser_code << "std::string sBufferData_" << si << " = buff.Pop<std::string>(sSize_" << si << ");" << std::endl;
                parser_code << "        ";
                parser_code << "m_" << c << " = Kokoro::Memory::Buffer(std::vector<uint8_t>(sBufferData_" << si << ".begin(), "
                            << "sBufferData_" << si << ".end()));" << std::endl;
                si++;
            } else {
                LOG_WARN << "Unimplemented data type: " << s.second << std::endl;
            }

            code << c << ";" << std::endl;
        }
    }

    parser_code << std::endl << "    }" << std::endl;

    code << std::endl << parser_code.str();

    code << "};" << std::endl;
    return code.str();
}

std::pair<MetadataInfo, IParser *> getMetaParserByExt(const std::string_view &svExt) {
    for (auto meta : g_vMetadata) {
        // Try to find the correct metadata
        //-----------------------------------
        if (std::find(meta.m_vsExtensions.begin(), meta.m_vsExtensions.end(), svExt) != meta.m_vsExtensions.end()) {
            // We found it!
            // Now lets find the parser
            //-----------------------------------
            if (g_umParsers.find(meta.m_sParser) != g_umParsers.end()) {
                // found it!
                //-----------------------------------
                return std::make_pair(meta, g_umParsers.at(meta.m_sParser));
                //-----------------------------------
            }
            //-----------------------------------

            // we didn't find it, so lets continue!
            //-------------------------------------
        }
    }

    return std::make_pair(MetadataInfo{}, nullptr);
}

#define IMPL_SIMPLE_DATA_TYPE(S, T)                                              \
    {                                                                            \
        T##_t *v = parser->getValue<T##_t>(S.first);                             \
        if (v == nullptr) {                                                      \
            LOG_WARN << "Unimplemented key in parser: " << s.first << std::endl; \
        } else {                                                                 \
            buffer.Push(*v);                                                     \
        }                                                                        \
    }

Kokoro::Memory::Buffer constructBinaryFromMeta(IParser *parser, MetadataInfo meta) {
    Kokoro::Memory::Buffer buffer;

    buffer.Push(meta.m_sUID);
    buffer.Push(meta.m_uVersion);

    for (auto s : meta.m_vSchema[meta.m_uVersion]) {
        // Try to find the correct Metadata
        //-----------------------------------

        if (s.second == "int8") {
            IMPL_SIMPLE_DATA_TYPE(s, int8);
        } else if (s.second == "int16") {
            IMPL_SIMPLE_DATA_TYPE(s, int16);
        } else if (s.second == "int32") {
            IMPL_SIMPLE_DATA_TYPE(s, int32);
        } else if (s.second == "int64") {
            IMPL_SIMPLE_DATA_TYPE(s, int64);
        }

        else if (s.second == "uint8") {
            IMPL_SIMPLE_DATA_TYPE(s, uint8);
        } else if (s.second == "uint16") {
            IMPL_SIMPLE_DATA_TYPE(s, uint16);
        } else if (s.second == "uint32") {
            IMPL_SIMPLE_DATA_TYPE(s, uint32);
        } else if (s.second == "uint64") {
            IMPL_SIMPLE_DATA_TYPE(s, uint64);
        }

        else if (s.second == "float32") {
            IMPL_SIMPLE_DATA_TYPE(s, float);
        } else if (s.second == "float64") {
            IMPL_SIMPLE_DATA_TYPE(s, double);
        }

        else if (s.second == "[..]byte") {
            std::vector<uint8_t> *v = parser->getValue<std::vector<uint8_t>>(s.first);

            if (v == nullptr) {
                LOG_WARN << "Unimplemented key in parser: " << s.first << std::endl;
            } else {
                buffer.Push(v->size());
                buffer.Append(*v);
            }
        }

        else {
            LOG_WARN << "Unimplemented data type: {}" << s.second << std::endl;
        }
        //-----------------------------------
    }

    return buffer;
}

/*
    ASCP (Asset Compiler) v1.0.0
    Copyright by Robin A. P. 2021

    .\ASCP.exe +file .\texture.png
*/
int main(int argc, const char **argv) {
    std::cout << "ASCP (Asset Compiler) v1.0.0" << std::endl << "Copyright by Robin A. P. 2021" << std::endl << std::endl;

    Kokoro::Utility::ArgParser argParser(argc, argv);

    std::string metadata_dir = "./metadata";
    std::string input_file = "";
    std::string output_path = "";

    bool generateCode = argParser.HasArg("gencode");

    if (!generateCode)
        if (!generateCode) {
            // Check if we have the +file arg
            //-----------------------------------
            if (!argParser.HasConfig("file")) {
                std::cout << "Missing argument: +file" << std::endl
                          << std::endl
                          << "ascp.exe +file [INPUT_FILE] +output "
                             "[OUTPUT_DIRECTORY] -gencode?"
                          << std::endl;
                return 1;
            }

            argParser.GetConfig("file", &input_file);

            if (!argParser.HasConfig("output")) {
                std::cout << "Missing argument: +output" << std::endl
                          << std::endl
                          << "ascp.exe +file [INPUT_FILE] [INPUT_FILE] +output "
                             "-gencode?"
                             "[OUTPUT_DIRECTORY]"
                          << std::endl;
                return 1;
            }

            argParser.GetConfig("output", &output_path);

            if (!Kokoro::FileSystem::Exists(output_path)) {
                Kokoro::FileSystem::CreateDirectory(output_path);
            }
        }

    // Check if we have the +metadata arg
    //-----------------------------------
    if (argParser.HasConfig("metadata")) {
        argParser.GetConfig("metadata", &metadata_dir);
    }

    // Register all parsers
    //-----------------------------------
    registerParser<ImageParser>("image");
    registerParser<AudioParser>("audio");
    registerParser<FileParser>("file");
    //-----------------------------------

    // Process metadata
    //-----------------------------------
    auto metadataFiles = Kokoro::FileSystem::ReadDirectory(metadata_dir);
    for (auto metaFilePath : metadataFiles) {
        std::vector<uint8_t> metaFileRawBinary = Kokoro::FileSystem::ReadBinaryFile(metaFilePath);
        std::string metaFileRawJson = std::string(metaFileRawBinary.begin(), metaFileRawBinary.end());

        json5_object metaDataFile;
        json5_parse(&metaDataFile, (char *)metaFileRawJson.c_str(), true);

        MetadataInfo info{};

        for (size_t i = 0; i < array_count(metaDataFile.elements); i++) {
            auto activeElement = metaDataFile.elements[i];
            if (strcmp(activeElement.name, "") == 0)
                continue;

            if (strcmp(activeElement.name, "unique_id") == 0) {
                info.m_sUID = activeElement.string;
            } else if (strcmp(activeElement.name, "ext") == 0) {
                info.m_sExt = activeElement.string;
            } else if (strcmp(activeElement.name, "struct") == 0) {
                info.m_sStructName = activeElement.string;
            } else if (strcmp(activeElement.name, "parser") == 0) {
                info.m_sParser = activeElement.string;
            } else if (strcmp(activeElement.name, "version") == 0) {
                info.m_uVersion = activeElement.integer;
            } else if (strcmp(activeElement.name, "flags") == 0) {
                for (size_t i = 0; i < array_count(activeElement.elements); i++) {
                    auto activeFlagElement = activeElement.elements[i];

                    if (activeFlagElement.string == nullptr)
                        continue;

                    info.m_vsFlags.push_back(activeFlagElement.string);
                }
            } else if (strcmp(activeElement.name, "extensions") == 0) {
                for (size_t i = 0; i < array_count(activeElement.elements); i++) {
                    auto activeExtElement = activeElement.elements[i];

                    if (activeExtElement.string == nullptr)
                        continue;

                    info.m_vsExtensions.push_back(activeExtElement.string);
                }
            } else if (strcmp(activeElement.name, "schema") == 0) {
                for (size_t i = 0; i < array_count(activeElement.elements); i++) {
                    auto activeSchemaElement_ = activeElement.elements[i];

                    info.m_vSchema.emplace_back();

                    for (size_t i = 0; i < array_count(activeSchemaElement_.elements); i++) {
                        auto activeSchemaElement = activeSchemaElement_.elements[i];

                        info.m_vSchema.back().insert(std::make_pair(activeSchemaElement.name, activeSchemaElement.string));
                    }
                }
            } else {
                LOG_WARN << "Key not implemented: " << activeElement.name << std::endl;
            }
        }

        g_vMetadata.push_back(info);
    }
    //-----------------------------------

    if (generateCode) {
        std::cout << "Generate Code..." << std::endl;

        for (auto metaInfo : g_vMetadata) {
            auto code = generateCodeForMeta(metaInfo);

            std::string outputPath = Kokoro::FileSystem::JoinPath(output_path, metaInfo.m_sStructName + ".hh");

            std::cout << outputPath << std::endl;

            auto data = std::vector<uint8_t>(code.begin(), code.end());

            Kokoro::FileSystem::WriteBinaryFile(outputPath, data);
        }

        return 0;
    }

    // Process input files
    //-----------------------------------
    std::string_view file_name = Kokoro::FileSystem::GetFileName(input_file);
    std::string_view file_name_no_ext = file_name.substr(0, file_name.rfind('.'));
    std::string file_extension = input_file.substr(input_file.rfind('.') + 1);

    auto metaParser = getMetaParserByExt(file_extension);
    if (metaParser.second == nullptr) {
        std::cout << "FATAL> Missing metadata for ext:" << file_extension << std::endl;
        exit(1);
    }

    Kokoro::Memory::Buffer buffer(Kokoro::FileSystem::ReadBinaryFile(input_file));

    metaParser.second->performParse(input_file, buffer);

    Kokoro::Memory::Buffer binaryBuffer = constructBinaryFromMeta(metaParser.second, metaParser.first);

    std::string outputPath = Kokoro::FileSystem::JoinPath(output_path, std::string(file_name_no_ext.begin(), file_name_no_ext.end()) + metaParser.first.m_sExt);

    Kokoro::FileSystem::WriteBinaryFile(outputPath, binaryBuffer.data());
    //-----------------------------------
    return 0;
}
