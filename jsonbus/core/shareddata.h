/*
 *   Copyright 2014 Emeric Verschuur <emericv@openihs.org>
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#ifndef JSONBUS_SHAREDDATA_H
#define JSONBUS_SHAREDDATA_H

#include <atomic>

namespace JSONBus {

class SharedData {
public:
	/// @brief Reference count
	std::atomic_uint_fast16_t ref;
	
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
inline SharedData::SharedData(): ref(0) {}
inline SharedData::~SharedData() {}

}

#endif // JSONBUS_SHAREDDATA_H
