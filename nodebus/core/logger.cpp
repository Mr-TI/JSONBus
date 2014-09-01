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

#include <nodebus/core/logger.h>
#include <stdio.h>
#include <stdarg.h>

namespace NodeBus {

const char *Logger::LEVEL_HDRS[][8] = {
	{
		NULL,
		"\n " COLOR_RED_HL "[ !! ]" COLOR_RED " ",
		"\n " COLOR_YEL_HL "[ ## ]" COLOR_YEL " ",
		"\n " COLOR_GRE_HL "[ ii ]" COLOR_GRE " ",
		"\n " COLOR_VIO_HL "[ ** ]" COLOR_VIO " ",
		"\n " COLOR_BLU_HL "[ ++ ]" COLOR_BLU " ",
		"\n " COLOR_MAG_HL "[ == ]" COLOR_MAG " ",
		"\n " COLOR_WHI_HL "[ -- ]" COLOR_WHI " "
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
	COLOR_RST,
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
#ifdef NODEBUS_DISPLAY_BACKTRACE
	char **symTbl = backtrace_symbols(e.d->backtrace, e.d->backtraceSize);
	if (symTbl != NULL) {
		for (uint i = 0; i < e.d->backtraceSize; i++) {
			m_stream << levelHdrs[m_level] << '\t';
			m_stream << symTbl[i];
		}
		free(symTbl);
	}
#endif //NODEBUS_DISPLAY_BACKTRACE
	return *this;
}

extern std::atomic_uint_fast64_t __nodebus_shared_data_count;

void __log_data_ref_init(void* data) {
	logFinest() << "SharedPtr::init   " << __demangle(typeid(*(SharedData*)data).name()) 
	<< "[" << toHexString(data) << "] (count=" << __nodebus_shared_data_count << ")";
}

void __log_data_ref_delete(void* data) {
	logFinest() << "SharedPtr::delete " << __demangle(typeid(*(SharedData*)data).name()) 
	<< "[" << toHexString(data) << "] (count=" << (__nodebus_shared_data_count-1) << ")";
}

}
