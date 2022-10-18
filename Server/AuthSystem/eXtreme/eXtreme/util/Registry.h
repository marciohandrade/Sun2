#ifndef  __eXtreme_Registry_H
#define  __eXtreme_Registry_H

namespace eXtreme {

class Registry  
{
public:
    Registry(HKEY key = HKEY_LOCAL_MACHINE);
    ~Registry();

    bool Open(char* subKey);
    bool Create(char* key);
    void Close();

    bool ReadDWORD(char* valueName, unsigned long* buffer);
    bool ReadString(char* valueName, char* buffer);

    bool WriteDWORD(char* valueName, unsigned long& buffer);
    bool WriteString(char* valueName, char* buffer);

private:
    HKEY  m_key, m_defaultKey;
    TCHAR m_subKey[1024];
};
}

#endif