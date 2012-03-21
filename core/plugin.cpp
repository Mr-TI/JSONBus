
#include "common.h"
#include "plugin.h"

namespace JSONBus {

Plugin::Plugin() {
	if (m_instance) {
		throw new PluginException("Only one instance of plugin by process can be instantiated");
	}
	m_instance = this;
}

Plugin::~Plugin() {
}

}
