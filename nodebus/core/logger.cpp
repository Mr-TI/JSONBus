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
