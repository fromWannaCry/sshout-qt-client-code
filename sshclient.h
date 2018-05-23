#ifndef SSHCLIENT_H
#define SSHCLIENT_H

#include <QtCore/QIODevice>

class SSHClient : public QIODevice {
	Q_OBJECT
public:
	enum SSHState {
		DISCONNECTED,
		CONNECTIING,
		AUTHENTICATING,
		AUTHENTICATED
	};

	SSHClient(QObject * = NULL);
	virtual bool connect(const QString &, quint16, const QString &, const QString & = QString()) = 0;
	virtual void disconnect() = 0;
	virtual void set_known_hosts(const QStringList &) = 0;
	virtual void set_identify_file(const QString &) = 0;
	virtual void set_reconnect_interval(int) = 0;
	virtual SSHState state() = 0;
	virtual bool is_connected() = 0;

signals:
	void state_changed(SSHClient::SSHState);
	void connected();
	void disconnected(int);
	//void readyRead();
};

#endif // SSHCLIENT_H
