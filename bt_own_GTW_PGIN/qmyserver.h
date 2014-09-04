/* This file is part of My Home System Emulator
 *
 * My Home System Emulator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.

 * My Home System Emulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with My Home System Emulator.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef QMYSERVER_H
#define QMYSERVER_H

#include <QTcpServer>
#include <QSslSocket>
#include <QList>

class QMyServer : public QTcpServer {

    Q_OBJECT

public:
    QSslSocket *socket;
    bool setKeyFile(QString keyFile);
    bool setCertFile(QString certFile);

protected:

    void incomingConnection (int socketDescriptor);


private:

    //   QSslSocket *socket;

    void _startServerEncryption ();
    void _connectSocketSignals ();

private slots:

    void slot_encrypted ();
    void slot_encryptedBytesWritten (qint64 written);
    void slot_modeChanged (QSslSocket::SslMode mode);
    void slot_peerVerifyError (const QSslError &error);
    void slot_sslErrors (const QList<QSslError> &errors);

    void slot_connected ();
    void slot_disconnected ();
    void slot_error (QAbstractSocket::SocketError);
    void slot_hostFound ();
    void slot_proxyAuthenticationRequired (const QNetworkProxy &, QAuthenticator *);
    void slot_stateChanged (QAbstractSocket::SocketState);

    void slot_readyRead ();

};

#endif // QMYSERVER_H
