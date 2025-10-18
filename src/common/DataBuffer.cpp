// (c) 2025 petrsnd@secureidx.com.  All rights reserved.

#include "DataBuffer.h"
#include "StringUtils.h"

#include <cstring>

namespace DraftMiller {

DataBufferException::DataBufferException(const std::string& msg) : std::runtime_error(msg)
{}

void DataBuffer::InitExtractorAndAppender()
{
#if defined(WORDS_BIGENDIAN)
    if (m_endianness == BigEndian)
    {
        m_extractor = [this](auto&& PH1, auto&& PH2) {
            NormalExtractInteger(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
        };
        m_appender = [this](auto&& PH1, auto&& PH2) {
            NormalAppendInteger(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
        };
    }
    else
    {
        m_extractor = [this](auto&& PH1, auto&& PH2) {
            ByteSwapExtractInteger(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
        };
        m_appender = [this](auto&& PH1, auto&& PH2) {
            ByteSwapAppendInteger(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
        };
    }
#else
    if (m_endianness == BigEndian)
    {
        m_extractor = [this](auto&& PH1, auto&& PH2) {
            ByteSwapExtractInteger(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
        };
        m_appender = [this](auto&& PH1, auto&& PH2) {
            ByteSwapAppendInteger(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
        };
    }
    else
    {
        m_extractor = [this](auto&& PH1, auto&& PH2) {
            NormalExtractInteger(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
        };
        m_appender = [this](auto&& PH1, auto&& PH2) {
            NormalAppendInteger(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
        };
    }
#endif
}
DataBuffer::DataBuffer(const DataBuffer::Endianness e) :
    m_frontPos(0),
    m_endianness(e)
{
    InitExtractorAndAppender();
}
DataBuffer::DataBuffer(const unsigned char* uc, const size_t len, const Endianness e) :
    m_frontPos(0),
    m_endianness(e)
{
    WriteCharArray(reinterpret_cast<const char*>(uc), len, false);
    InitExtractorAndAppender();
}
DataBuffer::DataBuffer(const Buffer& b, const Endianness e) :
    m_data(b.begin(), b.end()),
    m_frontPos(0),
    m_endianness(e)
{
    InitExtractorAndAppender();
}
DataBuffer::DataBuffer(const DataBuffer& d) :
    m_data(d.m_data.begin() + d.m_frontPos, d.m_data.end()), 
    m_frontPos(0),
    m_endianness(d.m_endianness)
{
    InitExtractorAndAppender();
}
DataBuffer::~DataBuffer() = default;
DataBuffer& DataBuffer::operator=(const DataBuffer& d)
{
    m_data = std::vector<unsigned char>(d.m_data.begin() + d.m_frontPos, d.m_data.end());
    m_frontPos = 0;
    m_endianness = d.m_endianness;
    InitExtractorAndAppender();
    return *this;
}
DataBuffer::operator Buffer() const
{
    return {m_data.begin() + m_frontPos, m_data.end()};
}
void DataBuffer::Clear()
{
    m_data.clear();
    m_frontPos = 0;
}
size_t DataBuffer::Size() const
{
    return m_data.size() - m_frontPos;
}
bool DataBuffer::Empty() const
{
    return Size() == 0;
}
unsigned char& DataBuffer::operator[](size_t n)
{
    return m_data[m_frontPos + n];
}
const unsigned char& DataBuffer::operator[](size_t n) const
{
    return m_data[m_frontPos + n];
}
int8_t DataBuffer::ReadInt8()
{
    return static_cast<int8_t>(ExtractByte());
}
uint8_t DataBuffer::ReadUInt8()
{
    return static_cast<uint8_t>(ExtractByte());
}
// ReSharper disable once CppMemberFunctionMayBeConst
int16_t DataBuffer::ReadInt16()
{
    int16_t ret = 0;
    m_extractor(reinterpret_cast<unsigned char*>(&ret), sizeof(int16_t));
    return ret;
}
// ReSharper disable once CppMemberFunctionMayBeConst
uint16_t DataBuffer::ReadUInt16()
{
    uint16_t ret = 0;
    m_extractor(reinterpret_cast<unsigned char*>(&ret), sizeof(uint16_t));
    return ret;
}
// ReSharper disable once CppMemberFunctionMayBeConst
int32_t DataBuffer::ReadInt32()
{
    int32_t ret = 0;
    m_extractor(reinterpret_cast<unsigned char*>(&ret), sizeof(int32_t));
    return ret;
}
// ReSharper disable once CppMemberFunctionMayBeConst
uint32_t DataBuffer::ReadUInt32()
{
    uint32_t ret = 0;
    m_extractor(reinterpret_cast<unsigned char*>(&ret), sizeof(uint32_t));
    return ret;
}
// ReSharper disable once CppMemberFunctionMayBeConst
int64_t DataBuffer::ReadInt64()
{
    int64_t ret = 0;
    m_extractor(reinterpret_cast<unsigned char*>(&ret), sizeof(int64_t));
    return ret;
}
// ReSharper disable once CppMemberFunctionMayBeConst
uint64_t DataBuffer::ReadUInt64()
{
    uint64_t ret = 0;
    m_extractor(reinterpret_cast<unsigned char*>(&ret), sizeof(uint64_t));
    return ret;
}
char DataBuffer::ReadChar()
{
    return static_cast<char>(ExtractByte());
}
unsigned char DataBuffer::ReadUChar()
{
    return ExtractByte();
}
// ReSharper disable once CppMemberFunctionMayBeConst
short DataBuffer::ReadShort()
{
    short ret = 0;
    m_extractor(reinterpret_cast<unsigned char *>(&ret), sizeof(short));
    return ret;
}
// ReSharper disable once CppMemberFunctionMayBeConst
unsigned short DataBuffer::ReadUShort()
{
    unsigned short ret = 0;
    m_extractor(reinterpret_cast<unsigned char *>(&ret), sizeof(unsigned short));
    return ret;
}
// ReSharper disable once CppMemberFunctionMayBeConst
int DataBuffer::ReadInt()
{
    int ret = 0;
    m_extractor(reinterpret_cast<unsigned char *>(&ret), sizeof(int));
    return ret;
}
// ReSharper disable once CppMemberFunctionMayBeConst
unsigned int DataBuffer::ReadUInt()
{
    unsigned int ret = 0;
    m_extractor(reinterpret_cast<unsigned char *>(&ret), sizeof(unsigned int));
    return ret;
}
// ReSharper disable once CppMemberFunctionMayBeConst
long DataBuffer::ReadLong()
{
    long ret = 0;
    m_extractor(reinterpret_cast<unsigned char *>(&ret), sizeof(long));
    return ret;
}
// ReSharper disable once CppMemberFunctionMayBeConst
unsigned long DataBuffer::ReadULong()
{
    unsigned long ret = 0;
    m_extractor(reinterpret_cast<unsigned char *>(&ret), sizeof(unsigned long));
    return ret;
}

std::string DataBuffer::ReadString(const size_t len)
{
    if (Size() <len)
    {
        throw DataBufferException(
                SC() << "Request to read String (" << len <<") larger than DataBuffer (" << Size() <<")");
    }

    std::string ret(reinterpret_cast<const char*>(&m_data[m_frontPos]), len);
    m_frontPos += static_cast<Buffer::difference_type>(len);
    return ret;
}
std::string DataBuffer::ReadNullTerminatedString()
{
    std::string ret;
    while (Size()> 0)
    {
        unsigned char uc = ExtractByte();
        if (uc == '\0')
        {
            break;
        }

        ret.append(1, static_cast<char>(uc));
    }

    return ret;
}
void DataBuffer::WriteInt8(const int8_t i)
{
    m_data.push_back(static_cast<unsigned char>(i));
}
void DataBuffer::WriteUInt8(const uint8_t ui)
{
    m_data.push_back(static_cast<unsigned char>(ui));
}
// ReSharper disable once CppMemberFunctionMayBeConst
void DataBuffer::WriteInt16(const int16_t i)
{
    m_appender(reinterpret_cast<const unsigned char*>(&i), sizeof(int16_t));
}
// ReSharper disable once CppMemberFunctionMayBeConst
void DataBuffer::WriteUInt16(const uint16_t ui)
{
    m_appender(reinterpret_cast<const unsigned char*>(&ui), sizeof(uint16_t));
}
// ReSharper disable once CppMemberFunctionMayBeConst
void DataBuffer::WriteInt32(const int32_t i)
{
    m_appender(reinterpret_cast<const unsigned char*>(&i), sizeof(int32_t));
}
// ReSharper disable once CppMemberFunctionMayBeConst
void DataBuffer::WriteUInt32(const uint32_t ui)
{
    m_appender(reinterpret_cast<const unsigned char*>(&ui), sizeof(uint32_t));
}
// ReSharper disable once CppMemberFunctionMayBeConst
void DataBuffer::WriteInt64(const int64_t i)
{
    m_appender(reinterpret_cast<const unsigned char*>(&i), sizeof(int64_t));
}
// ReSharper disable once CppMemberFunctionMayBeConst
void DataBuffer::WriteUInt64(const uint64_t ui)
{
    m_appender(reinterpret_cast<const unsigned char*>(&ui), sizeof(uint64_t));
}
void DataBuffer::WriteChar(const char c)
{
    m_data.push_back(static_cast<unsigned char>(c));
}
void DataBuffer::WriteUChar(const unsigned char uc)
{
    m_data.push_back(uc);
}
// ReSharper disable once CppMemberFunctionMayBeConst
void DataBuffer::WriteShort(const short s)
{
    m_appender(reinterpret_cast<const unsigned char*>(&s), sizeof(short));
}
// ReSharper disable once CppMemberFunctionMayBeConst
void DataBuffer::WriteUShort(const unsigned short us)
{
    m_appender(reinterpret_cast<const unsigned char*>(&us), sizeof(unsigned short));
}
// ReSharper disable once CppMemberFunctionMayBeConst
void DataBuffer::WriteInt(const int i)
{
    m_appender(reinterpret_cast<const unsigned char*>(&i), sizeof(int));
}
// ReSharper disable once CppMemberFunctionMayBeConst
void DataBuffer::WriteUInt(const unsigned int ui)
{
    m_appender(reinterpret_cast<const unsigned char*>(&ui), sizeof(unsigned int));
}
// ReSharper disable once CppMemberFunctionMayBeConst
void DataBuffer::WriteLong(const long l)
{
    m_appender(reinterpret_cast<const unsigned char*>(&l), sizeof(long));
}
// ReSharper disable once CppMemberFunctionMayBeConst
void DataBuffer::WriteULong(const unsigned long ul)
{
    m_appender(reinterpret_cast<const unsigned char*>(&ul), sizeof(unsigned long));
}
Buffer DataBuffer::ReadBuffer()
{
    Buffer ret;
    ret.resize(m_data.size() - m_frontPos);
    if (!ret.empty())
    {
        memcpy(&ret[0], &m_data[m_frontPos], m_data.size() - m_frontPos);
        m_frontPos += (static_cast<Buffer::difference_type>(m_data.size()) - m_frontPos);
    }
    return ret;
}
Buffer DataBuffer::ReadBuffer(const size_t len)
{
    Buffer ret;
    if (Size() < len)
    {
        throw DataBufferException(
                SC() << "Request to read Buffer (" << len << ") larger than DataBuffer (" << Size() << ")");
    }
    ret.resize(len);
    memcpy(&ret[0], &m_data[m_frontPos], len);
    m_frontPos += static_cast<Buffer::difference_type>(len);
    return ret;
}
void DataBuffer::WriteBuffer(const Buffer& buf)
{
    WriteUCharArray(&buf[0], buf.size(), false);
}
void DataBuffer::WriteBuffer(const Buffer& buf, const size_t len)
{
    WriteUCharArray(&buf[0], len, false);
}
void DataBuffer::WriteCharArray(const char* c, const bool keepNullTerminator)
{
    WriteCharArray(c, strlen(c), keepNullTerminator);
}
void DataBuffer::WriteCharArray(const char* c, const size_t len, const bool addNullTerminator)
{
    const size_t curSize = m_data.size();
    m_data.resize(curSize + len, '\0');
    memcpy(&m_data[curSize], c, len);
    if (addNullTerminator)
    {
        m_data.push_back('\0');
    }
}
void DataBuffer::WriteUCharArray(const unsigned char* uc, const bool keepNullTerminator)
{
    WriteCharArray(reinterpret_cast<const char *>(uc), keepNullTerminator);
}
void DataBuffer::WriteUCharArray(const unsigned char* uc, const size_t len, const bool addNullTerminator)
{
    WriteCharArray(reinterpret_cast<const char *>(uc), len, addNullTerminator);
}
void DataBuffer::WriteString(const std::string& s, const bool addNullTerminator)
{
    WriteCharArray(s.c_str(), s.length(), addNullTerminator);
}
void DataBuffer::WriteString(const std::string& s, const size_t len, const bool addNullTerminator)
{
    WriteCharArray(s.c_str(), len, addNullTerminator);
}

unsigned char DataBuffer::ExtractByte()
{
    if (Empty())
    {
        throw DataBufferException("Attempt to extract byte from empty DataBuffer");
    }
    const unsigned char ret = m_data[m_frontPos];
    m_frontPos++;
    return ret;
}
void DataBuffer::ByteSwapExtractInteger(unsigned char* ptr, const size_t numBytes)
{
    for (size_t i = numBytes; i> 0; i--)
    {
        ptr[i - 1] = ExtractByte();
    }
}
void DataBuffer::NormalExtractInteger(unsigned char* ptr, const size_t numBytes)
{
    for (size_t i = 0; i <numBytes; i++)
    {
        ptr[i] = ExtractByte();
    }
}
void DataBuffer::ByteSwapAppendInteger(const unsigned char* ptr, const size_t numBytes)
{
    for (size_t i = numBytes; i> 0; i--)
    {
        WriteUChar(ptr[i - 1]);
    }
}
void DataBuffer::NormalAppendInteger(const unsigned char* ptr, const size_t numBytes)
{
    for (size_t i = 0; i <numBytes; i++)
    {
        WriteUChar(ptr[i]);
    }
}

} // Magenta
