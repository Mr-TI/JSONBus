/*
    Copyright (c) 2011, Emeric Verschuur <emericv@gmail.com>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Emeric Verschuur <emericv@gmail.com> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Emeric Verschuur <emericv@gmail.com> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @brief JSONBus : Exception management.
 * @file exception.h
 * @author Emeric VERSCHUUR <contact@mr-ti.com>, (C) 2012
 */
#ifndef UNICOMCTRL_EXCEPTION_H
#define UNICOMCTRL_EXCEPTION_H

#include <QString>

#define declare_exception(name, parent) class JSONBUS_EXPORT name:public parent {public:inline name(const QString &msg):parent(msg) {};};

namespace unicomctrl {

/**
 * This class can manage exceptions.
 * @brief JSONBus : Exceptions.
 */
class JSONBUS_EXPORT Exception : public QObject {
public:
    /**
     * @brief Exception constructor.
     * @param message exeption message.
     */
    inline Exception(std::string message):QObject(),message(message) {}

    /**
     * @brief Exception destructor.
     */
    inline ~Exception() {}

    /**
     * @brief Get the exeption message.
     * @return std::string message.
     */
    inline const std::string &getMsg() const {
        return message;
    }

    /**
     * @brief Get the exeption message.
     * @return std::string message.
     */
    inline const std::string &getMessage() const {
        return message;
    }

    /**
     * @brief Return the class name.
     * @return "unicomctrl::Exception"
     */
    inline virtual std::string className() const {
        return "UniComCtrl::Exception";
    };
private:
    std::string message;
};

declare_exception(FatalErrorException, Exception);

}

#endif
