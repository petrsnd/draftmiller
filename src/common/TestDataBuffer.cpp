// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DataBuffer.h"

#include "UnitTestSuite.h"

#include <cstdlib>

using namespace Magenta;

// DataBuffer.h
static void TestDataBufferConstructFromUnsignedCharArray()
{
    unsigned char data[] = { '\01', '\02', '\03', '\04' };
    DataBuffer buf( data, 4 );
    int8_t expected1 = 1;
    int8_t expected2 = 2;
    int8_t expected3 = 3;
    int8_t expected4 = 4;
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected1 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected2 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected3 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected4 );
    ASSERT_TRUE( buf.Empty() );
}
static void TestDataBufferConstructFromBuffer()
{
    Buffer buffer;
    buffer.push_back( '\01' );
    buffer.push_back( '\02' );
    buffer.push_back( '\03' );
    buffer.push_back( '\04' );
    DataBuffer buf( buffer );
    int8_t expected1 = 1;
    int8_t expected2 = 2;
    int8_t expected3 = 3;
    int8_t expected4 = 4;
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected1 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected2 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected3 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected4 );
    ASSERT_TRUE( buf.Empty() );
}
static void TestDataBufferCopyConstructor()
{
    unsigned char data[] = { '\01', '\02', '\03', '\04' };
    DataBuffer buf( data, 4 );
    DataBuffer buf2( buf );
    int8_t expected1 = 1;
    int8_t expected2 = 2;
    int8_t expected3 = 3;
    int8_t expected4 = 4;
    ASSERT_ARE_EQUAL( buf2.ReadInt8(), expected1 );
    ASSERT_ARE_EQUAL( buf2.ReadInt8(), expected2 );
    ASSERT_ARE_EQUAL( buf2.ReadInt8(), expected3 );
    ASSERT_ARE_EQUAL( buf2.ReadInt8(), expected4 );
    ASSERT_TRUE( buf2.Empty() );
}
static void TestDataBufferAssignmentOperator()
{
    unsigned char data[] = { '\01', '\02', '\03', '\04' };
    DataBuffer buf( data, 4 );
    DataBuffer buf2 = buf;
    int8_t expected1 = 1;
    int8_t expected2 = 2;
    int8_t expected3 = 3;
    int8_t expected4 = 4;
    ASSERT_ARE_EQUAL( buf2.ReadInt8(), expected1 );
    ASSERT_ARE_EQUAL( buf2.ReadInt8(), expected2 );
    ASSERT_ARE_EQUAL( buf2.ReadInt8(), expected3 );
    ASSERT_ARE_EQUAL( buf2.ReadInt8(), expected4 );
    ASSERT_TRUE( buf2.Empty() );
}
static void TestDataBufferClear()
{
    unsigned char data[] = { '\01', '\02', '\03', '\04' };
    DataBuffer buf( data, 4 );
    buf.WriteInt8( 0x7f );
    buf.WriteInt8( 0x70 );
    buf.WriteInt8( 0x0f );
    buf.WriteInt8( 0x00 );
    ASSERT_FALSE( buf.Empty() );
    buf.Clear();
    ASSERT_TRUE( buf.Empty() );
    buf.WriteUCharArray( data, 4, false );
    ASSERT_FALSE( buf.Empty() );
    buf.Clear();
    ASSERT_TRUE( buf.Empty() );
}
static void TestDataBufferSize()
{
    unsigned char data[] = { '\01', '\02', '\03', '\04' };
    DataBuffer buf( data, 4 );
    ASSERT_ARE_EQUAL( buf.Size(), 4 );
    buf.WriteInt8( 0x7f );
    buf.WriteInt8( 0x70 );
    buf.WriteInt8( 0x0f );
    buf.WriteInt8( 0x00 );
    ASSERT_ARE_EQUAL( buf.Size(), 8 );
    buf.Clear();
    ASSERT_ARE_EQUAL( buf.Size(), 0 );
    buf.WriteUCharArray( data, 4, false );
    ASSERT_ARE_EQUAL( buf.Size(), 4 );
    buf.WriteInt32( 0 );
    ASSERT_ARE_EQUAL( buf.Size(), 8 );
    int8_t expected1 = 1;
    int8_t expected2 = 2;
    int8_t expected3 = 3;
    int8_t expected4 = 4;
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected1 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected2 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected3 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected4 );
    ASSERT_ARE_EQUAL( buf.Size(), 4 );
    int32_t expected5 = 0;
    ASSERT_ARE_EQUAL( buf.ReadInt32(), expected5 );
    ASSERT_ARE_EQUAL( buf.Size(), 0 );
}
static void TestDataBufferBracketOperator()
{
    unsigned char data[] = { '\01', '\02', '\03', '\04' };
    DataBuffer buf( data, 4 );
    buf.WriteInt8( 0x7f );
    buf.WriteInt8( ( int8_t )0x80 );
    buf.WriteInt8( ( int8_t )0xff );
    buf.WriteInt8( 0x00 );
    unsigned char expected1 = '\01';
    unsigned char expected2 = '\04';
    unsigned char expected3 = 0x80;
    int8_t expected4 = 1;
    unsigned char expected5 = 0xff;
    ASSERT_ARE_EQUAL( buf[0], expected1 );
    ASSERT_ARE_EQUAL( buf[3], expected2 );
    ASSERT_ARE_EQUAL( buf[5], expected3 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected4 );
    ASSERT_ARE_EQUAL( buf[5], expected5 );
}
static void TestDataBufferReadWriteInt8()
{
#if defined( _INT8_T )
    unsigned char data[] = { '\01', '\02', '\03', '\04' };
    DataBuffer buf;
    buf.WriteUCharArray( data, 4, false );
    int8_t expected1 = 1;
    int8_t expected2 = 2;
    int8_t expected3 = 3;
    int8_t expected4 = 4;
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected1 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected2 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected3 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), expected4 );
    ASSERT_TRUE( buf.Empty() );
    buf.WriteInt8( 0x7f );
    buf.WriteInt8( ( int8_t )0x80 );
    buf.WriteInt8( ( int8_t )0xff );
    buf.WriteInt8( 0x00 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), ( int8_t )127 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), ( int8_t )-128 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), ( int8_t )-1 );
    ASSERT_ARE_EQUAL( buf.ReadInt8(), ( int8_t )0 );
    ASSERT_TRUE( buf.Empty() );
    DataBuffer buf2;
    ASSERT_TRUE( buf2.Empty() );
    ASSERT_ARE_EQUAL( buf2.Size(), 0 );
    buf2.WriteInt8( 0x00 );
    ASSERT_FALSE( buf2.Empty() );
    ASSERT_ARE_EQUAL( buf2.Size(), 1 );
    buf2.WriteInt8( 0x7f );
    ASSERT_FALSE( buf2.Empty() );
    ASSERT_ARE_EQUAL( buf2.Size(), 2 );
    ASSERT_ARE_EQUAL( buf2.ReadInt8(), 0x00 );
    ASSERT_FALSE( buf2.Empty() );
    ASSERT_ARE_EQUAL( buf2.Size(), 1 );
    ASSERT_ARE_EQUAL( buf2.ReadInt8(), 0x7f );
    ASSERT_TRUE( buf2.Empty() );
    ASSERT_ARE_EQUAL( buf2.Size(), 0 );
