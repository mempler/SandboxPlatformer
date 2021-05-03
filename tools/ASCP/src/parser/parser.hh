
#pragma once

#include <string_view>
#include <unordered_map>

#include <Kokoro/Memory.hh>

#include <utility>

#define MOVE_TO_HEAP(T, X, V) T *HEAP_##X = new T(V)

class IParser {
private:
    std::unordered_map<std::string, void *> m_Data;

public:
    virtual void performParse(const std::string_view &svFileName, Kokoro::Memory::Buffer &bInputBuffer) = 0;

    template <typename T>
    T *getValue(std::string_view svKey) {
        return (T *)getKValue(svKey);
    }

protected:
    template <typename T>
    void setKData(const std::string_view &svKey, T *tpData) {
        m_Data.insert(std::make_pair(svKey, (void *)tpData));
    }

    void *getKValue(const std::string_view &svKey) {
        if (m_Data.find(svKey.data()) == m_Data.end())
            return nullptr;
        return m_Data[svKey.data()];
    };
};
