/*
  Copyright (c) 2011-2012 - Tőkés Attila

  This file is part of SmtpClient for Qt.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  See the LICENSE file for more details.
*/

#include "smtpclient.h"

#include <QFileInfo>
#include <QByteArray>
#include <QTimer>
#include <QEventLoop>
#include <QMetaEnum>

/* [1] Constructors and destructors */

SmtpClient::SmtpClient(const QString & host, int port, ConnectionType connectionType) :
    state(UnconnectedState),
    host(host),
    port(port),
    name("localhost"),
    isReadyConnected(false),
    isAuthenticated(false),
    isMailSent(false),
    isReset(false)
{
    setConnectionType(connectionType);

    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()),
            this, SLOT(socketReadyRead()));
}

SmtpClient::~SmtpClient() {}

/* [1] --- */


/* [2] Getters and Setters */
/**
 * @brief Returns the host name of the server.
 */
QString SmtpClient::getHost() const
{
    return this->host;
}

/**
 * @brief Return the port.
 */
int SmtpClient::getPort() const
{
    return this->port;
}

/**
 * @brief Returns the connection type used.
 */
SmtpClient::ConnectionType SmtpClient::getConnectionType() const
{
    return connectionType;
}

/**
 * @brief Returns the client's name.
 */
QString SmtpClient::getName() const
{
    return this->name;
}

/**
 * @brief Sets the client's name. This name is sent by the EHLO command.
 */
void SmtpClient::setName(const QString &name)
{
    this->name = name;
}

/**
 * @brief Returns the last response of the server.
 */
QString SmtpClient::getResponseText() const
{
    return responseText;
}

/**
 * @brief Returns the last response code recived by the client.
 */
int SmtpClient::getResponseCode() const
{
    return responseCode;
}

/**
 * @brief Return the socket used by the client. The type of the of the
 * connection is QTcpConnection in case of TcpConnection, and QSslSocket
 * for SslConnection and TlsConnection.
 */
QTcpSocket* SmtpClient::getSocket() {
    return socket;
}

/* [2] --- */


/* [3] Public methods */

void SmtpClient::connectToHost()
{
    if (state != UnconnectedState)
        return;

    changeState(ConnectingState);
}

void SmtpClient::login()
{
    if (!isReadyConnected || isAuthenticated)
        return;

    changeState(AuthenticatingState);
}

void SmtpClient::login(const QString &user, const QString &password, AuthMethod method)
{
    this->authInfo = AuthInfo(user, password, method);
    login();
}

void SmtpClient::sendMail(const MimeMessage & email)
{
    if (!isReadyConnected)
        return;

    isMailSent = false;

    this->email = &email;
    this->rcptType = 0;
    changeState(MailSendingState);
}

void SmtpClient::quit()
{
    changeState(_QUITTING_State);
}

void SmtpClient::reset()
{
    if (!isReadyConnected)
        return;

    isReset = false;

    changeState(ResetState);
}

bool SmtpClient::waitForReadyConnected(int msec) {
    if (state == UnconnectedState)
        return false;

    if (isReadyConnected)
        return true;

    waitForEvent(msec, SIGNAL(readyConnected()));

    return isReadyConnected;
}

bool SmtpClient::waitForAuthenticated(int msec) {
    if (!isReadyConnected)
        return false;

    if (isAuthenticated)
        return true;

    waitForEvent(msec, SIGNAL(authenticated()));

    return isAuthenticated;
}

bool SmtpClient::waitForMailSent(int msec) {
    if (!isReadyConnected)
        return false;

    if (isMailSent)
        return true;

    waitForEvent(msec, SIGNAL(mailSent()));

    return isMailSent;
}

bool SmtpClient::waitForReset(int msec)
{
    if (!isReadyConnected)
        return false;

    if (isReset)
        return true;

    waitForEvent(msec, SIGNAL(mailReset()));

    return isReset;
}

bool SmtpClient::waitForDisconnected(int msec)
{

    if (!isReadyConnected)
        return false;

    waitForEvent(msec, SIGNAL(disconnected()));

    return !isReadyConnected;
}

/* [3] --- */


/* [4] Protected methods */

void SmtpClient::setConnectionType(ConnectionType ct)
{
    this->connectionType = ct;

    switch (connectionType)
    {
    case TcpConnection:
        socket = new QTcpSocket(this);
        break;
    case SslConnection:
    case TlsConnection:
        socket = new QSslSocket(this);
        connect(socket, SIGNAL(encrypted()),
                this, SLOT(socketEncrypted()));
        break;
    }
}

