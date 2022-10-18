#pragma once
#ifndef SERVERCOMMON_GMCMDPARSER_H
#define SERVERCOMMON_GMCMDPARSER_H

typedef STLX_VECTOR<STLX_TSTRING> GM_CMD_STRING;

//==================================================================================================

struct GmCmdTokens;

class GMCmdParser
{
public:
    static const int kMaxNumberOfArguments = 20;
    static const int kMaxGmStringLength = 100;
    GMCmdParser();
    ~GMCmdParser();

    bool Parsing(const char* pszCmd, GM_CMD_STRING& vecCmd);
    // NOTE: f101208.4L
    bool Tokenize(const char* source_gm_cmd_string, GmCmdTokens* tokenized_info);
};

//==================================================================================================

struct GmCmdTokens
{
    struct Token {
        Token(const Token& rhs) : token(rhs.token) {}
        Token(const char* input_token) : token(input_token) {}
        void operator=(const Token& rhs) { *this = rhs; }
        const char* c_str() const { return token; };
        uint8_t size() const { return token ? static_cast<uint8_t>(strlen(token)) : 0; }
        const char* const token;
    };
    void clear();
    uint8_t size() const;
    Token operator[](size_t index) const;
    //
    uint8_t number_of_arguments;
    uint8_t token_offsets[GMCmdParser::kMaxNumberOfArguments];
    char string_block[GMCmdParser::kMaxGmStringLength + 1];
};

//==================================================================================================

inline void GmCmdTokens::clear()
{
    ZeroMemory(this, sizeof(*this));
}

inline uint8_t GmCmdTokens::size() const
{
    return number_of_arguments;
}

inline GmCmdTokens::Token GmCmdTokens::operator[](size_t index) const
{
    assert(string_block[_countof(string_block) - 1] == '\0');
    if (index < number_of_arguments) {
        return Token(&string_block[token_offsets[index]]);
    }
    return Token(&string_block[_countof(string_block) -1]);
}

#endif //SERVERCOMMON_GMCMDPARSER_H