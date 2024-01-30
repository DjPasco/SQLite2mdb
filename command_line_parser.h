#ifndef __COMMAND_LINE_PARSER_H__
#define __COMMAND_LINE_PARSER_H__
#pragma once

#include <vector>
#include <string>
#include <queue>
#include <unordered_map>

class CCommandLineParser
{
// Construction/Destruction
public:
    CCommandLineParser(int argc, wchar_t *argv[], const std::vector<std::pair<std::wstring, std::wstring>> &command_list)
    {
        bool bData = false;
        std::wstring sCmd = L"";
        std::wstring sLastCommand = L"";

        for (int32_t i1 = 0; i1 < argc; ++i1) {
            sCmd = argv[i1];
            if (IsCommand(sCmd, command_list)) {
                m_parsed_commands[sCmd] = L"";
                sLastCommand = sCmd;
                bData = true;
            }
            else if (bData) {
                bData = false;
                if (!sLastCommand.empty()) {
                    m_parsed_commands[sLastCommand] = sCmd;
                    sLastCommand = L"";
                }
            }
        }

        // When there is no parsed commands but there is arguments, interpret it as single path
        if (argc > 1 && m_parsed_commands.empty()) {
            for (int32_t i2 = 1; i2 < argc; ++i2) {
                m_sSinglePath += argv[i2]; 
                if (i2 < argc - 1) {
                    m_sSinglePath += L" ";
                }
            }
            m_bSinglePath = true;
        }
    }

	~CCommandLineParser() { }

// Operations
public:
	bool CheckCommand(const wchar_t *sCommand) const {
		auto found = m_parsed_commands.find(sCommand);
		return found != m_parsed_commands.end();
	}

	std::wstring GetData(const wchar_t *sCommand) const {
        auto found = m_parsed_commands.find(sCommand);
		if (found != m_parsed_commands.end()) {
			return found->second;
        }
		return L"";
	}

    bool IsSinglePath() const {
        return m_bSinglePath;
    }

    std::wstring GetSinglePath() const {
        return m_sSinglePath;
    }

// Utils
private:
	static bool IsCommand(std::wstring &sCommand, const std::vector<std::pair<std::wstring, std::wstring>> &command_list)  {
		if (!CheckAndRemovePrefix(sCommand))  {
			return false;
        }

		for (auto &cmd : command_list) {   
			if (::wcscmp(cmd.first.c_str(), sCommand.c_str()) == 0) {
				return true;
            }

			if (::wcscmp(cmd.second.c_str(), sCommand.c_str()) == 0) {
				sCommand = cmd.first;
				return true;
			}
		}

		return false;
	}

	static bool CheckAndRemovePrefix(std::wstring &sCommand)  {
        if (sCommand.size() < 2) {
            return false;
        }
		else if (sCommand.at(0) == L'-' && sCommand.at(1) == L'-') {
			sCommand.erase(0, 2);
			return true;
		}
		else if (sCommand.at(0) == L'-' && sCommand.size() < 3) {
			sCommand.erase(0, 1);
			return true;
		}
		return false;
	}

// Attributes
private:
	std::unordered_map<std::wstring, std::wstring> m_parsed_commands;
    bool m_bSinglePath {false};
    std::wstring m_sSinglePath;
};

#endif