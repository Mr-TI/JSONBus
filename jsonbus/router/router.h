/*
    Copyright (c) 2012, Emeric Verschuur <emericv@openihs.org>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the OpenIHS.org nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Emeric Verschuur ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Emeric Verschuur BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @brief JSONBus : JSONBus master router management.
 * @file router.h
 * @author Emeric VERSCHUUR <emericv@openihs.org>, (C) 2012
 */

#ifndef JSONBUS_ROUTER_H
#define JSONBUS_ROUTER_H

#include <jsonbus/core/slaveapplication.h>
#include <jsonbus/core/jsonserializer.h>

using namespace JSONBus;

/**
 * @brief JSONBus router management.
 */
class Router : public SlaveApplication {
public:
	/**
	 * @brief Service constructor.
	 */
	Router(int &argc, char **argv);

	/**
	 * @brief Service destructor.
	 */
	~Router();
	
protected:
	virtual void onRunLevelDefineArgs();
	virtual void onRunLevelSetup();
	
protected slots:
	/**
	 * @brief Inpout data treatment
	 * @param data
	 */
	virtual void onDataAvailable(QVariant data);
	
private:
	JSONSerializer m_jsonSerialiser;
};

#endif
