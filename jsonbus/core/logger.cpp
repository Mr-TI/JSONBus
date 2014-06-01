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

#include <jsonbus/core/logger.h>
#include <stdio.h>
#include <stdarg.h>

namespace JSONBus {

const char *Logger::LEVEL_HDRS[][8] = {
	{
		NULL,
		"\n \033[1;41m\033[1;37m[ !! ]\033[0m\033[1;31m ",
		"\n \033[1;43m\033[1;37m[ ## ]\033[0m\033[1;33m ",
		"\n \033[1;42m\033[1;37m[ ii ]\033[0m\033[1;32m ",
		"\n \033[1;45m\033[1;37m[ ** ]\033[0m\033[1;35m ",
		"\n \033[1;44m\033[1;37m[ ++ ]\033[0m\033[1;34m ",
		"\n \033[1;46m\033[1;37m[ == ]\033[0m\033[1;36m ",
		"\n \033[1;47m\033[1;37m[ -- ]\033[0m\033[1;37m "
	},
	{
		NULL,
		"\n [ !! ] ",
		"\n [ ## ] ",
		"\n [ ii ] ",
		"\n [ ** ] ",
		"\n [ ++ ] ",
		"\n [ == ] ",
		"\n [ -- ] "
	}
};

const char *Logger::LEVEL_FOOT[] = {
	"\033[0m",
	""
};

const char **Logger::levelHdrs = LEVEL_HDRS[0];
const char *Logger::levelFoot = LEVEL_FOOT[0];

Logger::Level Logger::globalLevel = Logger::INFO;

Logger &Logger::operator<<(const QByteArray & t) {
	char buff[5];
	QByteArray::const_iterator it = t.begin();
	if (it != t.end()) {
		snprintf(buff, 3, "%02X", (uint8_t)*it);
		*this << buff;
		it++;
		for (; it != t.end(); it++) {
			snprintf(buff, 3, "%02X", (uint8_t)*it);
			*this << ' ' << buff;
		}
	}
	return *this;
}

Logger &Logger::operator<<(Exception &e) {
	m_stream << __demangle(typeid(e).name());
	QString out(e.what());
	m_stream << levelHdrs[m_level] << "  what(): " << e.message();
#ifdef JSONBUS_DISPLAY_BACKTRACE
	char **symTbl = backtrace_symbols(e.d->backtrace, e.d->backtraceSize);
	if (symTbl != NULL) {
		for (uint i = 0; i < e.d->backtraceSize; i++) {
			m_stream << levelHdrs[m_level] << '\t';
			m_stream << symTbl[i];
		}
		free(symTbl);
	}
#endif //JSONBUS_DISPLAY_BACKTRACE
	return *this;
}

extern std::atomic_uint_fast64_t __jsonbus_shared_data_count;

void __log_data_ref_init(void* data) {
	logFinest() << "SharedPtr::init   " << __demangle(typeid(*(SharedData*)data).name()) 
	<< "[" << toHexString(data) << "] (count=" << __jsonbus_shared_data_count << ")";
}

void __log_data_ref_delete(void* data) {
	logFinest() << "SharedPtr::delete " << __demangle(typeid(*(SharedData*)data).name()) 
	<< "[" << toHexString(data) << "] (count=" << (__jsonbus_shared_data_count-1) << ")";
}

}
