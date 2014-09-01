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
 * @brief NodeBus : Globals
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_GLOBALS_H
#define NODEBUS_GLOBALS_H

#include <bits/wordsize.h>

#ifndef NODEBUS_EXPORT
#define NODEBUS_EXPORT
#endif

#if ( __WORDSIZE == 64 )
#	define NODEBUS_UINT8_HEXA_FMT "%02X"
#	define NODEBUS_UINT16_HEXA_FMT "%04X"
#	define NODEBUS_UINT32_HEXA_FMT "%08X"
#	define NODEBUS_UINT64_HEXA_FMT "%016lX"
#else
#	define NODEBUS_UINT8_HEXA_FMT "%02X"
#	define NODEBUS_UINT16_HEXA_FMT "%04X"
#	define NODEBUS_UINT32_HEXA_FMT "%08X"
#	define NODEBUS_UINT64_HEXA_FMT "%016llX"
#endif

#define COLOR_RED_HL "\033[1;41m\033[1;37m"
#define COLOR_YEL_HL "\033[1;43m\033[1;37m"
#define COLOR_GRE_HL "\033[1;42m\033[1;37m"
#define COLOR_VIO_HL "\033[1;45m\033[1;37m"
#define COLOR_BLU_HL "\033[1;44m\033[1;37m"
#define COLOR_MAG_HL "\033[1;46m\033[1;37m"
#define COLOR_WHI_HL "\033[1;45m\033[1;37m"

#define COLOR_RED "\033[0m\033[1;31m"
#define COLOR_YEL "\033[0m\033[1;33m"
#define COLOR_GRE "\033[0m\033[1;32m"
#define COLOR_VIO "\033[0m\033[1;35m"
#define COLOR_BLU "\033[0m\033[1;34m"
#define COLOR_MAG "\033[0m\033[1;36m"
#define COLOR_WHI "\033[0m\033[1;37m"

#define COLOR_RST "\033[0m"

/// @brief Format
enum FileFormat {
	/// @brief JSON format
	JSON,
	/// @brief BCON format
	BCON,
	/// @brief BSON format
	BSON,
	/// @brief IDL format
	IDL
};

#endif
