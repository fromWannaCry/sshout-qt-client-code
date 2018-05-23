#include "messagelog.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtCore/QVariant>

MessageLog::MessageLog()  {
	//database = new QSqlDatabase;
	//QSQLiteDriver *driver = new QSQLiteDriver;
	//database = QSqlDatabase::addDatabase(driver);
	database = QSqlDatabase::addDatabase("QSQLITE");
}

bool MessageLog::open(const QString &path) {
	if(!database.isValid()) return false;
	database.setDatabaseName(path);
	if(!database.open()) return false;
	QString sql_create_table("CREATE TABLE IF NOT EXISTS messages ("
				 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
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
	return database.close();
}

bool MessageLog::append_message(const QDateTime &dt, const QString &from_user, const QString &to_user, quint8 type, const QByteArray &message) {
	QSqlQuery query(database);
	query.prepare("INSERT INTO messages (time,from_user,to_user,type,message) VALUES (?,?,?,?,?);");
	query.bindValue(0, dt);
	query.bindValue(1, from_user);
	query.bindValue(2, to_user);
	query.bindValue(3, type);
	query.bindValue(4, message);
	return query.exec();
}
