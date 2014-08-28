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

#ifndef NODEBUS_SSLCONTEXT_H
#define NODEBUS_SSLCONTEXT_H

#include <openssl/ssl.h>
#include <nodebus/core/sharedptr.h>

/**
 * @namespace
 */
namespace NodeBus {

/**
 * @brief SSL context
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
class SSLContext : public SharedData {
public:
	SSLContext();
	
    virtual ~SSLContext();
	
	virtual SSL_CTX *getCTX() = 0;
};

inline SSLContext::SSLContext() {
}

inline SSLContext::~SSLContext() {
}


typedef SharedPtr<SSLContext> SSLContextPtr;

}

#endif // NODEBUS_SSLCONTEXT_H
