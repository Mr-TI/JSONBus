/*
 *   Copyright 2012-2013 Emeric Verschuur <emericv@openihs.org>
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *		   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <sharedlib.h>
#include <common.h>

namespace JSONBus {

SharedLib::SharedLib(const QString &path)
		: path(path),
		handle(0) {
}

SharedLib::~SharedLib() {
	if (handle != 0)
		unload();
}

void SharedLib::load(int flags) throw(SharedLibException) {
	QString lastError;
	if (handle != 0) {
		lastError = "Dynamic library already loaded.";
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
		lastError = "dlopen(): " + QString(dl_error);
		throw LoadSharedLibException(tr("Fail to load the dynamic library : ") + lastError);
	}
#endif
}

void *SharedLib::getSymbol(const char *symbol) throw(SharedLibException) {
	QString lastError;
	if (handle == 0) {
		lastError = "Dynamic library not loaded.";
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
		lastError = "dlsym(): " + QString(dl_error);
		throw SymbolSharedLibException(tr("Fail to get the symbol : ") + lastError);
	}
#endif
	return ptr;
}

void SharedLib::unload() throw(SharedLibException) {
	QString lastError;
	if (handle == 0) {
		lastError = "Dynamic library not loaded.";
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
