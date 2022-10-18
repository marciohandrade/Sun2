#ifndef N_BITSTREAM_H
#define N_BITSTREAM_H
//------------------------------------------------------------------------------
/**
    @author
    - RadonLabs GmbH 

    @since
    - 2005.6.30
    @remarks
    - 지완 추가 
*/

#include <memory.h>
#include "../ProgramCommon/Define.h"
#include "nnode.h"

//------------------------------------------------------------------------------
class nBitStream : public nNode
{
public:
    /// standard constructor, requires separate SetSize()
    nBitStream();
    /// constructor, expects stream size in bytes
    nBitStream(int size);
    /// the destructor
    virtual ~nBitStream();

    /// directly set contents of stream
    void Set(const uchar* s, int size);
    /// directly get contents of stream
    const uchar* Get() const;
    /// Set size of stream.
    void SetSize(int size);
    /// get the byte size of the stream
    int GetSize() const;
    /// set the current bit position in the stream
    void SetPos(int pos);
    /// return the current bit position in the stream
    int GetPos() const;
    /// return the number of bits left in the stream
    int BitsLeft() const;

    /// write a bool to the stream
    void WriteBool(bool flag);
    /// read a bool from the stream
    bool ReadBool();
    /// write a compressed integer to the stream
    void WriteInt(int value, int numBits);
    /// read a compressed integer from the stream
    int ReadInt(int numBits);
    /// write a float to the stream
    void WriteFloat(float value);
    /// read a float from the stream
    float ReadFloat();
    /// write a custom byte array to the stream
    void WriteCustom(const void* ptr, int size);
    /// read a custom byte array from the stream
    void ReadCustom(void* ptr, int size);
    /// write bitstream
    void WriteBitStream(nBitStream& stream);
    /// read bitstream
    void ReadBitStream(nBitStream& stream);
    /// copy data from source stream into this stream
    void Copy(const nBitStream& source);

    /// Lock for writing.
    void BeginWrite();
    /// Unlock for writing.
    void EndWrite();
    /// Lock for reading.
    void BeginRead();
    /// Unlock for reading.
    void EndRead();
    /// Writeable?
    bool IsWriteable() const;
    /// Readable?
    bool IsReadable() const;

    /// Assignment operator.
    nBitStream& operator = (const nBitStream& s);
    /// Equality operator.
    bool operator == (const nBitStream& s) const;
    /// Inequality operator.
    bool operator != (const nBitStream& s) const;

    // output
    void Print(int lines);

protected:
    /// clear stream contents, do not rewind bit pointer
    void Clear();
    /// Write bit at current position to stream.
    void WriteBit(bool bit);
    /// Read bit at currernt position from stream.
    bool ReadBit();

private:
    unsigned char* stream;
    int streamSize;
    int currentByte;    /// Current index in the stream buffer.
    int currentBit;     /// Current bit offset int currentByte.
    bool writeable;
    bool readable;
};

//------------------------------------------------------------------------------
/**
*/
inline
nBitStream::nBitStream()
{
    streamSize  = 0;
    currentByte = 0;
    currentBit  = 0;
    writeable   = false;
    readable    = false;
    stream      = 0;
}

//------------------------------------------------------------------------------
/**
    @param  size    the byte size of the bit stream (rounded up)
*/
inline
nBitStream::nBitStream(int size)
{
    // require
    ASSERT(size > 0);

    streamSize  = size;
    currentByte = 0;
    currentBit  = 0;
    writeable   = false;
    readable    = false;

    stream = new unsigned char[size];
    if (stream != 0)
    {
        memset(stream, 0, streamSize);
    }

    // ensure
    ASSERT(stream != 0);
}

