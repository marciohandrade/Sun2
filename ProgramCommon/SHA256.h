#pragma once

namespace Crypt
{
    class SHA256
    {
    public:
        SHA256();
        ~SHA256();
        const char* CreateKey(const char* input1, const char* input2);
        bool Compare(const char* input1, const char* input2, const char* key_string);
        const char* GetLastError();

    private:
        char key_string_[65];
    };

}