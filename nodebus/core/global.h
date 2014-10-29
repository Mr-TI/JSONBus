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
 * @brief NodeBus : Globals
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @copyright Copyright (C) 2012-2014 MBEDSYS SAS
 * This library is released under the GNU Lesser General Public version 2.1
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
#define COLOR_WHI_HL "\033[1;47m\033[1;37m"

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
