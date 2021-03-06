﻿#include "BaseFileReader.h"

#include <assert.h>

#include "../Common/StringHelper.h"

namespace Altseed {

BaseFileReader::BaseFileReader(const std::u16string& path) : BaseObject(), m_path(path), m_length(-1), m_position(0) {
    if (!GetIsInPackage()) {
        std::unique_lock<std::recursive_mutex> lock(m_readerMtx);

#ifdef _WIN32
        m_file.open((wchar_t*)path.c_str(), std::basic_ios<char>::in | std::basic_ios<char>::binary);
#else
        m_file.open(utf16_to_utf8(path).c_str(), std::basic_ios<char>::in | std::basic_ios<char>::binary);
#endif
    }
}

BaseFileReader::~BaseFileReader() {
    if (!GetIsInPackage()) m_file.close();
}

int64_t BaseFileReader::GetSize() {
    if (m_length < 0) {
        std::unique_lock<std::recursive_mutex> lock(m_readerMtx);
        m_file.seekg(0, std::ios_base::end);
        m_length = m_file.tellg();
        m_file.clear();
        m_file.seekg(0, std::ios_base::beg);
    }
    return m_length;
}

void BaseFileReader::ReadBytes(std::vector<uint8_t>& buffer, const int64_t count) {
    const auto size = GetSize();

    std::unique_lock<std::recursive_mutex> lock(m_readerMtx);

    if (m_position + count > GetSize() || count < 0) {
        buffer.resize(0);
        buffer.clear();
        return;
    }

    buffer.resize(count);
    m_file.read(reinterpret_cast<char*>(&buffer[0]), count);

    m_position += count;
}
uint32_t BaseFileReader::ReadUInt32() {
    std::vector<uint8_t> buffer;
    ReadBytes(buffer, sizeof(uint32_t));

    return *reinterpret_cast<const uint32_t*>(buffer.data());
}
uint64_t BaseFileReader::ReadUInt64() {
    std::vector<uint8_t> buffer;
    ReadBytes(buffer, sizeof(uint64_t));

    return *reinterpret_cast<const uint64_t*>(buffer.data());
}
void BaseFileReader::ReadAllBytes(std::vector<uint8_t>& buffer) {
    std::unique_lock<std::recursive_mutex> lock(m_readerMtx);
    const auto tmp = m_position;
    m_position = 0;
    ReadBytes(buffer, GetSize());
    m_position = tmp;
}

void BaseFileReader::Seek(const int64_t offset, const SeekOrigin origin) {
    const auto size = GetSize();

    std::unique_lock<std::recursive_mutex> lock(m_readerMtx);
    switch (origin) {
        case SeekOrigin::Begin:
            assert(0 <= offset && offset < size);
            m_file.seekg(offset, m_file.beg);
            m_position = offset;
            break;
        case SeekOrigin::Current:
            assert(0 <= m_position + offset && m_position + offset < size);
            m_file.seekg(offset, m_file.cur);
            m_position = offset + offset;
            break;
        case SeekOrigin::End:
            assert(0 <= offset + size && offset <= 0 && offset);
            m_file.seekg(offset, m_file.end);
            m_position = size + offset;
            break;
        default:
            assert(false);
            break;
    }
}

bool BaseFileReader::GetIsInPackage() const { return false; }
}  // namespace Altseed