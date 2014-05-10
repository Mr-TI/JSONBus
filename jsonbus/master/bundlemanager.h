/*
 *   Copyright 2014 Emeric Verschuur <emericv@openihs.org>
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#ifndef BUNDLEMANAGER_H
#define BUNDLEMANAGER_H
#include <QMap>
#include <QString>
#include <jsonbus/core/common.h>
#include <jsonbus/osgi/bundle.h>

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