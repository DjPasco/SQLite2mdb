#ifndef __CMD_PARSER_H__
#define __CMD_PARSER_H__
#pragma once

// Dependency on Tools
#ifndef __COMMAND_LINE_PARSER_H__
    #include "command_line_parser.h"
#endif

namespace sqlite2mdb_cmd_parser
{
    inline std::vector<std::pair<std::wstring, std::wstring>> GetDefaultCommands()
    {
        std::vector<std::pair<std::wstring, std::wstring>> commands;
        commands.push_back(std::make_pair(L"s", L"source"));
        commands.push_back(std::make_pair(L"d", L"destination"));
        commands.push_back(std::make_pair(L"l", L"log"));
        commands.push_back(std::make_pair(L"h", L"help"));
        return commands;
    }

    inline std::wstring GetSourcePath(const CCommandLineParser *pParser) 
    {
        return pParser->GetData(L"s");
    }

    inline std::wstring GetDestinationPath(const CCommandLineParser *pParser) 
    {
        return pParser->GetData(L"d");
    }

    inline std::wstring GetLogPath(const CCommandLineParser *pParser) 
    {
        return pParser->GetData(L"l");
    }

    inline bool IsHelp(const CCommandLineParser *pParser) 
    {
        return pParser->CheckCommand(L"h");
    }
}

#endif