#ifndef NEWSOCKET_H
#define NEWSOCKET_H
/*
 * Создаем connectToHost
 *
 * Attempts to make a connection to hostName on the given port. The protocol parameter can be used to specify
 * which network protocol to use (eg. IPv4 or IPv6).
 * hostName may be an IP address in string form (e.g., "43.195.83.32"),
 * Finally, if a connection is established, QAbstractSocket enters ConnectedState and emits connected().
 *
 * 1 - establish connection
 *     connectToHost(), check SocketState, process the connected() signal
 * 2 - read/write data
 *     readAll, write
 *
 * void QAbstractSocket::connected()
 *
 */
#endif // NEWSOCKET_H
