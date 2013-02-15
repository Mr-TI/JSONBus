/*
 *   Copyright 2012-2013 Emeric Verschuur <emericv@openihs.org>
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

#ifndef JSONBUS_POINTER_H
#define JSONBUS_POINTER_H

#include <jsonbus/core/exception.h>
#include <jsonbus/shareddata.h>

namespace JSONBus {

jsonbus_declare_exception(PointerException, JSONBus::Exception);
jsonbus_declare_exception(NullPointerException, PointerException);

template <typename T> class SharedPtr {
	T *m_data;
public:
	SharedPtr();
	SharedPtr(T *data);
	~SharedPtr();
	SharedPtr(const SharedPtr<T> &other);
	template<class X>
	SharedPtr(const SharedPtr<X> &other);
	T *data();
	const T *data() const;
	T &operator= (const T *data);
	T &operator= (const SharedPtr<T>& other);
	template<class X>
	T &operator= (const SharedPtr<X>& other);
	T &operator*() const;
	T *operator->();
	T *operator->() const;
	bool operator== (const T *data);
	bool operator== (const SharedPtr<T>& other);
	template<class X>
	bool operator== (const SharedPtr<X>& other);
	bool operator!= (const T *data);
	bool operator!= (const SharedPtr<T>& other);
	template<class X>
	bool operator!= (const SharedPtr<X>& other);
};

/// @brief Null pointer
const SharedPtr<SharedData> null;

template <typename T>
inline SharedPtr<T>::SharedPtr(): m_data(NULL) {}
template <typename T>
inline SharedPtr<T>::~SharedPtr() {
	if (m_data != NULL) {
		if (m_data->ref.fetch_sub(1) == 1) {
			delete m_data;
		}
	}
}
template <typename T>
inline SharedPtr<T>::SharedPtr(T* data): m_data(data) {
	if (m_data != NULL) {
		m_data->ref++;
	}
}
template <typename T>
inline SharedPtr<T>::SharedPtr(const SharedPtr< T >& other): m_data(other.m_data) {
	if (m_data != NULL) {
		m_data->ref++;
	}
}
template <typename T>
template <typename X>
inline SharedPtr<T>::SharedPtr(const SharedPtr< X >& other): m_data((T*)(other.data())) {
	if (m_data != NULL) {
		m_data->ref++;
	}
}
template <typename T>
inline T* SharedPtr<T>::data() {
	return m_data;
}
template <typename T>
inline const T* SharedPtr<T>::data() const {
	return m_data;
}
template <typename T>
inline T& SharedPtr<T>::operator=(const SharedPtr<T>& other) {
	return operator=(other.m_data);
}
template <typename T>
template <typename X>
inline T& SharedPtr<T>::operator=(const SharedPtr<X>& other) {
	return operator=(static_cast<T*>(other.m_data));
}
template <typename T>
T& SharedPtr<T>::operator=(const T *data) {
	if (m_data == data) {
		return *this;
	} else if (m_data != NULL) {
		if (m_data->ref.fetch_sub(1) == 1) {
			delete m_data;
		}
	}
}
template <typename T>
inline bool SharedPtr<T>::operator==(const SharedPtr<T>& other) {
	return m_data == other.m_data;
}
template <typename T>
template <typename X>
inline bool SharedPtr<T>::operator==(const SharedPtr<X>& other) {
	return m_data == other.data();
}
template <typename T>
inline bool SharedPtr<T>::operator==(const T *data) {
	return m_data == data;
}
template <typename T>
inline bool SharedPtr<T>::operator!=(const SharedPtr<T>& other) {
	return m_data != other.m_data;
}
template <typename T>
template <typename X>
inline bool SharedPtr<T>::operator!=(const SharedPtr<X>& other) {
	return m_data != other.data();
}
template <typename T>
inline bool SharedPtr<T>::operator!=(const T *data) {
	return m_data != data;
}
template <typename T>
inline T &SharedPtr<T>::operator*() const {
	if (m_data == NULL) {
		throw NullPointerException();
	}
	return *m_data;
}
template <typename T>
inline T *SharedPtr<T>::operator->() {
	if (m_data == NULL) {
		throw NullPointerException();
	}
	return m_data;
}
template <typename T>
inline T *SharedPtr<T>::operator->() const {
	if (m_data == NULL) {
		throw NullPointerException();
	}
	return m_data;
}

}

#endif // JSONBUS_POINTER_H
