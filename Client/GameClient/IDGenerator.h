#pragma once

class IDGenerator
{
public:
    IDGenerator( void );
    ~IDGenerator( void );

    DWORD GenerateID( void );
    void DestroyID( const DWORD dwID );

private:
    void Initialize( void );

private:
    typedef std::deque<DWORD> IdSet;
    IdSet m_idSet;
};
