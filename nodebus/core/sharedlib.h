/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@openihs.org>
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

/**
 * @brief NodeBus : Dynamic library management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
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
