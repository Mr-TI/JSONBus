/*
 *   Copyright 2012-2014 Emeric Verschuur <emericv@openihs.org>
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

#include <QStringList>
#include <nodebus/core/common.h>
#include <nodebus/core/cliarguments.h>
#include <nodebus/core/settings.h>
#include <nodebus/core/logger.h>
#include <nodebus/core/sslserversocketchannel.h>
#include <nodebus/core/ssliochannel.h>
#include <nodebus/core/sslcontext.h>
#include "proxy.h"
#include "stdpeer.h"
#include <nodebus/core/peeradmin.h>
#include "httppeer.h"
#include <openssl/pkcs12.h>

#define THROW_IOEXP_ON_ERR(msg, exp) \
	if ((exp) < 1) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + " - " msg ": " + SSLIOChannel::getLastError())
#define THROW_IOEXP_ON_NULL(msg, exp) \
	if ((exp) == nullptr) throw IOException(QString() + __FILE__ + ":" + QString::number(__LINE__) + " - " msg ": " + SSLIOChannel::getLastError())

nodebus_declare_master_service(Proxy)

class PKCS12SSLCtx : public SSLContext {
public:
	PKCS12SSLCtx(const QString &filename, const QString &passwd): m_ctx(NULL) {
		FILE *pks12file = NULL;
		try {
			m_ctx = SSL_CTX_new(SSLv23_method());
			THROW_IOEXP_ON_NULL("PKCS12 fopen", pks12file = fopen(filename.toStdString().c_str(), "rb"));
			PKCS12 *pkcs12;
			THROW_IOEXP_ON_NULL("d2i_PKCS12_fp", pkcs12 = d2i_PKCS12_fp(pks12file, 0));
			EVP_PKEY *pkey = nullptr;
			X509 *cert = nullptr;
			stack_st_X509 *ca = nullptr;
			
			THROW_IOEXP_ON_ERR("PKCS12_parse", PKCS12_parse(pkcs12, passwd.toStdString().c_str(), &pkey, &cert, &ca));
			fclose(pks12file);
			pks12file = NULL;
			if (pkey == nullptr) {
				throw IOException("Missing private key from PKCS12 keystore");
			}
			if (cert == nullptr) {
				throw IOException("Missing certificate from PKCS12 keystore");
			}
			if (ca == nullptr) {
				throw IOException("Missing root CA from PKCS12 keystore");
			}
			THROW_IOEXP_ON_ERR("SSL_CTX_use_certificate", SSL_CTX_use_certificate(m_ctx, cert));
			THROW_IOEXP_ON_ERR("SSL_CTX_use_PrivateKey", SSL_CTX_use_PrivateKey(m_ctx, pkey));
			THROW_IOEXP_ON_ERR("SSL_CTX_check_private_key", SSL_CTX_check_private_key(m_ctx));
			X509_STORE *store = SSL_CTX_get_cert_store(m_ctx);
			logFiner() << "PKCS12SSLCtx::CTX: CA number: " << sk_X509_num(ca);
			for (int i = 0; i < sk_X509_num(ca); i++) {
				THROW_IOEXP_ON_ERR("SSL_CTX_add_extra_chain_cert", SSL_CTX_add_extra_chain_cert(m_ctx, sk_X509_value(ca, i)));
				THROW_IOEXP_ON_ERR("X509_STORE_add_cert", X509_STORE_add_cert(store, sk_X509_value(ca, i)));
				THROW_IOEXP_ON_ERR("SSL_CTX_add_client_CA", SSL_CTX_add_client_CA(m_ctx, sk_X509_value(ca, i)));
				logFiner() << "PKCS12SSLCtx::CA name: " << sk_X509_value(ca, i)->name;
			}
			SSL_CTX_set_options(m_ctx, SSL_OP_ALL);
			SSL_CTX_set_verify(m_ctx, SSL_VERIFY_NONE | SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
			SSL_CTX_set_session_cache_mode(m_ctx, SSL_SESS_CACHE_BOTH);
			SSL_CTX_set_session_id_context(m_ctx, (const uchar*)"Proxy", 11);
			return;
		} catch (Exception &e) {
			if (m_ctx) {
				SSL_CTX_free(m_ctx);
			}
			if (pks12file) {
				fclose(pks12file);
			}
			throw e;
		}
	}
	
    virtual ~PKCS12SSLCtx() {
		SSL_CTX_free(m_ctx);
	}
	
	virtual SSL_CTX *getCTX() {
		return m_ctx;
	}
private:
	SSL_CTX *m_ctx;
};

Proxy::Proxy(int &argc, char **argv)
	: Application(argc, argv) {
}

Proxy::~Proxy() {
}

void Proxy::onInit() {
	CliArguments &args = CliArguments::getInstance();
// 	args.define("daemonize",		'd', tr("Launch this service in background"));
	args.define("edit-settings",	's', tr("Interactive settings edition"));
	args.define("config",			'c', tr("Set a custom config path"), "/etc/proxy/proxy.conf");
#ifdef WIN32
	args.define("win-cli",			'\0', tr("Switch to cli mode, don't launch a Windows service"));
#endif
}

void Proxy::onStart() {
	CliArguments &args = CliArguments::getInstance();
#ifdef WIN32
	m_settings = new Settings("Bouygues Telecom", "Proxy", QSettings::NativeFormat);
#else
	m_settings = new Settings(args.getValue("config").toString(), QSettings::NativeFormat);
#endif
	m_settings->define("pidfile",	tr("Path of the file where the service PID will be written in"),
					"/var/run/proxy.pid");
	m_settings->define("intf-main/listen-uri",	tr("Main interface - Listen addresses (comma separated URIs)"),
					"ssl://[::]:3693");
	m_settings->define("intf-main/keystore-path",	tr("Main interface - PKCS12 keystore path"),
					"/etc/proxy/keystore.p12");
	m_settings->define("intf-main/keystore-pwd",	tr("Main interface - PKCS12 keystore password"),
					"");
	m_settings->define("intf-console/listen-uri",	tr("Console interface - Listen addresses (comma separated URIs)"),
					"http://localhost:3695");
	m_settings->define("intf-console/keystore-path",	tr("Console interface - PKCS12 keystore path"),
					"");
	m_settings->define("intf-console/keystore-pwd",	tr("Console interface - PKCS12 keystore password"),
					"");
	if (args.isEnabled("edit-settings")) {
		m_settings->setup();
		throw ExitApplicationException();
	}
	
	SSL_load_error_strings();
	SSL_library_init();
	
	QStringList urls = m_settings->value("intf-main/listen-uri").toString().split(',');
	if (urls.isEmpty()) {
		throw ApplicationException("No URL given for listen addresse list on the main interface");
	}
	SSLContextPtr sslCtx;
	GenericPtr clientFactory = new StdPeer::Factory();
	for (auto it = urls.begin(); it != urls.end(); it++) {
		QUrl url(*it);
		ServerSocketChannelPtr server;
		if (url.scheme() == "ssl") {
			if (sslCtx == nullptr) {
				sslCtx = new PKCS12SSLCtx(m_settings->value("intf-main/keystore-path").toString(), 
							m_settings->value("intf-main/keystore-pwd").toString());
			}
			m_socketAdmin.attach(server = new SSLServerSocketChannel(url.host(), url.port(3693), sslCtx, 
															SSLServerSocketChannel::OPT_BACKLOG(5) | SSLServerSocketChannel::OPT_REUSEADDR), clientFactory);
		} else if (url.scheme() == "socket") {
			m_socketAdmin.attach(server = new ServerSocketChannel(url.host(), url.port(3693), 
															ServerSocketChannel::OPT_BACKLOG(5) | ServerSocketChannel::OPT_REUSEADDR), clientFactory);
		} else {
			throw ApplicationException("Unsupported sheme '" + url.scheme() + "' (possibles values are 'socket' or 'ssl')");
		}
		server->setKeepAlive(true);
		server->setKeepIntlv(60);
		server->setKeepIdle(600);
		server->setKeepCnt(5);
	}
	
	urls = m_settings->value("intf-console/listen-uri").toString().split(',');
	if (urls.isEmpty()) {
		throw ApplicationException("No URL given for listen addresse list on the console interface");
	}
	sslCtx = nullptr;
	clientFactory = new HttpPeer::Factory();
	for (auto it = urls.begin(); it != urls.end(); it++) {
		QUrl url(*it);
		if (url.scheme() == "https") {
			if (sslCtx == nullptr) {
				sslCtx = new PKCS12SSLCtx(m_settings->value("intf-console/keystore-path").toString(), 
							m_settings->value("intf-console/keystore-pwd").toString());
			}
			m_socketAdmin.attach(new SSLServerSocketChannel(url.host(), url.port(3695), sslCtx, 
															SSLServerSocketChannel::OPT_BACKLOG(5) | SSLServerSocketChannel::OPT_REUSEADDR), clientFactory);
		} else if (url.scheme() == "http") {
			m_socketAdmin.attach(new ServerSocketChannel(url.host(), url.port(3695), 
															ServerSocketChannel::OPT_BACKLOG(5) | ServerSocketChannel::OPT_REUSEADDR), clientFactory);
		} else {
			throw ApplicationException("Unsupported sheme '" + url.scheme() + "' (possibles values are 'http' or 'https')");
		}
	}

	connect(&m_socketAdmin, SIGNAL(terminated()), this, SLOT(quit()));
	connect(this, SIGNAL(aboutToQuit()), &m_socketAdmin, SLOT(cancel()));
	m_socketAdmin.start();
}
