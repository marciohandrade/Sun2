#include "SunClientPrecompiledHeader.h"
#include "Command.h"
#include "GlobalFunc.h"


SendPacketCommand::SendPacketCommand( eCONNECTION_INDEX connection, void* buffer, int buffer_size )
: connection_(CI_MAX)
, buffer_(NULL)
, buffer_size_(0)
{
    if( buffer_size > 0 && buffer != NULL )
    {
        connection_ = connection;
        buffer_ = new BYTE[buffer_size];
        memcpy( buffer_, buffer, buffer_size );
        buffer_size_ = buffer_size;
    }
}

SendPacketCommand::~SendPacketCommand()
{
    if( buffer_ != NULL )
    {
        delete buffer_;
        buffer_ = NULL;
        buffer_size_ = 0;
    }
}

void SendPacketCommand::OnExecute()
{
    if( buffer_ == NULL || buffer_size_ == 0 || connection_ == CI_MAX )
    {
        return;
    }

    GlobalFunc::SendPacket( connection_, buffer_, buffer_size_ );

}


