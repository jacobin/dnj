:://///////////////////////////////////////////////////////////////////////////
:: https://superuser.com/questions/528487/list-all-files-and-dirs-without-recursion-with-junctions
::  For example, if the directory
::      d:\testDirLink\link
::  is a MKLINK, and there is a file
::      aFile
::  in d:\testDirLink\link, then
::      1) "dir /a-l /b /s d:\testDirLink" will not list
::              d:\testDirLink\link
::        , but will list its subfile
::              d:\testDirLink\link\aFile;
::      2) "list_abs_exclude_reparse_points d:\testDirLink" will not list
::              d:\testDirLink\link
::        , nor will it list its subfile
::              d:\testDirLink\link\aFile
:: ----------------------------------------------------------------------------
:: Example:
@echo off
:Main
    setlocal
    call :list_abs_exclude_reparse_points %*
    endlocal &echo on & goto :eof

:://///////////////////////////////////////////////////////////////////////////
:list_abs_exclude_reparse_points <paths>
    :_shift_next_param
    if "%~1"=="" (
        call :_listDir "."
    ) else (
        call :_listDir "%~1"
    )

    if "%~2"=="" goto :eof
    shift
    goto :_shift_next_param

:://///////////////////////////////////////////////////////////////////////////
:_listDir <path>
    set "targetDir=%~1"
    set "theLastChar=%targetDir:~-1%"

    rem targetDirWithLastSlash, targetDirNoLastSlash
    if "%theLastChar%"=="\" (
        set "targetDirWithLastSlash=%targetDir%" & set "targetDirNoLastSlash=%targetDir:~0,-1%"
    ) else (
        set "targetDirWithLastSlash=%targetDir%\" & set "targetDirNoLastSlash=%targetDir%"
    )

    rem
    for /F "usebackq delims=" %%F in (`dir /B /A-L "%targetDirWithLastSlash%"`) do (
        echo "%targetDirNoLastSlash%\%%~F"
        if exist "%targetDirNoLastSlash%\%%~F\*.*" (
            call :_listDir "%targetDirNoLastSlash%\%%~F"
        )
    )
    goto :eof