void SmtpClient::changeState(SmtpClient::ClientState state) {
    this->state = state;

#ifdef QT_NO_DEBUG
    // Emit stateChanged signal only for non-internal states
    if (state <= DisconnectingState) {
        emit stateChanged(state);
    }
#else
    // emit all in debug mode
    qDebug() << "[SmtpClient] State:" << staticMetaObject.enumerator(staticMetaObject.indexOfEnumerator("ClientState")).valueToKey(state);
    emit stateChanged(state);
#endif

    switch (state)
    {
    case ConnectingState:
        switch (connectionType)
        {
        case TlsConnection:
        case TcpConnection:
            socket->connectToHost(host, port);
            break;
        case SslConnection:
            ((QSslSocket*) socket)->connectToHostEncrypted(host, port);
            break;
        }
        break;

    case AuthenticatingState:
        isAuthenticated = false;
        changeState(authInfo.authMethod == AuthPlain ? _AUTH_PLAIN_0 : _AUTH_LOGIN_0);
        break;

    case MailSendingState:
        isMailSent = false;
        changeState(_MAIL_0_FROM);
        break;

    case _QUITTING_State:
        sendMessage("QUIT");
        break;

    case DisconnectingState:

        // Server should disconnect after sending reply to QUIT command, but disconnecting here takes care of a non-compliantserver.
        socket->disconnectFromHost();
        isReadyConnected = false;
        break;

    case UnconnectedState:
        isReadyConnected = false;
        isAuthenticated = false;

        emit disconnected();
        break;

    case ResetState:
        sendMessage("RSET");
        break;

    case _EHLO_State:
        // Service ready. Send EHLO message and change the state
        sendMessage("EHLO " + name);
        break;

    case _READY_Connected:
        isReadyConnected = true;
        changeState(ReadyState);
        emit readyConnected();
        break;

    /* --- TLS --- */
    case _TLS_State:
        changeState(_TLS_0_STARTTLS);
        break;

    case _TLS_0_STARTTLS:
        // send a request to start TLS handshake
        sendMessage("STARTTLS");
        break;

    case _TLS_1_ENCRYPT:
        ((QSslSocket*) socket)->startClientEncryption();
        break;

    case _TLS_2_EHLO:
        // Send EHLO one more time
        sendMessage("EHLO " + name);
        break;

    case _READY_Encrypted:
        changeState(_READY_Connected);
        break;

    /* --- AUTH --- */
    case _AUTH_PLAIN_0:
        // Sending command: AUTH PLAIN base64('\0' + username + '\0' + password)
        sendMessage("AUTH PLAIN " + QByteArray().append((char) 0).append(authInfo.username.toUtf8())
                    .append((char) 0).append(authInfo.password.toUtf8()).toBase64());
        break;

    case _AUTH_LOGIN_0:
        sendMessage("AUTH LOGIN");
        break;

    case _AUTH_LOGIN_1_USER:
        // Send the username in base64
        sendMessage(QByteArray().append(authInfo.username.toUtf8()).toBase64());
        break;

    case _AUTH_LOGIN_2_PASS:
        // Send the password in base64
        sendMessage(QByteArray().append(authInfo.password.toUtf8()).toBase64());
        break;

    case _READY_Authenticated:
        isAuthenticated = true;
        authInfo = AuthInfo();
        changeState(ReadyState);
        emit authenticated();
        break;

    /* --- MAIL --- */
    case _MAIL_0_FROM:
        sendMessage("MAIL FROM:<" + email->getSender().getAddress() + ">");
        break;

    case _MAIL_1_RCPT_INIT:
        rcptType++;
        const QList<EmailAddress> *addressList;
        switch (rcptType)
        {
        case _TO:
            addressList = &email->getRecipients(MimeMessage::To);
            break;
        case _CC:
            addressList = &email->getRecipients(MimeMessage::Cc);
            break;
        case _BCC:
            addressList = &email->getRecipients(MimeMessage::Bcc);
            break;
        default:
            changeState(_MAIL_3_DATA);
            return;
        }
        addressIt = addressList->constBegin();
        addressItEnd = addressList->constEnd();
        changeState(_MAIL_2_RCPT);
        break;

    case _MAIL_2_RCPT:
        if (addressIt != addressItEnd) {
            sendMessage("RCPT TO:<" + addressIt->getAddress() + ">");
            addressIt++;
        } else {
            changeState(_MAIL_1_RCPT_INIT);
        }
        break;

    case _MAIL_3_DATA:
        sendMessage("DATA");
        break;

    case _MAIL_4_SEND_DATA:
        email->writeToDevice(*socket);

#ifndef QT_NO_DEBUG
        qDebug() << "[Socket] OUT:";
        qDebug() << email->toString();
#endif
        sendMessage("\r\n.");
        break;

    case _READY_MailSent:
        isMailSent = true;
        changeState(ReadyState);
        emit mailSent();
        break;

    default:
        ;
    }
}

