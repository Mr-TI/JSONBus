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

#include <nodebus/core/common.h>
#include <nodebus/core/logger.h>
#include "bundle.h"

namespace NodeBus {

QString Bundle::toString(Bundle::State state) {
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

Bundle::Bundle(const QString &path)
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
		m_bundleActivator = nullptr;
		m_libFile.unload();
		m_state = UNINSTALLED;
		return;
	}
	throw IllegalOperationException("Try to uninstall a bundle with state " + toString(m_state));
}

void Bundle::start() {
	if (QThread::currentThreadId() != m_owner) {
		//TODO: Operation execution by the owner thread
		throw UnsupportedOperationException("Bundle::install");
	}
	if (m_state == UNINSTALLED) {
		install();
	}
	if (m_state == RESOLVED) {
		m_state = STARTING;
		if (m_bundleActivator != nullptr) {
			m_bundleActivator->start(m_context);
		}
		m_state = ACTIVE;
	}
	throw IllegalOperationException("Try to start a bundle with state " + toString(m_state));
}

void Bundle::stop() {
	if (QThread::currentThreadId() != m_owner) {
		//TODO: Operation execution by the owner thread
		throw UnsupportedOperationException("Bundle::install");
		return;
	}
	if (m_state == ACTIVE) {
		m_state = STOPPING;
		if (m_bundleActivator != nullptr) {
			m_bundleActivator->stop(m_context);
		}
		m_state = RESOLVED;
	}
	throw IllegalOperationException("Try to stop a bundle with state " + toString(m_state));
}

}
