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

#ifndef NODEBUS_SHAREDDATA_H
#define NODEBUS_SHAREDDATA_H

#include <atomic>

namespace NodeBus {

class SharedData {
public:
	/// @brief Reference count
	std::atomic_uint_fast16_t ptrNbRef;
	
	/**
	 * @brief Shared data constructor
	 */ 
	SharedData();
	
	/**
	 * @brief Shared data destructor
	 */ 
	virtual ~SharedData();
private:
	SharedData(const SharedData& other);
    SharedData &operator=(const SharedData &);
};

// inline SharedData::SharedData(): ref(0) {
// }
// inline SharedData::~SharedData() {
// }

}

#endif // NODEBUS_SHAREDDATA_H