void SmtpClient::processResponse() {

    switch (state)
    {
    case ConnectedState:
        // Just connected to the server. Wait for 220 (Service ready)
        if (responseCode != 220) {
            emitError(ServerError);
            return;
        }
        changeState(_EHLO_State);
        break;

    case ResetState:
        if (responseCode != 250) {
            emitError(ServerError);
            return;
        }
        emit mailReset();
        changeState(ReadyState);
        break;

    case _EHLO_State:
        // The response code needs to be 250.
        if (responseCode != 250) {
            emitError(ServerError);
            return;
        }

        changeState((connectionType != TlsConnection) ? _READY_Connected : _TLS_State);
        break;

    case _QUITTING_State:
        // The response code needs to be 221.
        if (responseCode != 221) {
            emitError(ClientError);
            return;
        }
        changeState(DisconnectingState);

        break;

    /* --- TLS --- */
    case _TLS_0_STARTTLS:
        // The response code needs to be 220.
        if (responseCode != 220) {
            emitError(ServerError);
            return;
        }
        changeState(_TLS_1_ENCRYPT);
        break;

    case _TLS_2_EHLO:
        // The response code needs to be 250.
        if (responseCode != 250) {
            emitError(ServerError);
            return;
        }
        changeState(_READY_Encrypted);
        break;

    /* --- AUTH --- */
    case _AUTH_PLAIN_0:
        // If the response is not 235 then the authentication was failed
        if (responseCode != 235) {
            emitError(AuthenticationError);
            return;
        }
        changeState(_READY_Authenticated);
        break;

    case _AUTH_LOGIN_0:
        if (responseCode != 334) {
            emitError(AuthenticationError);
            return;
        }
        changeState(_AUTH_LOGIN_1_USER);
        break;

    case _AUTH_LOGIN_1_USER:
        if (responseCode != 334) {
            emitError(AuthenticationError);
            return;
        }
        changeState(_AUTH_LOGIN_2_PASS);
        break;

    case _AUTH_LOGIN_2_PASS:
        if (responseCode != 235) {
            emitError(AuthenticationError);
            return;
        }
        changeState(_READY_Authenticated);
        break;

    /* --- MAIL --- */
    case _MAIL_0_FROM:
        if (responseCode != 250) {
            emitError(MailSendingError);
            return;
        }
        changeState(_MAIL_1_RCPT_INIT);
        break;

    case _MAIL_2_RCPT:
        if (responseCode != 250) {
            emitError(MailSendingError);
            return;
        }
        changeState(_MAIL_2_RCPT);
        break;

    case _MAIL_3_DATA:
        if (responseCode != 354) {
            emitError(MailSendingError);
            return;
        }
        changeState(_MAIL_4_SEND_DATA);
        break;

    case _MAIL_4_SEND_DATA:
        if (responseCode != 250) {
            emitError(MailSendingError);
            return;
        }
        changeState(_READY_MailSent);
        break;

    default:
        ;
    }
}

void SmtpClient::sendMessage(const QString &text)
{

#ifndef QT_NO_DEBUG
    qDebug() << "[Socket] OUT:" << text;
#endif

    socket->write(text.toUtf8() + "\r\n");
    socket->flush();
}

void SmtpClient::emitError(SmtpClient::SmtpError e)
{
    emit error(e);
}

void SmtpClient::waitForEvent(int msec, const char *successSignal)
{
    QEventLoop loop;
    QObject::connect(this, successSignal, &loop, SLOT(quit()));
    QObject::connect(this, SIGNAL(error(SmtpClient::SmtpError)), &loop, SLOT(quit()));

    QTimer timer;
    if(msec > 0)
    {
        timer.setSingleShot(true);
        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        timer.start(msec);
    }

    loop.exec();
}

/* [4] --- */


/* [5] Slots for the socket's signals */

void SmtpClient::socketStateChanged(QAbstractSocket::SocketState state) {

#ifndef QT_NO_DEBUG
    qDebug() << "[Socket] State:" << state;
#endif

    switch (state)
    {
    case QAbstractSocket::ConnectedState:
        changeState(ConnectedState);

        break;
    case QAbstractSocket::UnconnectedState:
        changeState(UnconnectedState);
        break;
    default:
        ;
    }
}

void SmtpClient::socketError(QAbstractSocket::SocketError socketError) {
#ifndef QT_NO_DEBUG
    qDebug() << "[Socket] ERROR:" << socketError;
#else
    Q_UNUSED(socketError);
#endif
    emit error(SocketError);
}

void SmtpClient::socketReadyRead()
{
    QString responseLine;

    if (!socket->isOpen()) {
        emitError(SocketError);
        return;
    }

    while (socket->canReadLine()) {
        // Save the server's response
        responseLine = socket->readLine();
        tempResponse += responseLine;

#ifndef QT_NO_DEBUG
        qDebug() << "[Socket] IN: " << responseLine;
#endif
    }



    // Is this the last line of the response
    if (responseLine.length() > 3 && responseLine[3] == ' ') {
        responseText = tempResponse;
        tempResponse = "";

        // Extract the respose code from the server's responce (first 3 digits)
        responseCode = responseLine.left(3).toInt();

        // Check for server error
        if (responseCode / 100 == 4) {
            emitError(ServerError);
            return;
        }

        // Check for client error
        if (responseCode / 100 == 5) {
            emitError(ClientError);
            return;
        }

        processResponse();
    }
}

void SmtpClient::socketEncrypted() {
    if (state == _TLS_1_ENCRYPT) {
        changeState(_TLS_2_EHLO);
    }
}

/* [5] --- */




