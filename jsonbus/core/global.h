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
 * @brief JSONBus : Globals
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef JSONBUS_GLOBALS_H
#define JSONBUS_GLOBALS_H

#include <bits/wordsize.h>

#ifndef JSONBUS_EXPORT
#define JSONBUS_EXPORT
#endif

#if ( __WORDSIZE == 64 )
#	define JSONBUS_UINT8_HEXA_FMT "%02X"
#	define JSONBUS_UINT16_HEXA_FMT "%04X"
#	define JSONBUS_UINT32_HEXA_FMT "%08X"
#	define JSONBUS_UINT64_HEXA_FMT "%016lX"
#else
#	define JSONBUS_UINT8_HEXA_FMT "%02X"
#	define JSONBUS_UINT16_HEXA_FMT "%04X"
#	define JSONBUS_UINT32_HEXA_FMT "%08X"
#	define JSONBUS_UINT64_HEXA_FMT "%016llX"
#endif

#endif
