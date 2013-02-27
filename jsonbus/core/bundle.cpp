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

#include "common.h"
#include "bundle.h"
#include "logger.h"

namespace JSONBus {

Bundle::Bundle(const QString &path) throw(BundleException)
: m_state(RESOLVED), m_context(*this), m_libFile(path) {
	try {
		m_libFile.load();
	} catch (LoadSharedLibException e) {
		throw BundleException("Bundle file not found! - " + e.message());
	}
	try {
		m_manifest["BundleName"] = (*(const char *(*)())(m_libFile.getSymbol("__manifest_get_BundleName")))();
	} catch (SymbolSharedLibException e) {
		throw BundleException("Invalid BundleName property! - " + e.message());
	}
	try {
		m_manifest["BundleSymbolicName"] = (*(const char *(*)())(m_libFile.getSymbol("__manifest_get_BundleSymbolicName")))();
	} catch (SymbolSharedLibException e) {
		throw BundleException("Invalid BundleName property! - " + e.message());
	}
	try {
		m_manifest["BundleVersion"] = (*(const char *(*)())(m_libFile.getSymbol("__manifest_get_BundleVersion")))();
	} catch (SymbolSharedLibException e) {
		throw BundleException("Invalid BundleName property! - " + e.message());
	}
	try {
		m_bundleActivator = (*(BundleActivatorPtr(*)())(m_libFile.getSymbol("__manifest_get_BundleActivator")))();
	} catch (SymbolSharedLibException e) {}
}

void Bundle::start() throw(BundleException) {
	m_state = STARTING;
	if (m_bundleActivator != null) {
		m_bundleActivator->start(m_context);
	}
	m_state = ACTIVE;
}

void Bundle::stop() throw(BundleException) {
	m_state = STOPPING;
	if (m_bundleActivator != null) {
		m_bundleActivator->stop(m_context);
	}
	m_state = RESOLVED;
}

}
