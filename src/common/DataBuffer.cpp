// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DataBuffer.h"

#include <cstdlib>

namespace Magenta {

void DataBuffer::InitExtractorAndAppender()
{
#if defined( WORDS_BIGENDIAN )
    if ( m_endianness == BigEndian )
    {
        m_extractor = std::bind( &Magenta::DataBuffer::NormalExtractInteger, this, std::placeholders::_1, std::placeholders::_2 );
        m_appender = std::bind( &Magenta::DataBuffer::NormalAppendInteger, this, std::placeholders::_1, std::placeholders::_2 );
    }
    else
    {
        m_extractor = std::bind( &Magenta::DataBuffer::ByteSwapExtractInteger, this, std::placeholders::_1, std::placeholders::_2 );
        m_appender = std::bind( &Magenta::DataBuffer::ByteSwapAppendInteger, this, std::placeholders::_1, std::placeholders::_2 );
    }
#else
    if ( m_endianness == BigEndian )
    {
        m_extractor = std::bind( &Magenta::DataBuffer::ByteSwapExtractInteger, this, std::placeholders::_1, std::placeholders::_2 );
        m_appender = std::bind( &Magenta::DataBuffer::ByteSwapAppendInteger, this, std::placeholders::_1, std::placeholders::_2 );
    }
    else
    {
        m_extractor = std::bind( &Magenta::DataBuffer::NormalExtractInteger, this, std::placeholders::_1, std::placeholders::_2 );
        m_appender = std::bind( &Magenta::DataBuffer::NormalAppendInteger, this, std::placeholders::_1, std::placeholders::_2 );
    }
#endif
}
DataBuffer::DataBuffer( DataBuffer::Endianness e ) :
    m_data(),
    m_frontPos( 0 ),
    m_endianness( e )
{
    InitExtractorAndAppender();
}
DataBuffer::DataBuffer( const unsigned char* uc, const size_t len, const Endianness e ) :
    m_data(),
    m_frontPos( 0 ),
    m_endianness( e )
{
    WriteCharArray( ( const char* )uc, len, false );
    InitExtractorAndAppender();
}
DataBuffer::DataBuffer( const Buffer& b, const Endianness e ) :
    m_data( b.begin(), b.end() ),
    m_frontPos( 0 ),
    m_endianness( e )
{
    InitExtractorAndAppender();
}
DataBuffer::DataBuffer( const DataBuffer& d ) :
    m_data( d.m_data.begin() + d.m_frontPos, d.m_data.end() ), 
    m_frontPos( 0 ),
    m_endianness( d.m_endianness )
{
    InitExtractorAndAppender();
}
DataBuffer::~DataBuffer()
{}
DataBuffer& DataBuffer::operator=( const DataBuffer& d )
{
    m_data = std::vector< unsigned char >( d.m_data.begin() + d.m_frontPos, d.m_data.end() );
    m_frontPos = 0;
    m_endianness = d.m_endianness;
    InitExtractorAndAppender();
    return *this;
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
unsigned char& DataBuffer::operator[]( size_t n )
{
    return m_data[m_frontPos + n];
}
const unsigned char& DataBuffer::operator[]( size_t n ) const
{
    return m_data[m_frontPos + n];
}
#if defined( _INT8_T )
int8_t DataBuffer::ReadInt8()
{
    return ( int8_t )ExtractByte();
}
#endif
#if defined( _UINT8_T )
uint8_t DataBuffer::ReadUInt8()
{
    return ( uint8_t )ExtractByte();
}
#endif
#if defined( _INT16_T )
int16_t DataBuffer::ReadInt16()
{
    int16_t ret = 0;
    m_extractor( ( unsigned char* )&ret, sizeof( int16_t ) );
    return ret;
}
#endif
#if defined( _UINT16_T )
uint16_t DataBuffer::ReadUInt16()
{
    uint16_t ret = 0;
    m_extractor( ( unsigned char* )&ret, sizeof( uint16_t ) );
    return ret;
}
#endif
#if defined( _INT32_T )
int32_t DataBuffer::ReadInt32()
{
    int32_t ret = 0;
    m_extractor( ( unsigned char* )&ret, sizeof( int32_t ) );
    return ret;
}
#endif
#if defined( _UINT32_T )
uint32_t DataBuffer::ReadUInt32()
{
    uint32_t ret = 0;
    m_extractor( ( unsigned char* )&ret, sizeof( uint32_t ) );
    return ret;
}
#endif
#if defined( _INT64_T )
int64_t DataBuffer::ReadInt64()
{
    int64_t ret = 0;
    m_extractor( ( unsigned char* )&ret, sizeof( int64_t ) );
    return ret;
}
#endif
#if defined( _UINT64_T )
uint64_t DataBuffer::ReadUInt64()
{
    uint64_t ret = 0;
    m_extractor( ( unsigned char* )&ret, sizeof( uint64_t ) );
    return ret;
}
#endif
char DataBuffer::ReadChar()
{
    return ( char )ExtractByte();
}
unsigned char DataBuffer::ReadUChar()
{
    return ExtractByte();
}
short DataBuffer::ReadShort()
{
    short ret = 0;
    m_extractor( ( unsigned char* )&ret, sizeof( short ) );
    return ret;
}
unsigned short DataBuffer::ReadUShort()
{
    unsigned short ret = 0;
    m_extractor( ( unsigned char* )&ret, sizeof( unsigned short ) );
    return ret;
}
int DataBuffer::ReadInt()
{
    int ret = 0;
    m_extractor( ( unsigned char* )&ret, sizeof( int ) );
    return ret;
}
unsigned int DataBuffer::ReadUInt()
{
    unsigned int ret = 0;
    m_extractor( ( unsigned char* )&ret, sizeof( unsigned int ) );
    return ret;
}
long DataBuffer::ReadLong()
{
    long ret = 0;
    m_extractor( ( unsigned char* )&ret, sizeof( long ) );
    return ret;
}
unsigned long DataBuffer::ReadULong()
{
    unsigned long ret = 0;
    m_extractor( ( unsigned char* )&ret, sizeof( unsigned long ) );
    return ret;
}
char* DataBuffer::ReadCharArray( const size_t len )
{
    size_t cpyLen = ( len < Size() ? len : Size() );
    char* ret = ( char* )calloc( sizeof( char ), len + 1);
    memcpy( ret, &m_data[m_frontPos], cpyLen );
    m_frontPos += cpyLen;
    return ret;
}
unsigned char* DataBuffer::ReadUCharArray( const size_t len )
{
    return ( unsigned char* )ReadCharArray( len );
}
std::string DataBuffer::ReadString( const size_t len )
{
    char* c = ReadCharArray( len );
    return std::string( c, len );
}
std::string DataBuffer::ReadNullTerminatedString()
{
    std::string ret;
    while ( Size() > 0 )
    {
        unsigned char uc = ExtractByte();
        if ( uc == '\0' )
        {
            break;
        }
        ret.append( 1, ( char )uc );
    }
    return ret;
}
#if defined( _INT8_T )
void DataBuffer::WriteInt8( const int8_t i )
{
    m_data.push_back( ( unsigned char )i );
}
#endif
#if defined( _UINT8_T )
void DataBuffer::WriteUInt8( const uint8_t ui )
{
    m_data.push_back( ( unsigned char )ui );
}
#endif
#if defined( _INT16_T )
void DataBuffer::WriteInt16( const int16_t i )
{
    m_appender( ( const unsigned char* )&i, sizeof( int16_t ) );
}
#endif
#if defined( _UINT16_T )
void DataBuffer::WriteUInt16( const uint16_t ui )
{
    m_appender( ( const unsigned char* )&ui, sizeof( uint16_t ) );
}
#endif
#if defined( _INT32_T )
void DataBuffer::WriteInt32( const int32_t i )
{
    m_appender( ( const unsigned char* )&i, sizeof( int32_t ) );
}
#endif
#if defined( _UINT32_T )
void DataBuffer::WriteUInt32( const uint32_t ui )
{
    m_appender( ( const unsigned char* )&ui, sizeof( uint32_t ) );
}
#endif
#if defined( _INT64_T )
void DataBuffer::WriteInt64( const int64_t i )
{
    m_appender( ( const unsigned char* )&i, sizeof( int64_t ) );
}
#endif
#if defined( _UINT64_T )
void DataBuffer::WriteUInt64( const uint64_t ui )
{
    m_appender( ( const unsigned char* )&ui, sizeof( uint64_t ) );
}
#endif
void DataBuffer::WriteChar( const char c )
{
    m_data.push_back( ( unsigned char )c );
}
void DataBuffer::WriteUChar( const unsigned char uc )
{
    m_data.push_back( uc );
}
void DataBuffer::WriteShort( const short s )
{
    m_appender( ( const unsigned char* )&s, sizeof( short ) );
}
void DataBuffer::WriteUShort( const unsigned short us )
{
    m_appender( ( const unsigned char* )&us, sizeof( unsigned short ) );
}
void DataBuffer::WriteInt( const int i )
{
    m_appender( ( const unsigned char* )&i, sizeof( int ) );
}
void DataBuffer::WriteUInt( const unsigned int ui )
{
    m_appender( ( const unsigned char* )&ui, sizeof( unsigned int ) );
}
void DataBuffer::WriteLong( const long l )
{
    m_appender( ( const unsigned char* )&l, sizeof( long ) );
}
void DataBuffer::WriteULong( const unsigned long ul )
{
    m_appender( ( const unsigned char* )&ul, sizeof( unsigned long ) );
}
Buffer DataBuffer::ReadBuffer( const size_t len )
{
    Buffer ret;

    while ( Size() > 0 )
    {
        unsigned char uc = ExtractByte();
        ret.push_back( uc );
    }
    return ret;
}
char* DataBuffer::ReadCharArray()
{
    Buffer ret;
    while ( Size() > 0 )
    {
        unsigned char uc = ExtractByte();
        if ( uc == '\0' )
        {
            break;
        }
        ret.push_back( uc );
    }
    ret.push_back( '\0' );
    char* arr = ( char* )calloc( sizeof( char ), ret.size());
    memcpy( arr, &ret[0], ret.size() );
    return arr;
}
void DataBuffer::WriteCharArray( const char* c, bool keepNullTerminator )
{
    WriteCharArray( c, strlen( c ), keepNullTerminator );
}
void DataBuffer::WriteCharArray( const char* c, const size_t len, bool addNullTerminator )
{
    size_t curSize = m_data.size();
    m_data.resize( curSize + len, '\0' );
    memcpy( &m_data[curSize], c, len );
    if ( addNullTerminator )
    {
        m_data.push_back( '\0' );
    }
}
unsigned char* DataBuffer::ReadUCharArray()
{
    Buffer ret;
    while ( Size() > 0 )
    {
        unsigned char uc = ExtractByte();
        if ( uc == '\0' )
        {
            break;
        }
        ret.push_back( uc );
    }
    ret.push_back( '\0' );
    unsigned char* arr = ( unsigned char* )calloc( sizeof( unsigned char ), ret.size());
    memcpy( arr, &ret[0], ret.size() );
    return arr;
}
void DataBuffer::WriteUCharArray( const unsigned char* uc, bool keepNullTerminator )
{
    WriteCharArray( ( const char* )uc, keepNullTerminator );
}
void DataBuffer::WriteUCharArray( const unsigned char* uc, const size_t len, bool addNullTerminator )
{
    WriteCharArray( ( const char* )uc, len, addNullTerminator );
}
void DataBuffer::WriteString( const std::string& s, bool addNullTerminator )
{
    WriteCharArray( s.c_str(), s.length(), addNullTerminator );
}

unsigned char DataBuffer::ExtractByte()
{
    if ( Empty() )
    {
        throw DataBufferException( "Attempt to extract byte from empty DataBuffer" );
    }
    unsigned char ret = m_data[m_frontPos];
    m_frontPos++;
    return ret;
}
void DataBuffer::ByteSwapExtractInteger( unsigned char* ptr, size_t numBytes )
{
    for ( size_t i = numBytes; i > 0; i-- )
    {
        ptr[i - 1] = ExtractByte();
    }
}
void DataBuffer::NormalExtractInteger( unsigned char* ptr, size_t numBytes )
{
    for ( size_t i = 0; i < numBytes; i++ )
    {
        ptr[i] = ExtractByte();
    }
}
void DataBuffer::ByteSwapAppendInteger( const unsigned char* ptr, size_t numBytes )
{
    for ( size_t i = numBytes; i > 0; i-- )
    {
        WriteUChar( ptr[i - 1] );
    }
}
void DataBuffer::NormalAppendInteger( const unsigned char* ptr, size_t numBytes )
{
    for ( size_t i = 0; i < numBytes; i++ )
    {
        WriteUChar( ptr[i] );
    }
}

} // Magenta

