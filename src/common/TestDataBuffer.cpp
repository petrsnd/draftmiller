// (c) 2017 petrsnd@gmail.com.  All rights reserved.

#include "DataBuffer.h"

#include "UnitTestSuite.h"

using namespace std;
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
    DataBuffer buf;
    ASSERT_TRUE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 0 );
    buf.WriteInt16( INT16_C( 0x00ff ) );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 2 );
    buf.WriteInt16( INT16_C( 0x7f00 ) );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 4 );
    ASSERT_ARE_EQUAL( buf.ReadInt16(), INT16_C( 0x00ff ) );
    ASSERT_FALSE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 2 );
    ASSERT_ARE_EQUAL( buf.ReadInt16(), INT16_C( 0x7f00 ) );
    ASSERT_TRUE( buf.Empty() );
    ASSERT_ARE_EQUAL( buf.Size(), 0 );
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

#endif
}
static void TestDataBufferReadWriteInt32()
{
#if defined( _INT32_T )
    throw NotImplementedException();
#endif
}
static void TestDataBufferReadWriteUInt32()
{
#if defined( _UINT32_T )
    throw NotImplementedException();
#endif
}
static void TestDataBufferReadWriteInt64()
{
#if defined( _INT64_T )
    throw NotImplementedException();
#endif
}
static void TestDataBufferReadWriteUInt64()
{
#if defined( _UINT64_T )
    throw NotImplementedException();
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
    throw NotImplementedException();
}
static void TestDataBufferReadWriteShort()
{
    throw NotImplementedException();
}
static void TestDataBufferReadWriteUShort()
{
    throw NotImplementedException();
}
static void TestDataBufferReadWriteInt()
{
    throw NotImplementedException();
}
static void TestDataBufferReadWriteUInt()
{
    throw NotImplementedException();
}
static void TestDataBufferReadWriteLong()
{
    throw NotImplementedException();
}
static void TestDataBufferReadWriteULong()
{
    throw NotImplementedException();
}
static void TestDataBufferReadWriteCharArray()
{
    throw NotImplementedException();
}
static void TestDataBufferReadWriteCharArrayWithLength()
{
    throw NotImplementedException();
}
static void TestDataBufferReadWriteUCharArray()
{
    throw NotImplementedException();
}
static void TestDataBufferReadWriteUCharArrayWithLength()
{
    throw NotImplementedException();
}
static void TestDataBufferReadWriteString()
{
    throw NotImplementedException();
}
static void TestDataBufferReadWriteNullTerminatedString()
{
    throw NotImplementedException();
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

