#pragma once

//==================================================================================================
/// ArchiveOfStream class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2005. 8. 24
	@note
		- 가변 패킷을 읽고 쓰기 편하게 처리하는 Helper class
	@todo
		- 
	@history
		- 09.09.21, waverix, arrangement

*/
//==================================================================================================

// (WAVERIX) (NOTE) ArchiveOfStream계열은 패킷에서 많이 사용하므로 pack1로 설정되어 있다.
// 만약 별도로 사용하게 될 경우 문제가 발생할 수 있으므로 노파심에서 설정한다.
// (CHANGES) (f100520.3L) critical changes : changes code about a 'pragma pack'
#pragma pack(push)
#pragma pack()

class ArchiveOfStream
{
public: 
    ArchiveOfStream(BYTE* p, WORD size)
        : buffer_(p)
        , buffer_size_(size)
        , curr_pos_(0)
    {
    }

    ~ArchiveOfStream() {
    }

public:
    template<class T>
    const ArchiveOfStream& operator>>(T& type) const; // (WAVERIX) (090921) (ADD-IN)
    template<class T>
    ArchiveOfStream& operator>>(T& type);
    template<class T>
    ArchiveOfStream& operator<<(const T& type);
    __inline void Read(void* pBuffer, int size) const;
    __inline void Write(const void* pBuffer, int size); 
    __inline int GetSize() const;
    __inline int GetBufferSize() const;
    __inline void Seek(int offset) const;
private:
    BYTE* buffer_;
    int buffer_size_;
    mutable int curr_pos_; //for const object operation
};

//==================================================================================================

// (WAVERIX) (090921) (ADD-IN)
template<class T>
const ArchiveOfStream&
ArchiveOfStream::operator>>(T& type) const
{
    Read(&type, sizeof(T));
    return (*this);
}

template<class T>
ArchiveOfStream&
ArchiveOfStream::operator>>(T& type)
{
    Read(&type, sizeof(T));
    return (*this);
}

template<class T>
ArchiveOfStream&
ArchiveOfStream::operator<<(const T& type)
{
    Write(&type, sizeof(T));
    return (*this);
}

__inline void
ArchiveOfStream::Read(void* pBuffer, int size) const
{ 
    //ASSERT(curr_pos_ + size <= buffer_size_);
    ////__NA_20080801_SHOP_GAMESERVER_DOWN_BUGFIX
    //// (WAVERIX) (090529) (NOTE) Utility에 존재하는 것에 매크로 작업? 경우에 따라 적용 될 수
    //// 도 있고 아닐 수도 있고... 뭐냐, 이건...
    //if(curr_pos_ + size > buffer_size_)
    //    return;
    if (bool overrun = !(curr_pos_ + size <= buffer_size_)) {
        assert(!"tried to read indexing out of ranges");
        return;
    };

    CopyMemory(pBuffer, &buffer_[curr_pos_], size); 
    curr_pos_ += size;
}

__inline void
ArchiveOfStream::Write(const void* pBuffer, int size) 
{
    //ASSERT(curr_pos_ + size <= buffer_size_);
    //if(curr_pos_ + size > buffer_size_) return;
    if (bool overrun = !(curr_pos_ + size <= buffer_size_)) {
        assert(!"tried to write indexing out of ranges");
        return;
    };

    CopyMemory(&buffer_[curr_pos_], pBuffer, size); 
    curr_pos_ += size;
}

__inline int
ArchiveOfStream::GetSize() const {
    return curr_pos_;
}

__inline int
ArchiveOfStream::GetBufferSize() const {
    return buffer_size_;
}

__inline void
ArchiveOfStream::Seek(int offset) const {
    curr_pos_ += offset;
}


#pragma pack(pop)