#endif
}
static void TestDataBufferReadWriteUInt8()
{
#if defined( _UINT8_T )
    unsigned char data[] = { 0x01, 0x02, 0x03, 0x04 };
    DataBuffer buf;
    buf.WriteUCharArray( data, 4, false );
    uint8_t expected1 = 1;
    uint8_t expected2 = 2;
    uint8_t expected3 = 3;
    uint8_t expected4 = 4;
    ASSERT_ARE_EQUAL( buf.ReadUInt8(), expected1 );
    ASSERT_ARE_EQUAL( buf.ReadUInt8(), expected2 );
    ASSERT_ARE_EQUAL( buf.ReadUInt8(), expected3 );
    ASSERT_ARE_EQUAL( buf.ReadUInt8(), expected4 );
    ASSERT_TRUE( buf.Empty() );
    buf.WriteInt8( 0x7f );
    buf.WriteInt8( ( int8_t )0x80 );
    buf.WriteInt8( ( int8_t )0xff );
    buf.WriteInt8( 0x00 );
    ASSERT_ARE_EQUAL( buf.ReadUInt8(), ( uint8_t )127 );
    ASSERT_ARE_EQUAL( buf.ReadUInt8(), ( uint8_t )128 );
    ASSERT_ARE_EQUAL( buf.ReadUInt8(), ( uint8_t )255 );
    ASSERT_ARE_EQUAL( buf.ReadUInt8(), ( uint8_t )0 );
    ASSERT_TRUE( buf.Empty() );
    DataBuffer buf2;
    ASSERT_TRUE( buf2.Empty() );
    ASSERT_ARE_EQUAL( buf2.Size(), 0 );
    buf2.WriteUInt8( 0x00 );
    ASSERT_FALSE( buf2.Empty() );
    ASSERT_ARE_EQUAL( buf2.Size(), 1 );
    buf2.WriteUInt8( 0xff );
    ASSERT_FALSE( buf2.Empty() );
    ASSERT_ARE_EQUAL( buf2.Size(), 2 );
    ASSERT_ARE_EQUAL( buf2.ReadUInt8(), 0x00 );
    ASSERT_FALSE( buf2.Empty() );
    ASSERT_ARE_EQUAL( buf2.Size(), 1 );
    ASSERT_ARE_EQUAL( buf2.ReadUInt8(), 0xff );
    ASSERT_TRUE( buf2.Empty() );
    ASSERT_ARE_EQUAL( buf2.Size(), 0 );
#endif
}
static void TestDataBufferReadWriteInt16()
{
#if defined( _INT16_T )
    unsigned char data[] = { 0x00, 0x01, 0x01, 0x00 };
    DataBuffer bigEndianBuf;
    DataBuffer littleEndianBuf( DataBuffer::LittleEndian );
    bigEndianBuf.WriteUCharArray( data, 4, false );
    littleEndianBuf.WriteUCharArray( data, 4, false );
    int16_t bigExpected1 = 1;
    int16_t bigExpected2 = 256;
    int16_t littleExpected1 = 256;
    int16_t littleExpected2 = 1;
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadInt16(), bigExpected1 );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadInt16(), bigExpected2 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadInt16(), littleExpected1 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadInt16(), littleExpected2 );
    DataBuffer bigEndianBuf2;
    ASSERT_TRUE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 0 );
    bigEndianBuf2.WriteInt16( INT16_C( 0x00ff ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 2 );
    bigEndianBuf2.WriteInt16( INT16_C( 0x7f00 ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 4 );
    ASSERT_ARE_EQUAL( bigEndianBuf2.ReadInt16(), INT16_C( 0x00ff ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 2 );
    ASSERT_ARE_EQUAL( bigEndianBuf2.ReadInt16(), INT16_C( 0x7f00 ) );
    ASSERT_TRUE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 0 );
    DataBuffer littleEndianBuf2( DataBuffer::LittleEndian );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
    littleEndianBuf2.WriteInt16( INT16_C( 0x00ff ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 2 );
    littleEndianBuf2.WriteInt16( INT16_C( 0x7f00 ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 4 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadInt16(), INT16_C( 0x00ff ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 2 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadInt16(), INT16_C( 0x7f00 ) );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
#endif
}
static void TestDataBufferReadWriteUInt16()
{
#if defined( _UINT16_T )
    unsigned char data[] = { 0x00, 0xff, 0xff, 0x00 };
    DataBuffer bigEndianBuf;
    DataBuffer littleEndianBuf( DataBuffer::LittleEndian );
    bigEndianBuf.WriteUCharArray( data, 4, false );
    littleEndianBuf.WriteUCharArray( data, 4, false );
    uint16_t bigExpected1 = 255;
    uint16_t bigExpected2 = 65280;
    uint16_t littleExpected1 = 65280;
    uint16_t littleExpected2 = 255;
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadUInt16(), bigExpected1 );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadUInt16(), bigExpected2 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadUInt16(), littleExpected1 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadUInt16(), littleExpected2 );
    DataBuffer bigEndianBuf2;
    ASSERT_TRUE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 0 );
    bigEndianBuf2.WriteUInt16( UINT16_C( 0x00ff ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 2 );
    bigEndianBuf2.WriteUInt16( UINT16_C( 0xff00 ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 4 );
    ASSERT_ARE_EQUAL( bigEndianBuf2.ReadUInt16(), UINT16_C( 0x00ff ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 2 );
    ASSERT_ARE_EQUAL( bigEndianBuf2.ReadUInt16(), UINT16_C( 0xff00 ) );
    ASSERT_TRUE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 0 );
    DataBuffer littleEndianBuf2( DataBuffer::LittleEndian );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
    littleEndianBuf2.WriteUInt16( UINT16_C( 0x00ff ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 2 );
    littleEndianBuf2.WriteUInt16( UINT16_C( 0xff00 ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 4 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadUInt16(), UINT16_C( 0x00ff ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 2 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadUInt16(), UINT16_C( 0xff00 ) );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
#endif
}
static void TestDataBufferReadWriteInt32()
{
#if defined( _INT32_T )
    unsigned char data[] = { 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00 };
    DataBuffer bigEndianBuf;
    DataBuffer littleEndianBuf( DataBuffer::LittleEndian );
    bigEndianBuf.WriteUCharArray( data, 8, false );
    littleEndianBuf.WriteUCharArray( data, 8, false );
    int32_t bigExpected1 = 65537;
    int32_t bigExpected2 = 16777472;
    int32_t littleExpected1 = 16777472;
    int32_t littleExpected2 = 65537;
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadInt32(), bigExpected1 );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadInt32(), bigExpected2 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadInt32(), littleExpected1 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadInt32(), littleExpected2 );
    DataBuffer bigEndianBuf2;
    ASSERT_TRUE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 0 );
    bigEndianBuf2.WriteInt32( INT32_C( 0x00ff00ff ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 4 );
    bigEndianBuf2.WriteInt32( INT32_C( 0x7f00ff00 ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 8 );
    ASSERT_ARE_EQUAL( bigEndianBuf2.ReadInt32(), INT32_C( 0x00ff00ff ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 4 );
    ASSERT_ARE_EQUAL( bigEndianBuf2.ReadInt32(), INT32_C( 0x7f00ff00 ) );
    ASSERT_TRUE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 0 );
    DataBuffer littleEndianBuf2( DataBuffer::LittleEndian );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
    littleEndianBuf2.WriteInt32( INT32_C( 0x00ff00ff ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 4 );
    littleEndianBuf2.WriteInt32( INT32_C( 0x7f00ff00 ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 8 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadInt32(), INT32_C( 0x00ff00ff ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 4 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadInt32(), INT32_C( 0x7f00ff00 ) );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
#endif
}
static void TestDataBufferReadWriteUInt32()
{
#if defined( _UINT32_T )
    unsigned char data[] = { 0x00, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0x00 };
    DataBuffer bigEndianBuf;
    DataBuffer littleEndianBuf( DataBuffer::LittleEndian );
    bigEndianBuf.WriteUCharArray( data, 8, false );
    littleEndianBuf.WriteUCharArray( data, 8, false );
    uint32_t bigExpected1 = 16711935;
    uint32_t bigExpected2 = 4278255360;
    uint32_t littleExpected1 = 4278255360;
    uint32_t littleExpected2 = 16711935;
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadUInt32(), bigExpected1 );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadUInt32(), bigExpected2 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadUInt32(), littleExpected1 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadUInt32(), littleExpected2 );
    DataBuffer bigEndianBuf2;
    ASSERT_TRUE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 0 );
    bigEndianBuf2.WriteInt32( UINT32_C( 0x00ff00ff ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 4 );
    bigEndianBuf2.WriteInt32( UINT32_C( 0xff00ff00 ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 8 );
    ASSERT_ARE_EQUAL( bigEndianBuf2.ReadUInt32(), UINT32_C( 0x00ff00ff ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 4 );
    ASSERT_ARE_EQUAL( bigEndianBuf2.ReadUInt32(), UINT32_C( 0xff00ff00 ) );
    ASSERT_TRUE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 0 );
    DataBuffer littleEndianBuf2( DataBuffer::LittleEndian );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
    littleEndianBuf2.WriteUInt32( UINT32_C( 0x00ff00ff ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 4 );
    littleEndianBuf2.WriteUInt32( UINT32_C( 0xff00ff00 ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 8 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadUInt32(), UINT32_C( 0x00ff00ff ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 4 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadUInt32(), UINT32_C( 0xff00ff00 ) );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
#endif
}
static void TestDataBufferReadWriteInt64()
{
#if defined( _INT64_T )
    unsigned char data[] = { 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
                             0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00 };
    DataBuffer bigEndianBuf;
    DataBuffer littleEndianBuf( DataBuffer::LittleEndian );
    bigEndianBuf.WriteUCharArray( data, 16, false );
    littleEndianBuf.WriteUCharArray( data, 16, false );
    int64_t bigExpected1 = 281479271743489;
    int64_t bigExpected2 = 72058693566333184;
    int64_t littleExpected1 = 72058693566333184;
    int64_t littleExpected2 = 281479271743489;
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadInt64(), bigExpected1 );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadInt64(), bigExpected2 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadInt64(), littleExpected1 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadInt64(), littleExpected2 );
    DataBuffer bigEndianBuf2;
    ASSERT_TRUE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 0 );
    bigEndianBuf2.WriteInt64( INT64_C( 0x00ff00ff00ff00ff ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 8 );
    bigEndianBuf2.WriteInt64( INT64_C( 0x7f00ff00ff00ff00 ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 16 );
    ASSERT_ARE_EQUAL( bigEndianBuf2.ReadInt64(), INT64_C( 0x00ff00ff00ff00ff ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 8 );
    ASSERT_ARE_EQUAL( bigEndianBuf2.ReadInt64(), INT64_C( 0x7f00ff00ff00ff00 ) );
    ASSERT_TRUE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 0 );
    DataBuffer littleEndianBuf2( DataBuffer::LittleEndian );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
    littleEndianBuf2.WriteInt64( INT64_C( 0x00ff00ff00ff00ff ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 8 );
    littleEndianBuf2.WriteInt64( INT64_C( 0x7f00ff00ff00ff00 ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 16 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadInt64(), INT64_C( 0x00ff00ff00ff00ff ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 8 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadInt64(), INT64_C( 0x7f00ff00ff00ff00 ) );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
#endif
}
static void TestDataBufferReadWriteUInt64()
{
#if defined( _UINT64_T )
    unsigned char data[] = { 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff,
                             0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00 };
    DataBuffer bigEndianBuf;
    DataBuffer littleEndianBuf( DataBuffer::LittleEndian );
    bigEndianBuf.WriteUCharArray( data, 16, false );
    littleEndianBuf.WriteUCharArray( data, 16, false );
    uint64_t bigExpected1 = 71777214294589695;
    uint64_t bigExpected2 = 18374966859414961920U;
    uint64_t littleExpected1 = 18374966859414961920U;
    uint64_t littleExpected2 = 71777214294589695;
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadInt64(), bigExpected1 );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadInt64(), bigExpected2 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadInt64(), littleExpected1 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadInt64(), littleExpected2 );
    DataBuffer bigEndianBuf2;
    ASSERT_TRUE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 0 );
    bigEndianBuf2.WriteUInt64( UINT64_C( 0x00ff00ff00ff00ff ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 8 );
    bigEndianBuf2.WriteUInt64( UINT64_C( 0xff00ff00ff00ff00 ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 16 );
    ASSERT_ARE_EQUAL( bigEndianBuf2.ReadUInt64(), UINT64_C( 0x00ff00ff00ff00ff ) );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 8 );
    ASSERT_ARE_EQUAL( bigEndianBuf2.ReadUInt64(), UINT64_C( 0xff00ff00ff00ff00 ) );
    ASSERT_TRUE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 0 );
    DataBuffer littleEndianBuf2( DataBuffer::LittleEndian );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
    littleEndianBuf2.WriteUInt64( UINT64_C( 0x00ff00ff00ff00ff ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 8 );
    littleEndianBuf2.WriteUInt64( UINT64_C( 0xff00ff00ff00ff00 ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 16 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadUInt64(), UINT64_C( 0x00ff00ff00ff00ff ) );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 8 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadUInt64(), UINT64_C( 0xff00ff00ff00ff00 ) );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
#endif
}
static void TestDataBufferReadWriteChar()
{
    unsigned char data[] = { 'a', 'b', 'c', 'd' };
    DataBuffer buf;
    buf.WriteUCharArray( data, 4, false );
    char expected1 = 'a';
    char expected2 = 'b';
    char expected3 = 'c';
    char expected4 = 'd';
    ASSERT_ARE_EQUAL( buf.ReadChar(), expected1 );
    ASSERT_ARE_EQUAL( buf.ReadChar(), expected2 );
    ASSERT_ARE_EQUAL( buf.ReadChar(), expected3 );
    ASSERT_ARE_EQUAL( buf.ReadChar(), expected4 );
    ASSERT_TRUE( buf.Empty() );
    buf.WriteChar( ' ' );
    buf.WriteChar( '\t' );
    buf.WriteChar( '\0' );
    buf.WriteChar( '\n' );
    ASSERT_ARE_EQUAL( buf.Size(), 4 );
    ASSERT_ARE_EQUAL( buf.ReadChar(), ' ' );
    ASSERT_ARE_EQUAL( buf.ReadChar(), '\t' );
    ASSERT_ARE_EQUAL( buf.Size(), 2 );
    ASSERT_ARE_EQUAL( buf.ReadChar(), '\0' );
    ASSERT_ARE_EQUAL( buf.ReadChar(), '\n' );
    ASSERT_TRUE( buf.Empty() );
}
static void TestDataBufferReadWriteUChar()
{
    unsigned char data[] = { 0x01, 0x02, 0x03, 0x04 };
    DataBuffer buf;
    buf.WriteUCharArray( data, 4, false );
    unsigned char expected1 = 0x01;
    unsigned char expected2 = 0x02;
    unsigned char expected3 = 0x03;
    unsigned char expected4 = 0x04;
    ASSERT_ARE_EQUAL( buf.ReadUChar(), expected1 );
    ASSERT_ARE_EQUAL( buf.ReadUChar(), expected2 );
    ASSERT_ARE_EQUAL( buf.ReadUChar(), expected3 );
    ASSERT_ARE_EQUAL( buf.ReadUChar(), expected4 );
    ASSERT_TRUE( buf.Empty() );
    buf.WriteUChar( 0x00 );
    buf.WriteUChar( 0xf0 );
    buf.WriteUChar( 0x0f );
    buf.WriteUChar( 0xff );
    ASSERT_ARE_EQUAL( buf.Size(), 4 );
    ASSERT_ARE_EQUAL( buf.ReadUChar(), 0x00 );
    ASSERT_ARE_EQUAL( buf.ReadUChar(), 0xf0 );
    ASSERT_ARE_EQUAL( buf.Size(), 2 );
    ASSERT_ARE_EQUAL( buf.ReadUChar(), 0x0f );
    ASSERT_ARE_EQUAL( buf.ReadUChar(), 0xff );
    ASSERT_TRUE( buf.Empty() );
}
static void TestDataBufferReadWriteShort()
{
    unsigned char data[] = { 0x00, 0x01, 0x01, 0x00 };
    DataBuffer bigEndianBuf;
    DataBuffer littleEndianBuf( DataBuffer::LittleEndian );
    bigEndianBuf.WriteUCharArray( data, 4, false );
    littleEndianBuf.WriteUCharArray( data, 4, false );
    short bigExpected1 = 1;
    short bigExpected2 = 256;
    short littleExpected1 = 256;
    short littleExpected2 = 1;
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadShort(), bigExpected1 );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadShort(), bigExpected2 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadInt16(), littleExpected1 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadInt16(), littleExpected2 );
    DataBuffer buf;
    ASSERT_TRUE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 0 );
    buf.WriteShort( 0x00ff );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 2 );
    buf.WriteShort( 0x7f00 );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 4 );
    ASSERT_ARE_EQUAL( buf.ReadShort(), 0x00ff );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 2 );
    ASSERT_ARE_EQUAL( buf.ReadShort(), 0x7f00 );
    ASSERT_TRUE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 0 );
    DataBuffer littleEndianBuf2( DataBuffer::LittleEndian );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
    littleEndianBuf2.WriteShort( 0x00ff );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 2 );
    littleEndianBuf2.WriteShort( 0x7f00 );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 4 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadShort(), 0x00ff );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 2 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadShort(), 0x7f00 );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
}
static void TestDataBufferReadWriteUShort()
{
    unsigned char data[] = { 0x00, 0xff, 0xff, 0x00 };
    DataBuffer bigEndianBuf;
    DataBuffer littleEndianBuf( DataBuffer::LittleEndian );
    bigEndianBuf.WriteUCharArray( data, 4, false );
    littleEndianBuf.WriteUCharArray( data, 4, false );
    unsigned short bigExpected1 = 255;
    unsigned short bigExpected2 = 65280;
    unsigned short littleExpected1 = 65280;
    unsigned short littleExpected2 = 255;
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadUShort(), bigExpected1 );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadUShort(), bigExpected2 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadUShort(), littleExpected1 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadUShort(), littleExpected2 );
    DataBuffer bigEndianBuf2;
    ASSERT_TRUE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 0 );
    bigEndianBuf2.WriteUShort( 0x00ff );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 2 );
    bigEndianBuf2.WriteUShort( 0xff00 );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 4 );
    ASSERT_ARE_EQUAL( bigEndianBuf2.ReadUShort(), 0x00ff );
    ASSERT_FALSE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 2 );
    ASSERT_ARE_EQUAL( bigEndianBuf2.ReadUShort(), 0xff00 );
    ASSERT_TRUE( bigEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf2.Size(), 0 );
    DataBuffer littleEndianBuf2( DataBuffer::LittleEndian );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
    littleEndianBuf2.WriteUShort( 0x00ff );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 2 );
    littleEndianBuf2.WriteUShort( 0xff00 );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 4 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadUShort(), 0x00ff );
    ASSERT_FALSE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 2 );
    ASSERT_ARE_EQUAL( littleEndianBuf2.ReadUShort(), 0xff00 );
    ASSERT_TRUE( littleEndianBuf2.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf2.Size(), 0 );
}
static void TestDataBufferReadWriteInt()
{
    // some tests with previous data types are not possible because the size of an int is uncertain
    DataBuffer bigEndianBuf;
    DataBuffer littleEndianBuf( DataBuffer::LittleEndian );
    int expected1 = 12345;
    int expected2 = -4321;
    ASSERT_TRUE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), 0 );
    bigEndianBuf.WriteInt( expected1 );
    ASSERT_FALSE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), sizeof( int ) );
    bigEndianBuf.WriteInt( expected2 );
    ASSERT_FALSE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), sizeof( int ) * 2 );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadInt(), expected1 );
    ASSERT_FALSE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), sizeof( int ) );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadInt(), expected2 );
    ASSERT_TRUE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), 0 );
    ASSERT_TRUE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), 0 );
    littleEndianBuf.WriteInt( expected1 );
    ASSERT_FALSE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), sizeof( int ) );
    littleEndianBuf.WriteInt( expected2 );
    ASSERT_FALSE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), sizeof( int ) * 2 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadInt(), expected1 );
    ASSERT_FALSE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), sizeof( int ) );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadInt(), expected2 );
    ASSERT_TRUE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), 0 );
}
static void TestDataBufferReadWriteUInt()
{
    // some tests with previous data types are not possible because the size of an unsigned int is uncertain
    DataBuffer bigEndianBuf;
    DataBuffer littleEndianBuf( DataBuffer::LittleEndian );
    unsigned int expected1 = 12345;
    unsigned int expected2 = 54321;
    ASSERT_TRUE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), 0 );
    bigEndianBuf.WriteUInt( expected1 );
    ASSERT_FALSE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), sizeof( unsigned int ) );
    bigEndianBuf.WriteUInt( expected2 );
    ASSERT_FALSE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), sizeof( unsigned int ) * 2 );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadUInt(), expected1 );
    ASSERT_FALSE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), sizeof( unsigned int ) );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadUInt(), expected2 );
    ASSERT_TRUE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), 0 );
    ASSERT_TRUE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), 0 );
    littleEndianBuf.WriteUInt( expected1 );
    ASSERT_FALSE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), sizeof( unsigned int ) );
    littleEndianBuf.WriteUInt( expected2 );
    ASSERT_FALSE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), sizeof( unsigned int ) * 2 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadUInt(), expected1 );
    ASSERT_FALSE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), sizeof( unsigned int ) );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadUInt(), expected2 );
    ASSERT_TRUE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), 0 );
}
static void TestDataBufferReadWriteLong()
{
    // some tests with previous data types are not possible because the size of a long is uncertain
    DataBuffer bigEndianBuf;
    DataBuffer littleEndianBuf( DataBuffer::LittleEndian );
    long expected1 = 123456789;
    long expected2 = -987654321;
    ASSERT_TRUE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), 0 );
    bigEndianBuf.WriteLong( expected1 );
    ASSERT_FALSE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), sizeof( long ) );
    bigEndianBuf.WriteLong( expected2 );
    ASSERT_FALSE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), sizeof( long ) * 2 );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadLong(), expected1 );
    ASSERT_FALSE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), sizeof( long ) );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadLong(), expected2 );
    ASSERT_TRUE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), 0 );
    ASSERT_TRUE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), 0 );
    littleEndianBuf.WriteLong( expected1 );
    ASSERT_FALSE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), sizeof( long ) );
    littleEndianBuf.WriteLong( expected2 );
    ASSERT_FALSE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), sizeof( long ) * 2 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadLong(), expected1 );
    ASSERT_FALSE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), sizeof( long ) );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadLong(), expected2 );
    ASSERT_TRUE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), 0 );
}
static void TestDataBufferReadWriteULong()
{
    // some tests with previous data types are not possible because the size of an unsigned long is uncertain
    DataBuffer bigEndianBuf;
    DataBuffer littleEndianBuf( DataBuffer::LittleEndian );
    unsigned long expected1 = 123456789;
    unsigned long expected2 = 4076543210;
    ASSERT_TRUE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), 0 );
    bigEndianBuf.WriteULong( expected1 );
    ASSERT_FALSE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), sizeof( unsigned long ) );
    bigEndianBuf.WriteULong( expected2 );
    ASSERT_FALSE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), sizeof( unsigned long ) * 2 );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadULong(), expected1 );
    ASSERT_FALSE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), sizeof( unsigned long ) );
    ASSERT_ARE_EQUAL( bigEndianBuf.ReadULong(), expected2 );
    ASSERT_TRUE( bigEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( bigEndianBuf.Size(), 0 );
    ASSERT_TRUE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), 0 );
    littleEndianBuf.WriteULong( expected1 );
    ASSERT_FALSE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), sizeof( unsigned long ) );
    littleEndianBuf.WriteULong( expected2 );
    ASSERT_FALSE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), sizeof( unsigned long ) * 2 );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadULong(), expected1 );
    ASSERT_FALSE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), sizeof( unsigned long ) );
    ASSERT_ARE_EQUAL( littleEndianBuf.ReadULong(), expected2 );
    ASSERT_TRUE( littleEndianBuf.Empty() );
    ASSERT_ARE_EQUAL( littleEndianBuf.Size(), 0 );
}
static void TestDataBufferReadWriteCharArray()
{
    DataBuffer buf;
    ASSERT_TRUE( buf.Empty() );
    buf.WriteCharArray( "abcdefg", false );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), strlen( "abcdefg" ) );
    char* arr = buf.ReadCharArray();
    ASSERT_TRUE( strcmp( arr, "abcdefg" ) == 0 );
    free( arr );

    ASSERT_TRUE( buf.Empty() );
    buf.WriteCharArray( "1 2 3 4 5 6 asdf jkl;urur ^^%($*#(@*@)", true );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), strlen( "1 2 3 4 5 6 asdf jkl;urur ^^%($*#(@*@)" ) + 1 );
    arr = buf.ReadCharArray();
    ASSERT_TRUE( strcmp( arr, "1 2 3 4 5 6 asdf jkl;urur ^^%($*#(@*@)" ) == 0 );
    free( arr );

    ASSERT_TRUE( buf.Empty() );
    buf.WriteCharArray( "string 1", true );
    buf.WriteCharArray( "string 2", true );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), strlen( "string 1" ) + strlen( "string 2" ) + 2 );
    arr = buf.ReadCharArray();
    ASSERT_TRUE( strcmp( arr, "string 1" ) == 0 );
    free( arr );
    arr = buf.ReadCharArray();
    ASSERT_TRUE( strcmp( arr, "string 2" ) == 0 );
    free( arr );
    ASSERT_TRUE( buf.Empty() );
}
static void TestDataBufferReadWriteCharArrayWithLength()
{
    DataBuffer buf;
    ASSERT_TRUE( buf.Empty() );
    buf.WriteCharArray( "1234567890", 5, false );
    buf.WriteCharArray( "abcdefghij", 5, false );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 10 );
    char* arr = buf.ReadCharArray( 7 );
    ASSERT_TRUE( strcmp( arr, "12345ab" ) == 0 );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 3 );
    free( arr );
    arr = buf.ReadCharArray();
    ASSERT_TRUE( strcmp( arr, "cde" ) == 0 );
    free( arr );
    ASSERT_TRUE( buf.Empty() );
}
static void TestDataBufferReadWriteUCharArray()
{
    DataBuffer buf;
    ASSERT_TRUE( buf.Empty() );
    unsigned char data[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
    buf.WriteUCharArray( data, false );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 7 );
    unsigned char* arr = buf.ReadUCharArray();
    ASSERT_TRUE( memcmp( arr, data, 7 ) == 0 );
    free( arr );

    ASSERT_TRUE( buf.Empty() );
    buf.WriteUCharArray( data, true );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 8 );
    arr = buf.ReadUCharArray();
    ASSERT_TRUE( memcmp( arr, data, 8 ) == 0 );
    free( arr );

    ASSERT_TRUE( buf.Empty() );
    buf.WriteUCharArray( data, true );
    buf.WriteUCharArray( data, true );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 16 );
    arr = buf.ReadUCharArray();
    ASSERT_TRUE( memcmp( arr, data, 8 ) == 0 );
    free( arr );
    arr = buf.ReadUCharArray();
    ASSERT_TRUE( memcmp( arr, data, 8 ) == 0 );
    free( arr );
    ASSERT_TRUE( buf.Empty() );
}
static void TestDataBufferReadWriteUCharArrayWithLength()
{
    DataBuffer buf;
    ASSERT_TRUE( buf.Empty() );
    unsigned char data1[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a };
    unsigned char data2[] = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a };
    buf.WriteUCharArray( data1, 5, false );
    buf.WriteUCharArray( data2, 5, false );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 10 );
    unsigned char* arr = buf.ReadUCharArray( 7 );
    unsigned char expected1[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x11, 0x12 };
    ASSERT_TRUE( memcmp( arr, expected1, 7 ) == 0 );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 3 );
    free( arr );
    arr = buf.ReadUCharArray();
    unsigned char expected2[] = { 0x13, 0x14, 0x15 };
    ASSERT_TRUE( memcmp( arr, expected2, 3 ) == 0 );
    free( arr );
    ASSERT_TRUE( buf.Empty() );
}
static void TestDataBufferReadWriteString()
{
    DataBuffer buf;
    ASSERT_TRUE( buf.Empty() );
    buf.WriteString( "1234567890", false );
    buf.WriteString( "abcdefghij", 5, false );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 15 );
    std::string arr = buf.ReadString( 7 );
    ASSERT_TRUE( strcmp( arr.c_str(), "1234567" ) == 0 );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 8 );
    arr = buf.ReadNullTerminatedString();
    ASSERT_TRUE( strcmp( arr.c_str(), "890abcde" ) == 0 );
    ASSERT_TRUE( buf.Empty() );
}
static void TestDataBufferReadWriteNullTerminatedString()
{
    DataBuffer buf;
    ASSERT_TRUE( buf.Empty() );
    buf.WriteString( "1234567890", true );
    buf.WriteString( "abcdefghij", 5, true );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 17 );
    std::string arr = buf.ReadNullTerminatedString();
    ASSERT_TRUE( strcmp( arr.c_str(), "1234567890" ) == 0 );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 6 );
    arr = buf.ReadNullTerminatedString();
    ASSERT_TRUE( strcmp( arr.c_str(), "abcde" ) == 0 );
    ASSERT_TRUE( buf.Empty() );
}


int main( int argc, char** argv )
{
    bool dataBufferAllPassed;
    DECLARE_UNIT_TEST_SUITE( DataBuffers )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferConstructFromUnsignedCharArray )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferConstructFromBuffer )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferCopyConstructor )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferAssignmentOperator )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferClear )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferSize )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferBracketOperator )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteInt8 )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteUInt8 )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteInt16 )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteUInt16 )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteInt32 )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteUInt32 )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteInt64 )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteUInt64 )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteChar )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteUChar )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteShort )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteUShort )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteInt )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteUInt )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteLong )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteULong )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteCharArray )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteCharArrayWithLength )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteUCharArray )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteUCharArrayWithLength )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteString )
        ADD_UNIT_TEST( DataBuffers, TestDataBufferReadWriteNullTerminatedString )
    END_UNIT_TEST_SUITE( DataBuffers, dataBufferAllPassed )

    return ( dataBufferAllPassed ? 0 : 1 );
}

