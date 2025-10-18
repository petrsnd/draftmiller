#pragma once

// (c) 2025 petrsnd@secureidx.com.  All rights reserved.

#include "Exception.h"
#include "Primitives.h"

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace DraftMiller {

class DataBufferException final : public std::runtime_error
{
public:
    explicit DataBufferException(const std::string& msg);
    ~DataBufferException() override = default;
};

class DataBuffer
{
private:
    void InitExtractorAndAppender();

public:
    enum Endianness
    {
        BigEndian = 1, // network byte order
        LittleEndian = 2
    };
    explicit DataBuffer(Endianness e = BigEndian);
    DataBuffer(const unsigned char* uc, size_t len, Endianness e = BigEndian);
    explicit DataBuffer(const Buffer& b, Endianness e = BigEndian);
    DataBuffer(const DataBuffer& d);
    ~DataBuffer();
    DataBuffer& operator=(const DataBuffer& d);

    operator Buffer() const;  // NOLINT(*-explicit-constructor)

    void Clear();
    size_t Size() const;
    bool Empty() const;
    unsigned char& operator[](size_t n);
    const unsigned char& operator[](size_t n) const;

    int8_t ReadInt8();
    void WriteInt8(int8_t i);
    uint8_t ReadUInt8();
    void WriteUInt8(uint8_t ui);
    int16_t ReadInt16();
    void WriteInt16(int16_t i);
    uint16_t ReadUInt16();
    void WriteUInt16(uint16_t ui);
    int32_t ReadInt32();
    void WriteInt32(int32_t i);
    uint32_t ReadUInt32();
    void WriteUInt32(uint32_t ui);
    int64_t ReadInt64();
    void WriteInt64(int64_t i);
    uint64_t ReadUInt64();
    void WriteUInt64(uint64_t ui);
    char ReadChar();
    void WriteChar(char c);

    unsigned char ReadUChar();
    void WriteUChar(unsigned char uc);

    short ReadShort();
    void WriteShort(short s);

    unsigned short ReadUShort();
    void WriteUShort(unsigned short us);

    int ReadInt();
    void WriteInt(int i);

    unsigned int ReadUInt();
    void WriteUInt(unsigned int ui);

    long ReadLong();
    void WriteLong(long l);

    unsigned long ReadULong();
    void WriteULong(unsigned long ul);

    Buffer ReadBuffer();
    Buffer ReadBuffer(size_t len);
    void WriteBuffer(const Buffer& buf);
    void WriteBuffer(const Buffer& buf, size_t len);

    void WriteCharArray(const char* c, bool keepNullTerminator = true);
    void WriteCharArray(const char* c, size_t len, bool addNullTerminator = false);

    void WriteUCharArray(const unsigned char* uc, bool keepNullTerminator = true);
    void WriteUCharArray(const unsigned char* uc, size_t len, bool addNullTerminator = false);

    std::string ReadString(size_t len);
    std::string ReadNullTerminatedString();
    void WriteString(const std::string& s, bool addNullTerminator = true);
    void WriteString(const std::string& s, size_t len, bool addNullTerminator = true);

private:
    unsigned char ExtractByte();
    void ByteSwapExtractInteger(unsigned char* ptr, size_t numBytes);
    void NormalExtractInteger(unsigned char* ptr, size_t numBytes);
    void ByteSwapAppendInteger(const unsigned char* ptr, size_t numBytes);
    void NormalAppendInteger(const unsigned char* ptr, size_t numBytes);

    Buffer m_data;
    Buffer::difference_type m_frontPos;
    Endianness m_endianness;
    std::function<void (unsigned char*, size_t)> m_extractor;
    std::function<void (const unsigned char*, size_t)> m_appender;
};

} // Magenta
