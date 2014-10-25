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

#ifndef HELLOWORLDSERVICE_H
#define HELLOWORLDSERVICE_H
#include <QObject>

namespace org { namespace mbedsys {


class Hello: public QObject {
	Q_OBJECT
public slots:
	virtual void sayHello(const QString &name);
	virtual void sayHello();
};

inline void Hello::sayHello(const QString& name) {}
inline void Hello::sayHello() {}

}}

#endif // HELLOWORLDSERVICE_H
