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

#ifndef NODEBUS_BUNDLECONTEXT_H
#define NODEBUS_BUNDLECONTEXT_H

/**
 * @brief Bundle context management.
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @copyright Copyright (C) 2012-2014 MBEDSYS SAS
 * This library is released under the GNU Lesser General Public version 2.1
 */
#include <QObject>

namespace NodeBus {

class Bundle;
class BundleContext {
	BundleContext(const BundleContext&);
	Bundle &m_bundle;
public:
	
	/**
	 * @brief Bundle context constructor.
	 */
	BundleContext(Bundle &bundle);

	/**
	 * @brief Bundle context destructor.
	 */
	~BundleContext();
	
	/**
	 * @brief Return a reference to the bundle
	 * 
	 * @return a Bundle reference
	 */
	Bundle &bundle();
	
	/**
	 * @brief Register a service
	 */
	void registerService(QObject &service);
};

inline BundleContext::BundleContext(Bundle& bundle): m_bundle(bundle) {}
inline BundleContext::~BundleContext() {}
inline Bundle& BundleContext::bundle() {
	return m_bundle;
}

}

#endif // NODEBUS_BUNDLECONTEXT_H
