#pragma once



class Command
{
protected:
    Command(){}

public:
    virtual ~Command(){}
    void Execute() { OnExecute(); }

private:
    virtual void OnExecute() PURE;
};

class SendPacketCommand
    : public Command
{
private:
    eCONNECTION_INDEX connection_;
    BYTE* buffer_;
    int buffer_size_;

public:
    SendPacketCommand( eCONNECTION_INDEX connection, void* buffer, int buffer_size );

private:
    ~SendPacketCommand();

    void OnExecute();   // override
};


class FunctionCommand
    : public Command
{
private:
    void (*execute_func_)();

    void OnExecute()    // override
    { 
        execute_func_(); 
    }

public:
    FunctionCommand( void (*func)() )
        : execute_func_(func)
    {
    }
};
