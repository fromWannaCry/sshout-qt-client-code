#ifndef SERVERINFORMATION_H
#define SERVERINFORMATION_H

#include <QtCore/qglobal.h>
#include <QtCore/QDataStream>
#include <QtCore/QMetaType>
#include <QtCore/QString>
//class QString;

struct ServerInformation {
	QString host;
	quint16 port;
	QString identify_file;
};
Q_DECLARE_METATYPE(ServerInformation)


static inline QDataStream &operator<<(QDataStream &out, const ServerInformation &info) {
	return out << info.host << info.port << info.identify_file;
}

static inline QDataStream &operator>>(QDataStream &in, ServerInformation &info) {
	in >> info.host;
	in >> info.port;
	in >> info.identify_file;
	return in;
}

static inline bool operator==(const ServerInformation &v1, const ServerInformation &v2) {
	return v1.host == v2.host && v1.port == v2.port && v1.identify_file == v2.identify_file;
}

#endif // SERVERINFORMATION_H
