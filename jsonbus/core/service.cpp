
#include "common.h"
#include "service.h"

namespace JSONBus {

Service::Service(const QString &prefix, const QString &name)
	: m_prefix(prefix), m_name(name) {
	if (m_prefix.isEmpty()) {
		throw new InvalidPrefixServiceException("Service prefix is empty");
	}
	if (m_prefix.isEmpty()) {
		throw new InvalidNameServiceException("Service name is empty");
	}
}

Service::~Service() {
}

}
