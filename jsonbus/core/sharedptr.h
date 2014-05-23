/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@openihs.org>
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

#include <jsonbus/core/shareddata.h>

namespace JSONBus {

/**
 * @brief JSONBus : Shared pointer
 * 
 * @author <a href="mailto:emericv@openihs.org">Emeric Verschuur</a>
 * @date 2014
 * @copyright Apache License, Version 2.0
 */
template <typename T> class SharedPtr {
	T *m_data;
public:
	/**
	 * @brief Default shared pointer constructor
	 */
	SharedPtr();
	
	/**
	 * @brief Shared pointer constructor from a data address
	 * 
	 * @param data address to a data
	 */
	SharedPtr(T *data);
	
	/**
	 * @brief Shared pointer constructor from a equivalent type pointer
	 * 
	 * @param data other pointer
	 */
	SharedPtr(const SharedPtr<T> &other);
	
	/**
	 * @brief Shared pointer constructor from a other type pointer
	 * 
	 * @param other other pointer
	 */
	template<class X>
	SharedPtr(const SharedPtr<X> &other);
	
	/**
	 * @brief Shared pointer destructor
	 */
	~SharedPtr();
	
	/**
	 * @brief Get the internal data address
	 * 
	 * @return the internal data address
	 */
	T *data();
	
	/**
	 * @brief Get the internal data address
	 * 
	 * @return the internal data address
	 */
	const T *data() const;
	
	/**
	 * @brief Update the internal pointer to a given data address
	 * 
	 * @param data data address
	 * @return a reference to the pointer
	 */
	SharedPtr<T> &operator= (const T *data);
	
	/**
	 * @brief Affectation operator
	 * 
	 * @param other pointer
	 * @return a reference to the pointer
	 */
	SharedPtr<T> &operator= (const SharedPtr<T>& other);
	
	/**
	 * @brief Affectation operator
	 * 
	 * @param other pointer
	 * @return a reference to the pointer
	 */
	template<class X>
	SharedPtr<T> &operator= (const SharedPtr<X>& other);
	
	/**
	 * @brief Dereference pointer operator
	 * 
	 * @return a reference to the pointed data
	 */
	T &operator*() const;
	
	/**
	 * @brief Dereference pointer operator
	 * 
	 * @return a reference to the pointed data
	 */
	T *operator->();
	
	
	T *operator->() const;
	
	/**
	 * @brief Equality test operator between this pointer and a data address
	 * 
	 * @return true if the internal pointer and a given data address are equal, otherwise false
	 */
	bool operator== (const void *data) const;
	
	/**
	 * @brief Equality test operator between this pointer and a an other given pointer
	 * 
	 * @return true if this pointer and an other given pointer have the same data address, otherwise false
	 */
	bool operator== (const SharedPtr<T>& other) const;
	
	/**
	 * @brief Equality test operator between this pointer and a an other given pointer
	 * 
	 * @return true if this pointer and an other given pointer have the same data address, otherwise false
	 */
	template<class X>
	bool operator== (const SharedPtr<X>& other) const;
	
	/**
	 * @brief Difference test operator between this pointer and a data address
	 * 
	 * @return true if the internal pointer and a given data address are different, otherwise false
	 */
	bool operator!= (const void *data) const;
	
	/**
	 * @brief Difference test operator between this pointer and a an other given pointer
	 * 
	 * @return true if this pointer and an other given pointer don't have the same data address, otherwise false
	 */
	bool operator!= (const SharedPtr<T>& other) const;
	
	/**
	 * @brief Difference test operator between this pointer and a an other given pointer
	 * 
	 * @return true if this pointer and an other given pointer don't have the same data address, otherwise false
	 */
	template<class X>
	bool operator!= (const SharedPtr<X>& other) const;
};

/// @brief Generic pointer type
typedef SharedPtr<SharedData> GenericPtr;

extern void __raise_InvalidClassException();
extern void __raise_NullPointerException();
void __log_data_ref_init(void* data);
void __log_data_ref_delete(void* data);

#define __JSONBUS_SHAREDPTR_DEBUG_NEW() if (m_data->ref == 0) __log_data_ref_init(m_data)
#define __JSONBUS_SHAREDPTR_DEBUG_DEL() __log_data_ref_delete(m_data)

template <typename T>
inline SharedPtr<T>::SharedPtr(): m_data(nullptr) {}
template <typename T>
inline SharedPtr<T>::~SharedPtr() {
	if (m_data != nullptr) {
		if (m_data->ref.fetch_sub(1) == 1) {
			__JSONBUS_SHAREDPTR_DEBUG_DEL();
			delete m_data;
		}
	}
}
template <typename T>
inline SharedPtr<T>::SharedPtr(T* data): m_data(data) {
	if (m_data != nullptr) {
		if (data != nullptr && dynamic_cast<const T*>(data) == nullptr) {
			__raise_InvalidClassException();
		}
		__JSONBUS_SHAREDPTR_DEBUG_NEW();
		m_data->ref++;
	}
}
template <typename T>
inline SharedPtr<T>::SharedPtr(const SharedPtr< T >& other): m_data(other.m_data) {
	if (m_data != nullptr) {
		if (other.data() != nullptr && dynamic_cast<const T*>(other.data()) == nullptr) {
			__raise_InvalidClassException();
		}
		__JSONBUS_SHAREDPTR_DEBUG_NEW();
		m_data->ref++;
	}
}
template <typename T>
template <typename X>
inline SharedPtr<T>::SharedPtr(const SharedPtr< X >& other): m_data((T*)(other.data())) {
	if (m_data != nullptr) {
		if (other.data() != nullptr && dynamic_cast<const T*>(other.data()) == nullptr) {
			__raise_InvalidClassException();
		}
		__JSONBUS_SHAREDPTR_DEBUG_NEW();
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
inline SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) {
	return operator=(other.m_data);
}
template <typename T>
template <typename X>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<X>& other) {
	if (m_data == (T*)(other.data())) {
		return *this;
	} else if (m_data != nullptr) {
		if (m_data->ref.fetch_sub(1) == 1) {
			__JSONBUS_SHAREDPTR_DEBUG_DEL();
			delete m_data;
		}
	}
	if (other.data() != nullptr && dynamic_cast<const T*>(other.data()) == nullptr) {
		__raise_InvalidClassException();
	}
	m_data = (T*)(other.data());
	if (m_data != nullptr) {
		__JSONBUS_SHAREDPTR_DEBUG_NEW();
		m_data->ref++;
	}
	return *this;
}
template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const T *data) {
	if (m_data == data) {
		return *this;
	} else if (m_data != nullptr) {
		if (m_data->ref.fetch_sub(1) == 1) {
			__JSONBUS_SHAREDPTR_DEBUG_DEL();
			delete m_data;
		}
	}
	if (data != nullptr && dynamic_cast<const T*>(data) == nullptr) {
		__raise_InvalidClassException();
	}
	m_data = const_cast<T*>(data);
	if (m_data != nullptr) {
		__JSONBUS_SHAREDPTR_DEBUG_NEW();
		m_data->ref++;
	}
	return *this;
}
template <typename T>
inline bool SharedPtr<T>::operator==(const SharedPtr<T>& other) const {
	return m_data == other.m_data;
}
template <typename T>
template <typename X>
inline bool SharedPtr<T>::operator==(const SharedPtr<X>& other) const {
	return m_data == other.data();
}
template <typename T>
inline bool SharedPtr<T>::operator==(const void *data) const {
	return m_data == data;
}
template <typename T>
inline bool SharedPtr<T>::operator!=(const SharedPtr<T>& other) const {
	return m_data != other.m_data;
}
template <typename T>
template <typename X>
inline bool SharedPtr<T>::operator!=(const SharedPtr<X>& other) const {
	return m_data != other.data();
}
template <typename T>
inline bool SharedPtr<T>::operator!=(const void *data) const {
	return m_data != data;
}
template <typename T>
inline T &SharedPtr<T>::operator*() const {
	if (m_data == nullptr) {
		__raise_NullPointerException();
	}
	return *m_data;
}
template <typename T>
inline T *SharedPtr<T>::operator->() {
	if (m_data == nullptr) {
		__raise_NullPointerException();
	}
	return m_data;
}
template <typename T>
inline T *SharedPtr<T>::operator->() const {
	if (m_data == nullptr) {
		__raise_NullPointerException();
	}
	return m_data;
}

}

#endif // JSONBUS_POINTER_H
