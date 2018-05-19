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
	virtual void set_identify_file(const QString &) = 0;
	virtual void set_reconnect_interval(int) = 0;

signals:
	void state_change(SSHState);
};

#endif // SSHCLIENT_H
