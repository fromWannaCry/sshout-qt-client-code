#ifndef PACKET_H
#define PACKET_H

class QByteArray;
class QIODevice;

extern QByteArray sshout_get_packet(QIODevice *d);

#endif // PACKET_H
