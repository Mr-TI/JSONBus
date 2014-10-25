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
	
	inline static variant_t *map2variant(object_t *obj) {
		variant_t *variant = new variant_t(*obj);
		delete obj;
		return variant;
	}
	
	inline static variant_t *list2variant(array_t *tbl) {
		variant_t *variant = new variant_t(*tbl);
		delete tbl;
		return variant;
	}
	
	inline static object_t *mapAppendVariant(object_t *obj, string_t *key, variant_t *val) {
		if (!obj) {
			obj = new object_t();
		}
		obj->insert(*key, *val);
		delete key;
		delete val;
		return obj;
	}
	
	inline static array_t *listAppendVariant(array_t *tbl, variant_t *val) {
		if (!tbl) {
			tbl = new array_t();
		}
		tbl->push_back(*val);
		delete val;
		return tbl;
	}
};

}
