/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "registrywrapper.h"

RegistryWrapper::RegistryWrapper()
		: m_key(NULL)
		, m_enum_index(0)
{
}

RegistryWrapper::~RegistryWrapper()
{
	Close();
}

BOOL RegistryWrapper::Open(HKEY main, LPCTSTR key, BOOL create_if_none = TRUE, BOOL read_only = FALSE)
{
	Close();
	LONG ret;
	REGSAM sam = read_only ? KEY_READ : KEY_READ | KEY_WRITE;
	if (create_if_none)
		ret = RegCreateKeyEx(main, key, 0, NULL, 0, sam, NULL, &m_key, NULL);
	else
		ret = RegOpenKeyEx(main, key, 0, sam, &m_key);
	return (ret == ERROR_SUCCESS);
}

VOID RegistryWrapper::Close()
{
	if (m_key)
	{
		RegCloseKey(m_key);
		m_key = NULL;
	}
	m_enum_index = 0;
}

HKEY RegistryWrapper::GetHandle()
{
	return m_key;
}

BOOL RegistryWrapper::Info(LPDWORD subkey_count, LPDWORD subkey_max_length, LPDWORD value_count, LPDWORD value_name_max_length,
		LPDWORD value_max_length)
{
	return (ERROR_SUCCESS == RegQueryInfoKey(m_key, NULL, NULL, NULL, subkey_count, subkey_max_length, NULL, value_count,
			value_name_max_length,value_max_length, NULL, NULL));
}

DWORD RegistryWrapper::Query(LPCTSTR name, LPDWORD type, LPBYTE value, LPDWORD length)
{
	return (ERROR_SUCCESS == RegQueryValueEx(m_key, name, NULL, type, value, length));
}

BOOL RegistryWrapper::IsValueExist(LPCTSTR valuename)
{
	return (ERROR_FILE_NOT_FOUND != RegQueryValueEx(m_key, valuename, NULL, NULL, NULL, NULL));
}

VOID RegistryWrapper::EnumReset()
{
	m_enum_index = 0;
}

BOOL RegistryWrapper::EnumKey(LPTSTR name, LPDWORD length)
{
	return (ERROR_SUCCESS == RegEnumKeyEx(m_key, m_enum_index++, name, length, NULL, NULL, NULL, NULL));
}