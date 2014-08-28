/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
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
 * @brief NodeBus : Commuon
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_COMMON_H
#define NODEBUS_COMMON_H

#include <nodebus/core/global.h>

#include <iostream>
#include <QString>
#include <cxxabi.h>
#include <stdio.h>

#ifdef WIN32
#define NODEBUS_DEFAULT_LISTEN_ADDRESSES ":3693"
#define NODEBUS_DEFAULT_BASEDIR qApp->applicationDirPath()
#define NODEBUS_DEFAULT_PIDFILE NODEBUS_DEFAULT_BASEDIR "/nodebusd.pid"
#else
#define NODEBUS_DEFAULT_LISTEN_ADDRESSES "/var/run/nodebus.socket,:3693"
#define NODEBUS_DEFAULT_BASEDIR "/usr/lib/nodebus"
#define NODEBUS_DEFAULT_PIDFILE "/var/run/nodebusd.pid"
#endif
#define NODEBUS_DEFAULT_PLUGIN_DIR_PATH NODEBUS_DEFAULT_BASEDIR "/bundles"
#define NODEBUS_DEFAULT_REGISTRY_SERVICE_NAME "org.mbedsys.registry.Registry"

namespace NodeBus {

/**
 * @brief Outpout stream operator
 * @param stream Outpout stream
 * @param data QString object
 * @return the stream reference
 */
inline NODEBUS_EXPORT std::ostream &operator << (std::ostream &stream, const QString &data) {
	return (stream << data.toLocal8Bit().data());
}

/**
 * @brief Input stream operator
 * @param stream Input stream
 * @param data QString object
 * @return the stream reference
 */
inline NODEBUS_EXPORT std::istream &operator >> (QString &data, std::istream &stream) {
	std::string buff;
	stream >> buff;
	data = QString::fromLocal8Bit(buff.c_str());
	return stream;
}

#ifdef WIN32
/**
 * @brief Get the message from the Windows error code
 * @param no Error code
 * @return QString opject
 */
inline NODEBUS_EXPORT QString getMessageError(DWORD no) {
	LPVOID lpMsgBuf;
	QString buf;
	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0, (LPTSTR) &lpMsgBuf, 0, NULL)) {
		buf = QString::fromLocal8Bit((char*)lpMsgBuf);
		LocalFree(lpMsgBuf);
	} else {
		buf = "error no " + TOSTDSTR(no);
	}
	return buf;
}
#endif

inline QString __demangle(const char *name) {
	int status;
	char *ret = abi::__cxa_demangle(name, 0, 0, &status);
	QString result(ret);
	free(ret);
	return result;
}

inline QString toHexString(uint8_t value) {
	char buff[5];
	snprintf(buff, 5, NODEBUS_UINT8_HEXA_FMT, value);
	return QString(buff);
}

inline QString toHexString(uint16_t value) {
	char buff[7];
	snprintf(buff, 7, NODEBUS_UINT16_HEXA_FMT, value);
	return QString(buff);
}

inline QString toHexString(uint32_t value) {
	char buff[11];
	snprintf(buff, 11, NODEBUS_UINT32_HEXA_FMT, value);
	return QString(buff);
}

inline QString toHexString(uint64_t value) {
	char buff[19];
	snprintf(buff, 19, NODEBUS_UINT64_HEXA_FMT, value);
	return QString(buff);
}

#if ( __WORDSIZE == 64 )
inline QString toHexString(const void *ptr) {
	return toHexString(uint64_t(ptr));
}
#else
inline QString toHexString(const void *ptr) {
	return toHexString(uint32_t(ptr));
}
#endif

}

using namespace NodeBus;

#endif
