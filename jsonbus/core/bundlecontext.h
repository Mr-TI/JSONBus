/*
 *   Copyright 2013 Emeric Verschuur <emericv@openihs.org>
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

#ifndef JSONBUS_BUNDLECONTEXT_H
#define JSONBUS_BUNDLECONTEXT_H

/**
 * @brief Bundle context management.
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2013
 * @copyright Apache License, Version 2.0
 */

namespace JSONBus {

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
};

inline BundleContext::BundleContext(Bundle& bundle): m_bundle(bundle) {}
inline BundleContext::~BundleContext() {}
inline Bundle& BundleContext::bundle() {
	return m_bundle;
}

}

#endif // JSONBUS_BUNDLECONTEXT_H
