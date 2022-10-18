#ifndef N_PATHSTRING_H
#define N_PATHSTRING_H
//------------------------------------------------------------------------------
/**
    @ 경로 설정 스트링 클래스 

    @author
    - RadonLabs GmbH 

    @since
    - 2005.6.14
    @remarks
    - 지완 추가 
*/
//------------------------------------------------------------------------------


#include "Nstring.h"

class nPathString : public nString
{
public:
    /// default constructor
    nPathString();
    /// constructor 1
    nPathString(const char*str);
    /// copy constructor
    nPathString(const nPathString& rhs);
    /// = operator
    nPathString& operator=(const nPathString& rhs);
    /// = operator with string
    nPathString& operator=(const char* rhs);

    /// get pointer to extension (without the dot)
    const char* GetExtension() const;
    /// check if extension matches (no dot in the extension!)
    bool CheckExtension(const char* ext) const;
    /// convert backslashes to slashes
    void ConvertBackslashes();
    /// remove extension
    void StripExtension();
    /// extract the part after the last directory separator
    nPathString ExtractFileName() const;
    /// extract the last directory of the path
    nPathString ExtractLastDirName() const;
    /// extract the part before the last directory separator
    nPathString ExtractDirName() const;
    /// strip slash at end of path, if exists
    void StripTrailingSlash();
    /// extract path until last slash
    nPathString ExtractToLastSlash() const;

private:
    /// get pointer to last directory separator
    char* GetLastSlash() const;
};

//------------------------------------------------------------------------------
/**
*/
inline
nPathString::nPathString() :
    nString()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
nPathString::nPathString(const char* str) :
    nString(str)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
nPathString::nPathString(const nPathString& rhs) :
    nString()
{
    this->Copy(rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline
nPathString&
nPathString::operator=(const nPathString& rhs)
{
    this->Delete();
    this->Copy(rhs);
    return *this;
}

//------------------------------------------------------------------------------
/**
    06-Jan-03   floh    renamed to something more intuitive
*/
inline
void
nPathString::ConvertBackslashes()
{
    const char* ptr;
    if (this->string)
    {
        ptr = this->string;
    }
    else if (this->localString[0])
    {
        ptr = this->localString;
    }
    else
    {
        return;
    }

    char *buf = new char[this->strLen + 1];

    for (int i = 0; i <= this->strLen; i++)
    {
        buf[i] = (ptr[i] == '\\') ? '/' : ptr[i];
    }
    this->Set(buf);
    delete[] buf;
}

//------------------------------------------------------------------------------
/**
*/
inline
nPathString&
nPathString::operator=(const char* rhs)
{
    this->Set(rhs);
    return *this;
}

//------------------------------------------------------------------------------
/**
    @return     pointer to extension (without the dot), or 0
*/
inline
const char*
nPathString::GetExtension() const
{
    const char* str = this->Get();
    const char* ext = strrchr(str, '.');
    if (ext)
    {
        ext++;
        if (ext[0] != 0)
        {
            return ext;
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    Returns true if file extension matches.

    @param  ext     extension string (without the dot)
    @return         true if extension matches
*/
inline
bool
nPathString::CheckExtension(const char* ext) const
{
    const char* extStr = this->GetExtension();
    if (0 == extStr)
    {
        return false;
    }
    return (0 == (strcmp(ext, extStr)));
}

//------------------------------------------------------------------------------
/**
    Strips last slash, if the path name ends on a slash.
*/
inline
void
nPathString::StripTrailingSlash()
{
    if (this->strLen > 0)
    {
        int pos = strLen - 1;
        char* str = (char*) this->Get();
        if ((str[pos] == '/') || (str[pos] == '\\'))
        {
            str[pos] = 0;
        }
    }
}

//------------------------------------------------------------------------------
/**
    Remove the file extension.
*/
inline
void
nPathString::StripExtension()
{
    char* ext = (char*) this->GetExtension();
    if (ext)
    {
        ext[-1] = 0;
    }
}

//------------------------------------------------------------------------------
/**
    Get a pointer to the last directory separator.
*/
inline
char*
nPathString::GetLastSlash() const
{
    char* s = (char*) this->Get();
    char* lastSlash = strrchr(s, '/');
    if (0 == lastSlash) lastSlash = strrchr(s, '\\');
    if (0 == lastSlash) lastSlash = strrchr(s, ':');
    return lastSlash;
}

//------------------------------------------------------------------------------
/**
    Return a nPathString object containing the part after the last
    path separator.
*/
inline
nPathString
nPathString::ExtractFileName() const
{
    nPathString pathString;
    char* lastSlash = this->GetLastSlash();
    if (lastSlash)
    {
        pathString = &(lastSlash[1]);
    }
    else
    {
        pathString = this->Get();
    }
    return pathString;
}

//------------------------------------------------------------------------------
/**
    Return a nPathString object containing the last directory of the path, i.e.
    a category.

    - 17-Feb-04     floh    fixed a bug when the path ended with a slash
*/
inline
nPathString
nPathString::ExtractLastDirName() const
{
    nPathString pathString(*this);
    char* lastSlash = pathString.GetLastSlash();

    // special case if path ends with a slash
    if (lastSlash)
    {
        if (0 == lastSlash[1])
        {
            *lastSlash = 0;
            lastSlash = pathString.GetLastSlash();
        }

        char* secLastSlash = 0;
        if (0 != lastSlash)
        {
            *lastSlash = 0; // cut filename
            secLastSlash = pathString.GetLastSlash();
            if (secLastSlash)
            {
                *secLastSlash = 0;
                return nPathString(secLastSlash+1);
            }
        }
    }
    return "";
}

//------------------------------------------------------------------------------
/**
    Return a nPathString object containing the part before the last
    directory separator.

    NOTE: I left my fix in that returns the last slash (or colon), this was
    necessary to tell if a dirname is a normal directory or an assign.

    - 17-Feb-04     floh    fixed a bug when the path ended with a slash
*/
inline
nPathString
nPathString::ExtractDirName() const
{
    nPathString pathString(*this);
    char* lastSlash = pathString.GetLastSlash();

    // special case if path ends with a slash
    if (lastSlash)
    {
        if (0 == lastSlash[1])
        {
            *lastSlash = 0;
            lastSlash = pathString.GetLastSlash();
        }
        if (lastSlash)
        {
            *++lastSlash = 0;
        }
    }
    return pathString;
}

//------------------------------------------------------------------------------
/**
    Return a path string object which contains of the complete path
    up to the last slash. Returns an empty string if there is no
    slash in the path.
*/
inline
nPathString
nPathString::ExtractToLastSlash() const
{
    nPathString pathString(*this);
    char* lastSlash = pathString.GetLastSlash();
    if (lastSlash)
    {
        lastSlash[1] = 0;
    }
    else
    {
        pathString = "";
    }
    return pathString;
}

//------------------------------------------------------------------------------
/**
*/
static
inline
nPathString
operator+(const nPathString& s0, const nPathString& s1)
{
    nPathString newString = s0;
    newString.Append(s1.Get());
    return newString;
}

//------------------------------------------------------------------------------
#endif

