#ifndef PACKET_H
#define PACKET_H

class QByteArray;
class QIODevice;

enum SSHOUTGetPacketState {
	SSHOUT_GET_PACKET_SUCCESS,
	SSHOUT_GET_PACKET_READ_ERROR,
	SSHOUT_GET_PACKET_SHORT_READ,
	SSHOUT_GET_PACKET_TOO_SHORT,
	SSHOUT_GET_PACKET_TOO_LONG,
	SSHOUT_GET_PACKET_INCOMPLETE,
};
//extern QByteArray sshout_get_packet(QIODevice *d);
extern SSHOUTGetPacketState sshout_get_packet(QIODevice *, QByteArray *);

#endif // PACKET_H
