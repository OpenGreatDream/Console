﻿#include "ConRegistry.hpp"
#include "WinVersion.hpp"
#include "VTConverter.hpp"
#include "String.hpp"
#include "_winreg.hpp"
#include <strsafe.h>

using namespace winreg;

namespace MinConsoleNative
{
    //*************************************************************
    //
    //  RegDelnodeRecurse()
    //
    //  Purpose:    Deletes a registry key and all its subkeys / values.
    //
    //  Parameters: hKeyRoot    -   Root key
    //              lpSubKey    -   SubKey to delete
    //
    //  Return:     TRUE if successful.
    //              FALSE if an error occurs.
    //
    //*************************************************************
    bool RegDelnodeRecurse(HKEY hKeyRoot, LPTSTR lpSubKey)
    {
        LPTSTR lpEnd;
        LONG lResult;
        DWORD dwSize;
        TCHAR szName[MAX_PATH];
        HKEY hKey;
        FILETIME ftWrite;

        // First, see if we can delete the key without having
        // to recurse.

        lResult = RegDeleteKey(hKeyRoot, lpSubKey);

        if (lResult == ERROR_SUCCESS)
            return TRUE;

        lResult = RegOpenKeyEx(hKeyRoot, lpSubKey, 0, KEY_READ, &hKey);

        if (lResult != ERROR_SUCCESS)
        {
            if (lResult == ERROR_FILE_NOT_FOUND)
            {
                //printf("Key not found.\n");
                return TRUE;
            }
            else
            {
                //printf("Error opening key.\n");
                return FALSE;
            }
        }

        // Check for an ending slash and add one if it is missing.

        lpEnd = lpSubKey + lstrlen(lpSubKey);

        if (*(lpEnd - 1) != TEXT('\\'))
        {
            *lpEnd = TEXT('\\');
            lpEnd++;
            *lpEnd = TEXT('\0');
        }

        // Enumerate the keys

        dwSize = MAX_PATH;
        lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
            NULL, NULL, &ftWrite);

        if (lResult == ERROR_SUCCESS)
        {
            do
            {

                *lpEnd = TEXT('\0');
                StringCchCat(lpSubKey, MAX_PATH * 2, szName);

                if (!RegDelnodeRecurse(hKeyRoot, lpSubKey))
                {
                    break;
                }

                dwSize = MAX_PATH;

                lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
                    NULL, NULL, &ftWrite);

            }
            while (lResult == ERROR_SUCCESS);
        }

        lpEnd--;
        *lpEnd = TEXT('\0');

        RegCloseKey(hKey);

        // Try again to delete the key.

        lResult = RegDeleteKey(hKeyRoot, lpSubKey);

        if (lResult == ERROR_SUCCESS)
            return TRUE;

        return FALSE;
    }

    //*************************************************************
    //
    //  RegDelnode()
    //
    //  Purpose:    Deletes a registry key and all its subkeys / values.
    //
    //  Parameters: hKeyRoot    -   Root key
    //              lpSubKey    -   SubKey to delete
    //
    //  Return:     TRUE if successful.
    //              FALSE if an error occurs.
    //
    //*************************************************************
    bool RegDelnode(HKEY hKeyRoot, LPCTSTR lpSubKey)
    {
        TCHAR szDelKey[MAX_PATH * 2];
        StringCchCopy(szDelKey, MAX_PATH * 2, lpSubKey);

        return RegDelnodeRecurse(hKeyRoot, szDelKey);
    }

    EXPORT_FUNC_EX(bool) MinIsLegacyConsole()
    {
        //Windows10以下都是旧版控制台
        if (!winVersion.IsWindows10OrLater())
        {
            return true;
        }

        RegKey key{ HKEY_CURRENT_USER, L"Console" };
        auto result = key.TryGetDwordValue(L"ForceV2");
        //找到了ForceV2则返回相反结果
        if (result.has_value())
        {
            return !result.value();
        }
        //如果找不到ForceV2则判断是否支持VT, 如果支持VT一定不是旧版控制台
        else
        {
            return !VTConverter::VTSupport();
        }
    }

    EXPORT_FUNC_EX(bool) MinEnableLegacyConsole(bool enable)
    {
        RegKey key{ HKEY_CURRENT_USER, L"Console" };
        key.SetDwordValue(L"ForceV2", !enable);
        return true;
    }

    EXPORT_FUNC_EX(bool) MinDeleteConsoleRegistry()
    {
        return RegDelnode(HKEY_CURRENT_USER, L"Console");
    }

    EXPORT_FUNC_EX(bool) MinWTIsDefaultConsole()
    {
        RegKey key;

        RegResult r = key.TryOpen(HKEY_CURRENT_USER, L"Console\\%%Startup");

        if (!r.Failed())
        {
            auto v1 = key.TryGetStringValue(L"DelegationConsole");
            auto v2 = key.TryGetStringValue(L"DelegationTerminal");
            if (v1.has_value() && v2.has_value())
            {
                //即使有值也必须检查值, 不能为{00000000-0000-0000-0000-000000000000}
                if (!String::Compare(v1.value(), L"{00000000-0000-0000-0000-000000000000}") && !String::Compare(v2.value(), L"{00000000-0000-0000-0000-000000000000}"))
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool ConRegistry::IsLegacyConsole()
    {
        return MinIsLegacyConsole();
    }

    bool ConRegistry::EnableLegacyConsole(bool enable)
    {
        return MinEnableLegacyConsole(enable);
    }

    bool ConRegistry::DeleteConsoleRegistry()
    {
        return MinDeleteConsoleRegistry();
    }

    bool ConRegistry::WTIsDefaultConsole()
    {
        return MinWTIsDefaultConsole();
    }
}