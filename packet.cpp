/* Secure Shout Host Oriented Unified Talk
 * Copyright 2015-2018 Rivoreo
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 */

#include "sshout/api.h"
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>

QByteArray sshout_get_packet(QIODevice *d) {
	quint32 length;
	QByteArray buffer;
	QDataStream in(&buffer, QIODevice::ReadOnly);
	buffer = d->read(4);
	if(buffer.isEmpty()) return buffer;
	in >> length;
	buffer.clear();
	buffer = d->read(length);
	return buffer;
}
