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

#ifndef IDLGLOBALS_H
#define IDLGLOBALS_H

#include <QVariant>
#include <QString>
#include <QMap>
#include <QList>

namespace idlparser {

#ifndef JSONPARSER_DRIVER_H_TYPES
#define JSONPARSER_DRIVER_H_TYPES
typedef QVariant variant_t;
typedef QString string_t;
typedef QMap<string_t, variant_t> variant_map_t;
typedef QList<variant_t> variant_list_t;
#endif // JSONPARSER_DRIVER_H_TYPES

class Driver;

}

#endif // IDLGLOBALS_H
