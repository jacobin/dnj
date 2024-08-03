////////////////////////////////////////////////////////////////////////////////
# include <stdio.h>
# include <windows.h>
# include <tchar.h>
# include <assert.h>

// https://blog.csdn.net/guowenyan001/article/details/17259173
# include <Shlwapi.h>
# pragma comment(lib, "shlwapi.lib")  //Windows API PathFileExists

////////////////////////////////////////////////////////////////////////////////
void MyList(TCHAR* szDir, size_t nLen)
{
    assert(szDir[nLen - 1] == _T('\\'));
    WIN32_FIND_DATA Fd;

    szDir[nLen] = _T('*');
    szDir[nLen + 1] = _T('.');
    szDir[nLen + 2] = _T('*');
    szDir[nLen + 3] = NULL;
    FINDEX_INFO_LEVELS FIL = FindExInfoStandard;
    FINDEX_SEARCH_OPS FSO = FindExSearchNameMatch;
    HANDLE hFile = FindFirstFileEx( szDir, FIL, &Fd, FSO, NULL, 0);
    szDir[nLen] = NULL;
    if (INVALID_HANDLE_VALUE == hFile) return;

    for (BOOL bOk(TRUE); bOk; bOk = FindNextFile(hFile, &Fd)) {
        const auto& fN = Fd.cFileName;
        TCHAR szFullPath[2048] = {NULL};
        _stprintf(szFullPath, _T("%s%s"), szDir, fN);
        if (Fd.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) continue;
        if (Fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            const bool c = _T('.') == fN[0] && 0 == fN[1];
            const bool u = _T('.') == fN[0] && _T('.') == fN[1] && 0 == fN[2];
            if (c || u) continue;
            const auto nLen(_tcslen(szFullPath));
            szFullPath[nLen] = _T('\\');
            szFullPath[nLen + 1] = NULL;
            MyList(szFullPath, nLen + 1);
            szFullPath[nLen] = NULL;
        }
        _tprintf(_T("%s\n"), szFullPath);
    }
    FindClose(hFile);
}

////////////////////////////////////////////////////////////////////////////////
int _tmain(int argc, TCHAR* argv[])
{
    if (argc <= 1) {
        _ftprintf(stderr, _T("You must specify the target path."));
        exit(1);
    }
    if (!PathFileExists(argv[1])) {
        _ftprintf(stderr
            , _T("The path you specified does not exist."));
        exit(1);
    }
    if (!(GetFileAttributes(argv[1]) & FILE_ATTRIBUTE_DIRECTORY)) {
        _ftprintf(stderr
            , _T("The path you specified is not a directory."));
        exit(1);
    }

    TCHAR szTargetDir[2048] = { NULL };
    _stprintf(szTargetDir, _T("%s"), argv[1]);
    auto nLen = _tcslen(szTargetDir);
    if (_T('\\') != szTargetDir[nLen - 1])
    {
        szTargetDir[nLen] = _T('\\');
        szTargetDir[nLen + 1] = NULL;
    }

    MyList(szTargetDir, _tcslen(szTargetDir));
    return 0;
}

////////////////////////////////// END /////////////////////////////////////////