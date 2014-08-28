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
 * @brief NodeBus : Manifest management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_BUNDLEMANIFEST_H
#define NODEBUS_BUNDLEMANIFEST_H
#include <QMap>
#include <QVariant>

#define MANIFEST_BUNDLE_NAME(name) \
extern "C" {\
	const char *__manifest_get_BundleName() {\
		return name;\
	}\
}

#define MANIFEST_BUNDLE_SYMBOLIC_NAME(name) \
extern "C" {\
	const char *__manifest_get_BundleSymbolicName() {\
		return name;\
	}\
}

#define MANIFEST_BUNDLE_VERSION(version) \
extern "C" {\
	const char *__manifest_get_BundleVersion() {\
		return version;\
	}\
}

#define MANIFEST_REQUIRE_SERVICE(service) \
extern "C" {\
	const char *__manifest_get_RequireService() {\
		return version;\
	}\
}

#define MANIFEST_BUNDLE_ACTIVATOR(class_name) \
extern "C" {\
	void *__manifest_get_BundleActivator () {\
		return new class_name();\
	}\
}

namespace NodeBus {

typedef QMap<QString, QVariant> BundleManifest;

}

#endif //NODEBUS_BUNDLEMANIFEST_H
