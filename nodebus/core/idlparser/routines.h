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

#include "globals.h"
#include "driver.h"
#include <iostream>

using namespace std;

namespace jsonparser {

class r {
public:
	inline static variant_t *string2variant(string_t *str) {
		variant_t *variant = new variant_t(*str);
		delete str;
		return variant;
	}
	
	inline static variant_t *map2variant(variant_map_t *obj) {
		variant_t *variant = new variant_t(*obj);
		delete obj;
		return variant;
	}
	
	inline static variant_t *list2variant(variant_list_t *tbl) {
		variant_t *variant = new variant_t(*tbl);
		delete tbl;
		return variant;
	}
	
	inline static variant_map_t *mapAppendVariant(variant_map_t *obj, string_t *key, variant_t *val) {
		if (!obj) {
			obj = new variant_map_t();
		}
		obj->insert(*key, *val);
		delete key;
		delete val;
		return obj;
	}
	
	inline static variant_list_t *listAppendVariant(variant_list_t *tbl, variant_t *val) {
		if (!tbl) {
			tbl = new variant_list_t();
		}
		tbl->push_back(*val);
		delete val;
		return tbl;
	}
};

}
