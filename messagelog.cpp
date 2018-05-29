#include "messagelog.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtCore/QDateTime>
#include <QtCore/QVariant>
#include <QtCore/QFile>
#include <QtCore/QCoreApplication>
#include <signal.h>

MessageLog::MessageLog()  {
	//database = new QSqlDatabase;
	//QSQLiteDriver *driver = new QSQLiteDriver;
	//database = QSqlDatabase::addDatabase(driver);
	database = QSqlDatabase::addDatabase("QSQLITE");
}

bool MessageLog::open(const QString &path) {
	if(!database.isValid()) return false;
	database.setDatabaseName(path);
	//qDebug("is open ? %d", database.isOpen());
	if(database.isOpen()) return false;
	QByteArray lock_path_ba = path.toLocal8Bit();
	lock_file = new QFile(QString("%1.lock").arg(path));
	if(lock_file->exists()) {
		if(!lock_file->open(QFile::ReadOnly)) {
			qWarning("MessageLog::open: lock file '%s' exist but can't open", lock_path_ba.data());
			return false;
		}
		QByteArray content = lock_file->readLine();
		lock_file->close();
		if(!content.isEmpty()) {
			int pid = content.toInt();
			if(pid && kill(pid, 0) == 0) {
				qWarning("MessageLog::open: database file is locked by process %d via lock file '%s'", pid, lock_path_ba.data());
				return false;
			}
		}
		//lock_file->remove();
	}
	if(!lock_file->open(QFile::WriteOnly | QFile::Truncate)) {
		qWarning("MessageLog::open: cannot open lock file '%s'", lock_path_ba.data());
		return false;
	}
	if(lock_file->write(QByteArray::number(QCoreApplication::applicationPid())) < 0) {
		QByteArray error_msg = lock_file->errorString().toLocal8Bit();
		qWarning("MessageLog::open: cannot write to lock file '%s', %s", lock_path_ba.data(), error_msg.data());
		lock_file->close();
		return false;
	}
	lock_file->close();
	if(!database.open()) return false;
	QString sql_create_table("CREATE TABLE IF NOT EXISTS messages ("
				 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
				 "receive_time DATETIME,"
				 "time DATETIME,"
				 "from_user CHAR(32),"
				 "to_user CHAR(32),"
				 "type INT8,"
				 "message TEXT );");
	if(!QSqlQuery(sql_create_table, database).exec()) {
		QByteArray ba = path.toLocal8Bit();
		qWarning("MessageLog::open: failed to run create table SQL in database %s, giving up open", ba.data());
		database.close();
		return false;
	}
	return true;
}

void MessageLog::close() {
	if(!database.isOpen()) return;
	database.close();
	lock_file->remove();
}

bool MessageLog::append_message(const QDateTime &dt, const QString &from_user, const QString &to_user, quint8 type, const QByteArray &message) {
	QSqlQuery query(database);
	query.prepare("INSERT INTO messages (receive_time,time,from_user,to_user,type,message) VALUES (?,?,?,?,?,?);");
	query.bindValue(0, QDateTime::currentDateTime());
	query.bindValue(1, dt);
	query.bindValue(2, from_user);
	query.bindValue(3, to_user);
	query.bindValue(4, type);
	query.bindValue(5, message);
	return query.exec();
}
