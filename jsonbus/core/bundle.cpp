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

const char* Bundle::toString(Bundle::State state) {
	switch (state) {
	case UNINSTALLED:
		return "UNINSTALLED";
	case RESOLVED:
		return "RESOLVED";
	case STARTING:
		return "STARTING";
	case ACTIVE:
		return "ACTIVE";
	case STOPPING:
		return "STOPPING";
	default:
		return "";
	}
}

Bundle::Bundle(const QString &path) throw(BundleException)
: m_state(RESOLVED), m_context(*this), m_libFile(path) {
	m_owner = QThread::currentThreadId();
	try {
		m_libFile.load();
	} catch (LoadSharedLibException e) {
		throw BundleException("Unable to load the bundle file: " + e.message());
	}
	try {
		m_manifest["Bundle-Name"] = (*(const char *(*)())(m_libFile.getSymbol("__manifest_get_BundleName")))();
	} catch (SymbolSharedLibException e) {
		throw BundleException("Invalid Bundle-Name property! - " + e.message());
	}
	try {
		m_manifest["Bundle-SymbolicName"] = (*(const char *(*)())(m_libFile.getSymbol("__manifest_get_BundleSymbolicName")))();
	} catch (SymbolSharedLibException e) {
		throw BundleException("Invalid Bundle-SymbolicName property! - " + e.message());
	}
	try {
		m_manifest["Bundle-Version"] = (*(const char *(*)())(m_libFile.getSymbol("__manifest_get_BundleVersion")))();
	} catch (SymbolSharedLibException e) {
		throw BundleException("Invalid Bundle-Version property! - " + e.message());
	}
	try {
		m_manifest["Require-Service"] = (*(const char *(*)())(m_libFile.getSymbol("__manifest_get_RequireService")))();
	} catch (SymbolSharedLibException e) {}
	m_libFile.unload();
}

void Bundle::install() {
	if (QThread::currentThreadId() != m_owner) {
		//TODO: Operation execution by the owner thread
		throw UnsupportedOperationException("Bundle::install");
	}
	if (m_state == UNINSTALLED) {
		try {
			m_libFile.load();
		} catch (LoadSharedLibException e) {
			throw BundleException("Unable to load the bundle file: " + e.message());
		}
		try {
			m_bundleActivator = (*(BundleActivator *(*)())(m_libFile.getSymbol("__manifest_get_BundleActivator")))();
		} catch (SymbolSharedLibException e) {}
		return;
	}
	throw IllegalOperationException("Try to install a bundle with state " + toString(m_state));
}

void Bundle::uninstall() {
	if (QThread::currentThreadId() != m_owner) {
		//TODO: Operation execution by the owner thread
		throw UnsupportedOperationException("Bundle::uninstall");
	}
	if (m_state == ACTIVE) {
		stop();
	}
	if (m_state == RESOLVED) {
		m_bundleActivator = null;
		m_libFile.unload();
		m_state = UNINSTALLED;
		return;
	}
	throw IllegalOperationException("Try to uninstall a bundle with state " + toString(m_state));
}

void Bundle::start() throw(BundleException) {
	if (QThread::currentThreadId() != m_owner) {
		//TODO: Operation execution by the owner thread
		throw UnsupportedOperationException("Bundle::install");
	}
	if (m_state == UNINSTALLED) {
		install();
	}
	if (m_state == RESOLVED) {
		m_state = STARTING;
		if (m_bundleActivator != null) {
			m_bundleActivator->start(m_context);
		}
		m_state = ACTIVE;
	}
	throw IllegalOperationException("Try to start a bundle with state " + toString(m_state));
}

void Bundle::stop() throw(BundleException) {
	if (QThread::currentThreadId() != m_owner) {
		//TODO: Operation execution by the owner thread
		throw UnsupportedOperationException("Bundle::install");
		return;
	}
	if (m_state == ACTIVE) {
		m_state = STOPPING;
		if (m_bundleActivator != null) {
			m_bundleActivator->stop(m_context);
		}
		m_state = RESOLVED;
	}
	throw IllegalOperationException("Try to stop a bundle with state " + toString(m_state));
}

}
