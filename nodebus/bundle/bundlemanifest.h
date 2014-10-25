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
