#ifndef MESSAGELOG_H
#define MESSAGELOG_H

#include <QtCore/qglobal.h>
#include <QtSql/QSqlDatabase>

QT_BEGIN_NAMESPACE
class QString;
class QDateTime;
class QFile;
QT_END_NAMESPACE

class MessageLog {
public:
	MessageLog();
	bool open(const QString &);
	void close();
	bool append_message(const QDateTime &, const QString &, const QString &, quint8, const QByteArray &);

private:
	QSqlDatabase database;
	QFile *lock_file;
};

#endif // MESSAGELOG_H
