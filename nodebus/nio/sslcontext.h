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
