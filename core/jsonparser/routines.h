/*
    Copyright (c) 2012, Emeric Verschuur <emericv@openihs.org>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the OpenIHS.org nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Emeric Verschuur <emericv@openihs.org> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Emeric Verschuur <emericv@openihs.org> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
