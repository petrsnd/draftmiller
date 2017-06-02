#pragma once

// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "Exception.h"
#include "Primitives.h"

#include <string>
#include <vector>
#include <functional>
#include <cstdint>
#if defined( _MSC_VER )
#define _INT8_T 1
#define _UINT8_T 1
#define _INT16_T 1
#define _UINT16_T 1
#define _INT32_T 1
#define _UINT32_T 1
#define _INT64_T 1
#define _UINT64_T 1
#endif

namespace Magenta {

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
    DataBuffer( const Endianness e = BigEndian );
    DataBuffer( const unsigned char* uc, const size_t len, const Endianness e = BigEndian );
    DataBuffer( const Buffer& b, const Endianness e = BigEndian );
    DataBuffer( const DataBuffer& d );
    ~DataBuffer();
    DataBuffer& operator=( const DataBuffer& d );

    void Clear();
    size_t Size() const;
    bool Empty() const;
    unsigned char& operator[]( size_t n );
    const unsigned char& operator[]( size_t n ) const;

#if defined( _INT8_T )
    int8_t ReadInt8();
    void WriteInt8( const int8_t i );
#endif
#if defined( _UINT8_T )
    uint8_t ReadUInt8();
    void WriteUInt8( const uint8_t ui );
#endif
#if defined( _INT16_T )
    int16_t ReadInt16();
    void WriteInt16( const int16_t i );
#endif
#if defined( _UINT16_T )
    uint16_t ReadUInt16();
    void WriteUInt16( const uint16_t ui );
#endif
#if defined( _INT32_T )
    int32_t ReadInt32();
    void WriteInt32( const int32_t i );
#endif
#if defined( _UINT32_T )
    uint32_t ReadUInt32();
    void WriteUInt32( const uint32_t ui );
#endif
#if defined( _INT64_T )
    int64_t ReadInt64();
    void WriteInt64( const int64_t i );
#endif
#if defined( _UINT64_T )
    uint64_t ReadUInt64();
    void WriteUInt64( const uint64_t ui );
#endif
    char ReadChar();
    void WriteChar( const char c );

    unsigned char ReadUChar();
    void WriteUChar( const unsigned char uc );

    short ReadShort();
    void WriteShort( const short s );

    unsigned short ReadUShort();
    void WriteUShort( const unsigned short us );

    int ReadInt();
    void WriteInt( const int i );

    unsigned int ReadUInt();
    void WriteUInt( const unsigned int ui );

    long ReadLong();
    void WriteLong( const long l );

    unsigned long ReadULong();
    void WriteULong( const unsigned long ul );

    char* ReadCharArray();
    void WriteCharArray( const char* c, bool keepNullTerminator = true );
    char* ReadCharArray( const size_t len );
    void WriteCharArray( const char* c, const size_t len, bool addNullTerminator = false );

    char* ReadUCharArray();
    void WriteUCharArray( const unsigned char* uc, bool keepNullTerminator = true );
    unsigned char* ReadUCharArray( const size_t len );
    void WriteUCharArray( const unsigned char* uc, const size_t len, bool addNullTerminator = false );

    std::string ReadString( const size_t len );
    std::string ReadNullTerminatedString();
    void WriteString( const std::string& s, bool addNullTerminator = true );
private:
    unsigned char ExtractByte();
    void ByteSwapExtractInteger( unsigned char* ptr, size_t numBytes );
    void NormalExtractInteger( unsigned char* ptr, size_t numBytes );
    void ByteSwapAppendInteger( const unsigned char* ptr, size_t numBytes );
    void NormalAppendInteger( const unsigned char* ptr, size_t numBytes );

    std::vector< unsigned char > m_data;
    size_t m_frontPos;
    Endianness m_endianness;
    std::function< void ( unsigned char*, size_t ) > m_extractor;
    std::function< void ( const unsigned char*, size_t ) > m_appender;
};

} // Magenta

