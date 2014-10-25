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

#ifndef BUNDLEMANAGER_H
#define BUNDLEMANAGER_H
#include <QMap>
#include <QString>
#include <nodebus/core/common.h>
#include <nodebus/bundle/bundle.h>

class BundleManager {
	QMap<QString, BundlePtr> m_bundles;
	BundleManager(const BundleManager&);
	BundleManager();
	static BundleManager m_instance;
public:
	
	/**
	 * @brief Get the global instance
	 */
	BundleManager &global();
	
	/**
	 * @brief Install a bundle
	 * 
	 * @param url Bundle URL
	 */
	void install(const QString &url);
};

inline BundleManager::BundleManager() {}

inline BundleManager& BundleManager::global() {
	return m_instance;
}

#endif // BUNDLEMANAGER_H
