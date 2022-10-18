#include "stdafx.h"

#include <zlib.h>

#include "./PacketControl_Compress.h"
#include "./CategoryLinkPair.h"
#include "./PacketControl_Types.hxx"
//==================================================================================================
//
namespace nsPacket {
;

const int CompressUtil::COMPRESS_LEVEL = Z_BEST_SPEED;
CompressUtil::eTRANS_STATE CompressUtil::ms_COMPRESS_TRANS = CompressUtil::DEFAULT_TRANS;

PacketInfo CompressUtil::MakeStream(const BOOLEAN isPack, BYTE* OUT pOutPtr,
                                    MSG_BASE* const pMsg, const WORD wSize)
{
#if !defined(_DEBUG)
    __UNUSED(isPack);
#endif
    PacketInfo destInfo = { (MSG_BASE*)pOutPtr, 0 };
    PacketInfo srcInfo = { (MSG_BASE*)pMsg, 0 };

    MSG_BASE* const pDestMsgBase = (MSG_BASE*)pOutPtr;
    BYTE* const pControlPtr = (BYTE*)(pMsg+1);
    const WORD controlSz = wSize-sizeof(MSG_BASE);

    DWORD compressedSz = BUFFER_SIZE;
    INT iErr = Z_OK;

    if(PACKET_CONTROL_PACKING)
    {
        if(CategoryConverter::IsCategoryPacked(pMsg->m_byCategory))
        {
            PacketInfo ret = { pMsg, wSize };
            return ret;
        }

        WRITEtoMSG_BASE(destInfo, *(MSG_BASE*)pMsg);
        if(NEED_COMPRESS_TOTAL_SZ < wSize)
        {
            CategoryConverter::SetPackInfo(pDestMsgBase);
            ms_COMPRESS_TRANS = PACKET_COMPRESS_TRANS;
            iErr = compress2((BYTE*)destInfo.pMsg, &compressedSz, pControlPtr, controlSz, COMPRESS_LEVEL);
            ms_COMPRESS_TRANS = DEFAULT_TRANS;
        }
        else
        {
            CopyMemory(destInfo.pMsg, pControlPtr, controlSz);
            compressedSz = controlSz;
        }
    }
    else
    {
        READ_MSG_BASEfromMSG_BASE(destInfo, srcInfo);
        if(CategoryConverter::IsCategoryPacked(pDestMsgBase->m_byCategory))
        {
            CategoryConverter::SetUnPackInfo(pDestMsgBase);
            ms_COMPRESS_TRANS = PACKET_DECOMPRESS_TRANS;
            iErr = uncompress((BYTE*)destInfo.pMsg, &compressedSz, pControlPtr, controlSz);
            ms_COMPRESS_TRANS = DEFAULT_TRANS;
        }
        else
        {
            CopyMemory(destInfo.pMsg, srcInfo.pMsg, controlSz);
            compressedSz = controlSz;
        }
    }

    PacketInfo ret = { (MSG_BASE*)pOutPtr, (WORD)(sizeof(MSG_BASE) + compressedSz) };
    return ret;
};

//==================================================================================================
//

voidpf __cdecl CompressUtil::memalloc_handler(uInt len, const int compress_trans)
{
    if(compress_trans)
    {
        const INT total_alloc_count = 5;
        static INT        alloced_count = 0;
        static BYTE        buffer_stream[4][65536];
        static BYTE        plate_stream[5816];

        if(alloced_count%total_alloc_count)
        {
            ASSERT(len == 65536);
            BYTE* const pAllocPtr = buffer_stream[alloced_count-1];
            //printf("alloc stream ptr %u\n", pAllocPtr);
            alloced_count = (alloced_count+1)%total_alloc_count;
            return pAllocPtr;
        }
        else
        {
            ASSERT(len == 5816);
            alloced_count = (alloced_count+1)%total_alloc_count;
            return plate_stream;
        }
    }
    else
    {
        switch(len)
        {
            //式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
            //    <FOR DECOMPRESS>
        case 24:
            {
                static BYTE stream[24];
                return stream;
            }
        case 64:
            {
                static BYTE stream[64];
                return stream;
            }
        case 11520:
            {
                static BYTE stream[11520];
                return stream;
            }
        case 32768:
            {
                static BYTE stream[32768];
                return stream;
            }
        //case 1156:    // table size
        //case 1236:
        //    {
        //        static BYTE stream[1236];
        //        return stream;
        //    }
        case 76:
            {
                static BYTE stream[76];
                return stream;
            }
        //case 1152:
        //    {
        //        static BYTE stream[1152];
        //        return stream;
        //    }
        case 28:
            {
                static BYTE stream[28];
                return stream;
            }
            //
            //式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
        default:
            {
                if(0 < len && len <= 1424)
                {
                    const INT total_alloc_count = 2;
                    static INT        alloced_count = 0;
                    static BYTE        stream[total_alloc_count][1424];

                    BYTE* const pAllocPtr = stream[alloced_count];
                    alloced_count = (alloced_count+1)%total_alloc_count;
                    return pAllocPtr;
                }
                //printf("Unexpected Request Size %u\n", len);
                ASSERT(!"Unexpected Request Size");
                return 0;// malloc(len);
            }
        }
    }
    assert(!"impossible reached point");
    //return 0;
}

void __cdecl CompressUtil::memfree_handler(voidpf ptr, const int compress_trans)
{
    __UNUSED((ptr, compress_trans));
}

};    // end namespace


