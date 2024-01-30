#include "stdafx.h"

// AfxDaoTerm();
#include "afxdao.h"

#include "DBUtils/dsDatabase.h"

#include "sqlite2mdb_cmd_parser.h"

// std::wcout
#include <iostream>

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif

static inline bool ExecuteCommandLineConv(int argc, wchar_t *argv[])
{  
	CCommandLineParser parser(argc, argv, sqlite2mdb_cmd_parser::GetDefaultCommands());

    if (sqlite2mdb_cmd_parser::IsHelp(&parser)) {
        std::wcout << L"SQLite database converter to MS DAO format database. HELP INFO:\n";
        std::wcout << L"-s (--source)     : source database (SQLite).\n";
        std::wcout << L"-d (--destination): destination database (MS DAO). If destinations does not exist - mdb file will be created from source database schema.\n";
        std::wcout << L"-l (--log)        : log path (txt file).\n";
        std::wcout << L"-h (--help)       : help information.\n";
        return true;
    }

    const std::wstring sSrcDBPath = sqlite2mdb_cmd_parser::GetSourcePath(&parser);
    if (sSrcDBPath.empty()) {
        std::wcout << L"No source database path defined. Use -s";
        return false;
    }

    std::wstring sTargeDBPath = sqlite2mdb_cmd_parser::GetDestinationPath(&parser);
	if (sTargeDBPath.empty()) { 
        std::wcout << L"No destination database path defined. Use -d";
		return false;
	}

    const std::wstring sLogPath = sqlite2mdb_cmd_parser::GetLogPath(&parser);
    if (!sLogPath.empty()) { 
        dsDatabase::SetLogPath(sLogPath.c_str());
	}
    
    dsDatabase dbSource;
    if (!dbSource.OpenDB(sSrcDBPath.c_str())) {
        std::wcout << L"Can not open source database: " << sSrcDBPath.c_str();
        return false;
    }

    if (!dbSource.CloneTo(sTargeDBPath.c_str(), dsDBType::Dao)) {
        std::wstring sMessage = L"Database clone operation failed:\n";
        sMessage += L"Source: ";
        sMessage += sSrcDBPath;
        sMessage += L"\nTarget: ";
        sMessage += sTargeDBPath;
        
        dsDatabase::dbErrorHandler errorHandler = dbSource.GetErrorHandler();
        if (errorHandler) {
            errorHandler(sMessage.c_str());
        }
        
        std::wcout << sMessage.c_str();
        return false;
    }

    return true;
}

int wmain(int argc, wchar_t* argv[])
{
    const bool bRet = ::ExecuteCommandLineConv(argc, argv);
    AfxDaoTerm();

    if (!bRet) {
        return 1;
    }

    return 0;
}