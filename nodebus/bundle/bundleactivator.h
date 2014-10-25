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
 * @brief NodeBus : Bundle activator management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */

#ifndef NODEBUS_BUNDLEACTIVATOR_H
#define NODEBUS_BUNDLEACTIVATOR_H

#include <nodebus/core/exception.h>
#include <nodebus/core/settings.h>
#include <nodebus/core/sharedptr.h>
#include <nodebus/bundle/bundlecontext.h>

#ifndef NODEBUS_EXPORT
#define NODEBUS_EXPORT
#endif

#include <QString>

namespace NodeBus {

nodebus_declare_exception(BundleActivatorException, Exception);

/**
 * @brief Bundle activator management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class NODEBUS_EXPORT BundleActivator: public SharedData {
	
private:
	BundleActivator(const BundleActivator &other);
	
public:
	
	/**
	 * @brief Bundle activator constructor.
	 */
	BundleActivator();

	/**
	 * @brief Bundle activator destructor.
	 */
	~BundleActivator();
	
	/**
	 * @brief Function called on bundle start
	 */
	virtual void start(BundleContext &context) = 0;
	
	/**
	 * @brief Function called on bundle stop
	 */
	virtual void stop(BundleContext &context) = 0;
	
};

/// @brief Bundle activator shared pointer type
typedef SharedPtr<BundleActivator> BundleActivatorPtr;

inline BundleActivator::BundleActivator() {}
inline BundleActivator::~BundleActivator() {}

}

#endif
