/*
 * Copyright (C) 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <sharedlib.h>
#include <common.h>

namespace NodeBus {

SharedLib::SharedLib(const QString &path)
		: path(path),
		handle(0) {
}

SharedLib::~SharedLib() {
	if (handle != 0)
		unload();
}

void SharedLib::load(int flags) {
	QString lastError;
	if (handle != 0) {
		lastError = tr("Dynamic library already loaded.");
		throw InvalidSharedLibException(tr("Fail to load the dynamic library : ") + lastError);
	}
#ifdef WIN32
	handle = LoadLibrary(path.c_str());
	if (!handle) {
		lastError = "LoadLibrary(): " + getMessageError(GetLastError());
		throw LoadSharedLibException(tr("Fail to load the dynamic library : " + lastError);
	}
#else
	char *dl_error;
	handle = dlopen(path.toAscii(), flags);
	if (!handle) {
		dl_error = dlerror();
		lastError = "dlopen(): " + QString::fromLocal8Bit(dl_error);
		throw LoadSharedLibException(tr("Fail to load the dynamic library : ") + lastError);
	}
#endif
}

void *SharedLib::getSymbol(const char *symbol) {
	QString lastError;
	if (handle == 0) {
		lastError = tr("Dynamic library not loaded.");
		throw InvalidSharedLibException(tr("Fail to get a symbol : ") + lastError);
	}
	void *ptr;
#ifdef WIN32
	ptr = GetProcAddress(handle, symbol);
	if (ptr == NULL)  {
		lastError = "GetProcAddress(): " + getMessageError(GetLastError());
		throw SymbolSharedLibException(tr("Fail to get the symbol : ") + lastError);
	}
#else
	char *dl_error;
	ptr = dlsym(handle, symbol);
	if ((dl_error = dlerror()) != NULL)  {
		lastError = "dlsym(): " + QString::fromLocal8Bit(dl_error);
		throw SymbolSharedLibException(tr("Fail to get the symbol : ") + lastError);
	}
#endif
	return ptr;
}

void SharedLib::unload() {
	QString lastError;
	if (handle == 0) {
		lastError = tr("Dynamic library not loaded.");
		throw InvalidSharedLibException(tr("Fail to unload the plugin : ") + lastError);
	}
#ifdef WIN32
	FreeLibrary(handle);
#else
	dlclose(handle);
#endif
	handle = 0;
}

}
