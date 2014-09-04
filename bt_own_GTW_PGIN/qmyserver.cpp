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

#include "qmyserver.h"
#include <QByteArray>
#include <QSslKey>
#include <QFile>

//#define LOCALCERTIFICATE_FILE "cert.pem"
//#define PRIVATEKEY_FILE "cert.key"

void QMyServer::incomingConnection (int socketDescriptor)
{
    qDebug("QMyServer::incomingConnection(%d)", socketDescriptor);

    socket = new QSslSocket;

    if (!socket) {
        qWarning("not enough memory to create new QSslSocket");
        return;
    }
    socket->setProtocol(QSsl::AnyProtocol);

    _connectSocketSignals();

    if (!socket->setSocketDescriptor(socketDescriptor)) {
        qWarning("couldn't set socket descriptor");
        delete socket;
        return;
    }
    _startServerEncryption();
}

void QMyServer::_startServerEncryption ()
{
    if (QSslSocket::supportsSsl())
        qDebug()<< "Supporto SSL attivo....";
    else
        qDebug()<< "Supporto SSL non attivo.... Controlla l'include della libreria libssl .... ";

    QFile cert(":/files/resources/mycertcert.pem");
    if (!cert.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QByteArray certba = cert.readAll();

    QFile keyfile(":/files/resources/mycertkey.pem");
    if (!keyfile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QByteArray keyba = keyfile.readAll();

    QSslKey keyKey(keyba, QSsl::Rsa);

    if (keyKey.isNull()) {
        qWarning("Key is null");
        socket->disconnectFromHost();
        return;
    }

    socket->setLocalCertificate( QSslCertificate( certba ) );
    socket->setPrivateKey(keyKey);
    socket->startServerEncryption();

}

void QMyServer::_connectSocketSignals ()
{
    connect(socket, SIGNAL(encrypted()), this, SLOT(slot_encrypted()));
    connect(socket, SIGNAL(encryptedBytesWritten(qint64)),
            this, SLOT(slot_encryptedBytesWritten(qint64)));
    connect(socket, SIGNAL(modeChanged(QSslSocket::SslMode)),
            this, SLOT(slot_modeChanged(QSslSocket::SslMode)));
    connect(socket, SIGNAL(peerVerifyError(const QSslError &)),
            this, SLOT(slot_peerVerifyError (const QSslError &)));
    connect(socket, SIGNAL(sslErrors(const QList<QSslError> &)),
            this, SLOT(slot_sslErrors(const QList<QSslError> &)));
    connect(socket, SIGNAL(readyRead()),
            this, SLOT(slot_readyRead()));
    connect(socket, SIGNAL(connected()),
            this, SLOT(slot_connected()));
    connect(socket, SIGNAL(disconnected()),
            this, SLOT(slot_disconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slot_error(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(hostFound()),
            this, SLOT(slot_hostFound()));
    connect(socket, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)),
            this, SLOT(slot_proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(slot_stateChanged(QAbstractSocket::SocketState)));
}

bool QMyServer::setKeyFile(QString /*keyFile*/)
{
    return true;
}

bool QMyServer::setCertFile(QString /*certFile*/)
{
    return true;
}

void QMyServer::slot_encrypted ()
{
    qDebug("QMyServer::slot_encrypted");
}

void QMyServer::slot_encryptedBytesWritten (qint64 written)
{
    qDebug("QMyServer::slot_encryptedBytesWritten(%ld)", (long) written);
}

void QMyServer::slot_modeChanged (QSslSocket::SslMode mode)
{
    qDebug("QMyServer::slot_modeChanged(%d)", mode);
}

void QMyServer::slot_peerVerifyError (const QSslError &)
{
    qDebug("QMyServer::slot_peerVerifyError");
}

void QMyServer::slot_sslErrors (const QList<QSslError> &)
{
    qDebug("QMyServer::slot_sslErrors");
}

void QMyServer::slot_readyRead ()
{
    qDebug("QMyServer::slot_readyRead");
}

void QMyServer::slot_connected ()
{
    qDebug("QMyServer::slot_connected");
}

void QMyServer::slot_disconnected ()
{
    qDebug("QMyServer::slot_disconnected");
}

void QMyServer::slot_error (QAbstractSocket::SocketError err)
{
    qDebug() << "QMyServer::slot_error(" << err << ")";
    qDebug() << socket->errorString();
}

void QMyServer::slot_hostFound ()
{
    qDebug("QMyServer::slot_hostFound");
}

void QMyServer::slot_proxyAuthenticationRequired (const QNetworkProxy &, QAuthenticator *)
{
    qDebug("QMyServer::slot_proxyAuthenticationRequired");
}

void QMyServer::slot_stateChanged (QAbstractSocket::SocketState state)
{
    qDebug() << "QMyServer::slot_stateChanged(" << state << ")";
}
