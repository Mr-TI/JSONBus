/*
    Copyright (c) 2012, Emeric Verschuur <contact@openihs.org>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the OpenIHS.org nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Emeric Verschuur ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Emeric Verschuur BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @brief JSONBus : Dynamic library management.
 * @file sharedlib.h
 * @author Emeric VERSCHUUR <contact@openihs.org>, (C) 2012
 */

#ifndef JSONBUS_SHAREDLIB_H
#define JSONBUS_SHAREDLIB_H

#include <jsonbus/core/exception.h>

#ifndef JSONBUS_EXPORT
#define JSONBUS_EXPORT
#endif

#ifndef WIN32
#include <dlfcn.h>
#endif
#include <QString>

namespace JSONBus {

jsonbus_declare_exception(SharedLibException, Exception);

/**
 * @brief Dynamic library management.
 */
class JSONBUS_EXPORT SharedLib: public QObject {
public:
	/**
	 * @brief SharedLib constructor.
	 * @param path Dynamic library path.
	 */
	SharedLib(const QString &path);

	/**
	 * @brief SharedLib destructor.
	 */
	~SharedLib();

	/**
	 * @brief Load the dynamic library.
	 * @param flags Flags.
	 * @throw Exception on error.
	 */
	void load(int flags = DefaultFlags);

	/**
	 * @brief Test if the dynamic library is loaded.
	 * @return true if it is loaded, otherwise false.
	 */
	inline bool isLoaded() {
		return handle == 0 ? false : true;
	}

	/**
	 * @brief Get a symbol from this dynamic library.
	 * @param symbol The symbol name.
	 * @return A pointer to the symbol.
	 * @throw Exception on error.
	 */
	void *getSymbol(const char *symbol);

	/**
	 * @brief Unload the dynamic library.
	 * @throw Exception on error.
	 */
	void unload();

	/**
	 * @brief Return the class name.
	 * @return "jsonbus::SharedLib"
	 */
	inline virtual QString className() const {
		return "jsonbus::SharedLib";
	}
private:

#ifdef WIN32
	typedef HMODULE handle_t;
#	define RTLD_LAZY 0
#	define RTLD_GLOBAL 0
#else
	typedef void* handle_t;
#endif
	
	static const int DefaultFlags = RTLD_LAZY | RTLD_GLOBAL;

	QString path;
	handle_t handle;
};

}

#endif
