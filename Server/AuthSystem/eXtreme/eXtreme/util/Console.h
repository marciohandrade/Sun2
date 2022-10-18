#pragma once

#define  CONSOLE_NORMALSTRING_LEN		1024

namespace eXtreme {

class Console
{
public:
    Console(LPCTSTR title = 0);
    ~Console();

    void SetTabSize(BYTE size);
    void SetTitle(LPCTSTR title);
    void EnableInput(bool flag = true);

    void WriteLine(LPTSTR format, ...);
    void WriteLineNormal(LPTSTR format, ...);
    void ReadLine(LPTSTR buffer, DWORD bufferSize, DWORD* readLen);

    bool IsKeyboardHit();
    int  GetHitKey();

    char NormalString[CONSOLE_NORMALSTRING_LEN];

private:
    void ScrollScreenBuffer(int width);
    void OutputString(WORD attr, char* buffer);
    void SetForeAttr(WORD* attr, char c);
    void SetBackAttr(WORD* attr, char c);
    void NewLine();
    void Tab();

    HANDLE      m_handleInput;
    HANDLE      m_handleOutput;

    BYTE        m_tabSize;
    int         m_nsOffset;         //  NormalString offset
};

}