//------------------------------------------------------------------------------
/**
*/
inline
nBitStream::~nBitStream()
{
    if (stream != 0)
    {
        delete[] stream;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nBitStream::Clear()
{
    // require
    ASSERT( stream != 0 );
    ASSERT( streamSize > 0 );

    memset(stream, 0, streamSize);
}

//------------------------------------------------------------------------------
/**
    Write a bool value (always encoded into 1 bit) to the current
    stream position.

    @param  flag    the bool value
*/
inline
void
nBitStream::WriteBool(bool flag)
{
    ASSERT(writeable);

    WriteBit(flag);
}

//------------------------------------------------------------------------------
/**
    Reads a bool value (always encoded into 1 bit) from the current
    stream position.

    @return     the bool value
*/
inline
bool
nBitStream::ReadBool()
{
    ASSERT(readable);

    return ReadBit();
}

//------------------------------------------------------------------------------
/**
    Write a signed integer clamped to numBits bits.

    @param  value       the integer value to write
    @param  numBits     number of bits to write
*/
inline
void
nBitStream::WriteInt(int value, int numBits)
{
    ASSERT(writeable);
    ASSERT((1 < numBits) && (numBits <= 32));
    ASSERT(BitsLeft() >= numBits);

    // Write sign
    int tmp = (1 << 31);
    int tmp2 = (value & tmp);
    WriteBit(tmp2 != 0);
    numBits -= 2;       // I use numBits for shifting
                        // 1 Bit less for sign
                        // 1 Bit less for shifting

    // Make absolute value;
    // write sign and value separate
    value = n_abs(value);

    // Write value
    while (numBits >= 0)
    {
        tmp = (1 << numBits);
        tmp2 = (value & tmp);
        WriteBit(tmp2 != 0);
        numBits--;
    }

    ASSERT(numBits == -1);
}

//------------------------------------------------------------------------------
/**
    Read a signed integer clamped to numBits bits, the rest of the
    32 bytes will be filled with the correct sign bit.

    @param  numBits     number of bits to read
    @return             the integer, correclty expanded to 32 bits
*/
inline
int
nBitStream::ReadInt(int numBits)
{
    ASSERT(readable);
    ASSERT((1 < numBits) && (numBits <= 32));
    ASSERT(BitsLeft() >= numBits);

    int value = 0;

    // Read sign
    bool sign = ReadBit();
    numBits -= 2;       // I use numBits for shifting
                        // 1 Bit less for sign
                        // 1 Bit less for shifting

    // Read data
    while (numBits >= 0)
    {
        int v = ReadBit() ? 1 : 0;
        value |= (v << numBits);
        numBits--;
    }

    ASSERT(numBits == -1);

    // set sign
    if (sign)
    {
        value = -value;
    }

    return value;
}

//------------------------------------------------------------------------------
/**
    Write a float value into the stream, the float value will always be
    encoded into 32 bits.

    @param  value   the float value to write
*/
inline
void
nBitStream::WriteFloat(float value)
{
    ASSERT(writeable);
    ASSERT(BitsLeft() >= 32);

    int tmp;
    memcpy(&tmp, &value, 4);
    WriteInt(tmp, 32);      // Write floats as ints; use 32 BITS
}

//------------------------------------------------------------------------------
/**
    Read a float value (encoded in 32 bit) from the stream.

    @return      the float value
*/
inline
float
nBitStream::ReadFloat()
{
    ASSERT(readable);
    ASSERT(BitsLeft() >= 32);

    int tmp = ReadInt(32);      // Read floats as int; use 32 BITS
    float value;
    memcpy(&value, &tmp, 4);

    return value;
}

//------------------------------------------------------------------------------
/**
    Write a custom byte array to the stream.

    @param  ptr     pointer to byte array to write to the stream
    @param  size    number of bytes to write
*/
inline
void
nBitStream::WriteCustom(const void* ptr, int size)
{
    ASSERT(writeable);
    ASSERT(ptr != 0);
    ASSERT(size > 0);
    ASSERT(BitsLeft() >= size * 8);

    // advance to next byte in stream
    // if current bit offset is greater 0
    if (currentBit > 0)
    {
        currentByte++;
        currentBit = 0;
    }
    memcpy(&stream[currentByte], ptr, size);
    currentByte += size;
}

//------------------------------------------------------------------------------
/**
    Read a custom byte array from the stream.

    @param  ptr     pointer to byte array
    @param  size    size in bytes to read
*/
inline
void
nBitStream::ReadCustom(void* ptr, int size)
{
    ASSERT(readable);
    ASSERT(ptr != 0);
    ASSERT(size > 0);
    ASSERT(BitsLeft() >= size * 8);

    // advance to next byte in stream
    // if current bit offset is greater 0
    if (currentBit > 0)
    {
        currentByte++;
        currentBit = 0;
    }
    memcpy(ptr, &stream[currentByte], size);
    currentByte += size;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nBitStream::ReadBitStream(nBitStream& stream)
{
    ASSERT(stream.streamSize <= (BitsLeft() * 8));

    ReadCustom(stream.stream, stream.streamSize);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nBitStream::WriteBitStream(nBitStream& stream)
{
    ASSERT(stream.streamSize <= (BitsLeft() * 8));

    WriteCustom(stream.stream, stream.streamSize);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nBitStream::Set(const uchar* s, int size)
{
    // require
    ASSERT(s != 0);
    ASSERT(size > 0);

    // delete old stream, if exits
    if (stream != 0) delete[] stream;

    // create and copy stream
    stream = new unsigned char[size];
    ASSERT(stream != 0);
    streamSize = size;
    memcpy(stream, s, size);
}

//------------------------------------------------------------------------------
/**
*/
inline
const uchar*
nBitStream::Get() const
{
    return stream;
}


//------------------------------------------------------------------------------
/**
*/
inline
void
nBitStream::SetSize(int size)
{
    ASSERT(size > 0);

    if (stream != 0)
    {
        delete stream;
    }
    stream = new unsigned char[size];
    ASSERT(stream != 0);
    streamSize = size;
    Clear();

    ASSERT(streamSize == size);
}

//------------------------------------------------------------------------------
/**
*/
inline
int
nBitStream::GetSize() const
{
    return streamSize;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nBitStream::SetPos(int pos)
{
    ASSERT(!writeable);       // Can't change bit pos while writing!!!
    ASSERT((0 <= pos) && (pos <= streamSize * 8));

    currentByte = pos / 8;
    currentBit  = pos % 8;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
nBitStream::GetPos() const
{
    return (currentByte * 8) + currentBit;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
nBitStream::BitsLeft() const
{
    return (GetSize() * 8) - GetPos();
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nBitStream::Copy(const nBitStream& source)
{
    ASSERT(source.streamSize > 0);
    ASSERT(source.streamSize == streamSize);
    ASSERT(stream != 0);
    ASSERT(source.stream != 0);

    memcpy(stream, source.stream, source.streamSize);
    currentBit = streamSize * 8;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nBitStream::BeginWrite()
{
    ASSERT(!writeable);
    ASSERT(!readable);
    Clear();
    SetPos(0);
    writeable = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nBitStream::EndWrite()
{
    ASSERT(writeable);
    ASSERT(!readable);
    writeable = false;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nBitStream::BeginRead()
{
    ASSERT(!readable);
    ASSERT(!writeable);
    SetPos(0);
    readable = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nBitStream::EndRead()
{
    ASSERT(readable);
    ASSERT(!writeable);
    readable = false;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nBitStream::IsWriteable() const
{
    return writeable;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nBitStream::IsReadable() const
{
    return readable;
}

//------------------------------------------------------------------------------
/**
    Print debug data to stdout.
*/
inline
void
nBitStream::Print(int lines)
{
    // require
    ASSERT(stream != 0);

    // print out the first bits
    for (int i = 0; i < lines; i++)
    {
        n_printf("%4.d: ", i + 1);
        char ch = stream[i];
        for (int j = 0; j < 8; j++)
        {
            if ( ch & ( 1 << ( 7 - j ) ) )
            {
                n_printf( "%d", 1 );
            }
            else
            {
                n_printf( "%d", 0 );
            }
        }
        n_printf( "\n" );
    }
}

//------------------------------------------------------------------------------
/**
    Assignment operator.
*/
inline
nBitStream&
nBitStream::operator = (const nBitStream& s)
{
    ASSERT(s.streamSize > 0);
    ASSERT(s.streamSize == streamSize);
    ASSERT(stream != 0);
    ASSERT(s.stream != 0);

    Copy(s);

    return *this;
}

//------------------------------------------------------------------------------
/**
    Equality operator.
*/
inline
bool
nBitStream::operator == (const nBitStream& s) const
{
    if (streamSize != s.streamSize) return false;

    return (memcmp(stream, s.stream, streamSize) == 0);
}

//------------------------------------------------------------------------------
/**
    Inequality operator.
*/
inline
bool
nBitStream::operator != (const nBitStream& s) const
{
    if (streamSize != s.streamSize) return true;

    return (memcmp(stream, s.stream, streamSize) != 0);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nBitStream::WriteBit(bool bit)
{
    ASSERT(stream != 0);
    ASSERT(BitsLeft() >= 1);
    ASSERT(IsWriteable());

    // Set bit at current bit in current byte;
    // assume that all upcomming bits are cleared
    if (bit)
    {
        int v = (bit) ? 1 : 0;
        stream[currentByte] |= (v << (7 - currentBit));
    }

    // Advance to next byte if current bit is 8!
    if (++currentBit == 8)
    {
        currentByte++;
        currentBit = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nBitStream::ReadBit()
{
    ASSERT(stream != 0);
    ASSERT(BitsLeft() >= 1);
    ASSERT(IsReadable());

    // Read current bit
    bool flag = ((stream[currentByte] & (1 << (7 - currentBit))) != 0);

    // Advance to next byte if current bit is 8!
    if (++currentBit == 8)
    {
        currentByte++;
        currentBit = 0;
    }

    return flag;
}

#endif
