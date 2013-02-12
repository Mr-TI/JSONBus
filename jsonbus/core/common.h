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

/**
 * @brief JSONBus : Commuon
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2013
 * @copyright Apache License, Version 2.0
 */

#ifndef JSONBUS_COMMON_H
#define JSONBUS_COMMON_H

#ifndef JSONBUS_EXPORT
#define JSONBUS_EXPORT
#endif

#include <iostream>
#include <QString>
#include <cxxabi.h>

#ifdef WIN32
#define JSONBUS_DEFAULT_LISTEN_ADDRESSES ":3693"
#define JSONBUS_DEFAULT_BASEDIR qApp->applicationDirPath()
#define JSONBUS_DEFAULT_PIDFILE JSONBUS_DEFAULT_BASEDIR "/jsonbusd.pid"
#else
#define JSONBUS_DEFAULT_LISTEN_ADDRESSES "/var/run/jsonbus.socket,:3693"
#define JSONBUS_DEFAULT_BASEDIR "/usr/lib/jsonbus"
#define JSONBUS_DEFAULT_PIDFILE "/var/run/jsonbusd.pid"
#endif
#define JSONBUS_DEFAULT_PLUGIN_DIR_PATH JSONBUS_DEFAULT_BASEDIR "/plugins"

namespace JSONBus {

/**
 * @brief Outpout stream operator
 * @param stream Outpout stream
 * @param data QString object
 * @return the stream reference
 */
inline JSONBUS_EXPORT std::ostream &operator << (std::ostream &stream, const QString &data) {
	return (stream << data.toUtf8().data());
}

/**
 * @brief Input stream operator
 * @param stream Input stream
 * @param data QString object
 * @return the stream reference
 */
inline JSONBUS_EXPORT std::istream &operator >> (QString &data, std::istream &stream) {
	std::string buff;
	stream >> buff;
	data = QString::fromUtf8(buff.c_str());
	return stream;
}

#ifdef WIN32
/**
 * @brief Get the message from the Windows error code
 * @param no Error code
 * @return QString opject
 */
inline JSONBUS_EXPORT QString getMessageError(DWORD no) {
	LPVOID lpMsgBuf;
	QString buf;
	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0, (LPTSTR) &lpMsgBuf, 0, NULL)) {
		buf = string((char*)lpMsgBuf);
		LocalFree(lpMsgBuf);
	} else {
		buf = "error no " + TOSTDSTR(no);
	}
	return buf;
}
#endif

inline QString demangle(const char *name) {
	int status;
	char *ret = abi::__cxa_demangle(name, 0, 0, &status);
	QString result(ret);
	free(ret);
	return result;
}

}

#endif
