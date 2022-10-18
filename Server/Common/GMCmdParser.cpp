#include "stdafx.h"
#include "./GMCmdParser.h"

GMCmdParser::GMCmdParser(void)
{
}

GMCmdParser::~GMCmdParser(void)
{
}

bool GMCmdParser::Parsing(const char* pszCmd, GM_CMD_STRING& vecCmd)
{
    if (pszCmd == NULL || (strcmp(pszCmd, "") == 0)) {
        return false;
    }
    STLX_TSTRING strCmd = pszCmd;
    STLX_TSTRING strHead[kMaxNumberOfArguments];
    STLX_TSTRING strTemp = strCmd;
    int nIdx = 0, nCntCmd = 0;

    //1. 이 문장을 파싱한다.
    for (int i = 0; i < kMaxNumberOfArguments; i++)
    {
        nIdx = (int)strCmd.find(' ');
        if (nIdx == STLX_TSTRING::npos)
        {
            //만일 마지막 문자열이면, 구분자가 없기 때문에 여기로 온다.
            if (strCmd.length() > 0)
            {
                strHead[i] = strCmd;
                vecCmd.push_back(strHead[i]);
                nCntCmd++;
            }

            strCmd.clear();

            break;
        }

        strHead[i] = strCmd.substr(0, nIdx);
        vecCmd.push_back(strHead[i]);
        strCmd.erase(0, strHead[i].length() + 1);  // +1를 해준것은 빈칸을 포함시켜서 지워야 하기 때문이다.
        nCntCmd++;
    }

    return true;
}

bool GMCmdParser::Tokenize(const char* gm_cmd_string_source, GmCmdTokens* tokenized_info)
{
    tokenized_info->clear();
    size_t src_len = strlen(gm_cmd_string_source);
    if (src_len >= _countof(tokenized_info->string_block)) {
        return false;
    }
    strncpy(tokenized_info->string_block, gm_cmd_string_source,
            _countof(tokenized_info->string_block));
    tokenized_info->string_block[_countof(tokenized_info->string_block) - 1] = '\0';
    //_strlwr(tokenized_info->string_block);
    //
    const char* gm_cmd = strtok(tokenized_info->string_block, " ");
    if (gm_cmd == 0) {
        return false;
    };
    // NOTE: changes first token only to lower case except arguments
    // becasue it's possible that is like to the below sample.
    //                                        [O]        [X]    [X]
    // 'Channel_Notice Hello, WORLD' -> 'channel_notice hello, world'
    // 'Channel_Notice Hello, WORLD' -> 'channel_notice Hello, WORLD' - expected
    _strlwr(tokenized_info->string_block);
    // set invalid referece (not null)
    memset(tokenized_info->token_offsets,
           _countof(tokenized_info->string_block) - 1,
           sizeof(tokenized_info->token_offsets));
    //
    uint8_t* offset_it = tokenized_info->token_offsets;
    *offset_it++ = static_cast<uint8_t>(gm_cmd - tokenized_info->string_block);
    //
    bool is_failed = false;
    while (const char* argument = strtok(NULL, " "))
    {
        int offset = static_cast<int>(offset_it - tokenized_info->token_offsets);
        if (offset > _countof(tokenized_info->token_offsets)) {
            is_failed = true;
            break;
        }
        *offset_it++ = static_cast<uint8_t>(argument - tokenized_info->string_block);
    };
    if (is_failed) {
        tokenized_info->clear();
        return false;
    }
    // assert(success)
    tokenized_info->number_of_arguments = \
        static_cast<uint8_t>(offset_it - tokenized_info->token_offsets);

    return true;
}