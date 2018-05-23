#ifndef MESSAGELOG_H
#define MESSAGELOG_H

#include <QtCore/qglobal.h>
#include <QtSql/QSqlDatabase>

class QString;
class QDateTime;

class MessageLog {
public:
	MessageLog();
	bool open(const QString &);
	void close();
	bool append_message(const QDateTime &, const QString &, const QString &, quint8, const QByteArray &);

private:
	QSqlDatabase database;
};

#endif // MESSAGELOG_H
