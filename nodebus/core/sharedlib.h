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

/**
 * @brief NodeBus : Dynamic library management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_SHAREDLIB_H
#define NODEBUS_SHAREDLIB_H

#include <nodebus/core/exception.h>
#include <nodebus/core/sharedptr.h>

#ifndef NODEBUS_EXPORT
#define NODEBUS_EXPORT
#endif

#ifndef WIN32
#include <dlfcn.h>
#endif
#include <QString>

namespace NodeBus {

nodebus_declare_exception(SharedLibException, Exception);
nodebus_declare_exception(InvalidSharedLibException, SharedLibException);
nodebus_declare_exception(LoadSharedLibException, SharedLibException);
nodebus_declare_exception(SymbolSharedLibException, SharedLibException);

/**
 * @brief Dynamic library management.
 */
class NODEBUS_EXPORT SharedLib: public QObject, public SharedData {
	SharedLib(const SharedLib &other);
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
	bool isLoaded();

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

/// @brief Plugin shared pointer type
typedef SharedPtr<SharedLib> SharedLibPtr;

inline bool SharedLib::isLoaded() {
	return handle == 0 ? false : true;
}

}

#endif
