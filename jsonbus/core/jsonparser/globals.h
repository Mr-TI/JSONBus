/*
 *   Copyright 2012-2013 Emeric Verschuur <emericv@openihs.org>
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

#ifndef GLOBALS_H
#define GLOBALS_H

#include <QVariant>
#include <QString>
#include <QMap>
#include <QList>

namespace jsonparser {

#ifndef JSONPARSER_DRIVER_H_TYPES
#define JSONPARSER_DRIVER_H_TYPES
typedef QVariant variant_t;
typedef QString string_t;
typedef QMap<string_t, variant_t> object_t;
typedef QList<variant_t> array_t;
#endif // JSONPARSER_DRIVER_H_TYPES

class Driver;

}

#endif // GLOBALS_H
