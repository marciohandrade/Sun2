#pragma once

//==================================================================================================
/// ArchiveOfStream class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2005. 8. 24
	@note
		- ���� ��Ŷ�� �а� ���� ���ϰ� ó���ϴ� Helper class
	@todo
		- 
	@history
		- 09.09.21, waverix, arrangement

*/
//==================================================================================================

// (WAVERIX) (NOTE) ArchiveOfStream�迭�� ��Ŷ���� ���� ����ϹǷ� pack1�� �����Ǿ� �ִ�.
// ���� ������ ����ϰ� �� ��� ������ �߻��� �� �����Ƿ� ���Ľɿ��� �����Ѵ�.
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
    //// (WAVERIX) (090529) (NOTE) Utility�� �����ϴ� �Ϳ� ��ũ�� �۾�? ��쿡 ���� ���� �� ��
    //// �� �ְ� �ƴ� ���� �ְ�... ����, �̰�...
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
