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

#include <iostream>
using namespace std;

/* [1] Constructors and destructors */

SmtpClient::SmtpClient(const QString & host, int port, ConnectionType connectionType) :
    state(UnconnectedState),
    name("localhost"),
    authMethod(AuthPlain),
    connectionTimeout(5000),
    responseTimeout(5000)
{
    setConnectionType(connectionType);

    this->host = host;
    this->port = port;

    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()),
            this, SLOT(socketReadyRead()));
}

SmtpClient::~SmtpClient() {}

/* [1] --- */


/* [2] Getters and Setters */

void SmtpClient::setUser(const QString &user)
{
    this->user = user;
}

void SmtpClient::setPassword(const QString &password)
{
    this->password = password;
}

void SmtpClient::setAuthMethod(AuthMethod method)
{
    this->authMethod = method;
}

void SmtpClient::setHost(QString &host)
{
    this->host = host;
}

void SmtpClient::setPort(int port)
{
    this->port = port;
}

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
        //QSslSocket &s = *((QSslSocket*) socket);
        connect(socket, SIGNAL(encrypted()),
                this, SLOT(socketEncrypted()));
        break;
    }
}

const QString& SmtpClient::getHost() const
{
    return this->host;
}

const QString& SmtpClient::getUser() const
{
    return this->user;
}

const QString& SmtpClient::getPassword() const
{
    return this->password;
}

SmtpClient::AuthMethod SmtpClient::getAuthMethod() const
{
    return this->authMethod;
}

int SmtpClient::getPort() const
{
    return this->port;
}

SmtpClient::ConnectionType SmtpClient::getConnectionType() const
{
    return connectionType;
}

const QString& SmtpClient::getName() const
{
    return this->name;
}

void SmtpClient::setName(const QString &name)
{
    this->name = name;
}

const QString & SmtpClient::getResponseText() const
{
    return responseText;
}

int SmtpClient::getResponseCode() const
{
    return responseCode;
}

QTcpSocket* SmtpClient::getSocket() {
    return socket;
}

/* [2] --- */


/* [3] Public methods */

bool SmtpClient::connectToHost()
{
    changeState(ConnectingState);
    return true;
}

bool SmtpClient::login()
{
    changeState(AuthenticatingState);
    return true;
}

bool SmtpClient::login(const QString &user, const QString &password, AuthMethod method)
{
    this->user = user;
    this->password = password;
    this->authMethod = method;
    return login();
}

bool SmtpClient::sendMail(MimeMessage& email)
{
    try
    {
        // Send the MAIL command with the sender
        sendMessage("MAIL FROM: <" + email.getSender().getAddress() + ">");

        waitForResponse();

        if (responseCode != 250) return false;

        // Send RCPT command for each recipient
        QList<EmailAddress*>::const_iterator it, itEnd;
        // To (primary recipients)
        for (it = email.getRecipients().begin(), itEnd = email.getRecipients().end();
             it != itEnd; ++it)
        {
            sendMessage("RCPT TO: <" + (*it)->getAddress() + ">");
            waitForResponse();

            if (responseCode != 250) return false;
        }

        // Cc (carbon copy)
        for (it = email.getRecipients(MimeMessage::Cc).begin(), itEnd = email.getRecipients(MimeMessage::Cc).end();
             it != itEnd; ++it)
        {
            sendMessage("RCPT TO: <" + (*it)->getAddress() + ">");
            waitForResponse();

            if (responseCode != 250) return false;
        }

        // Bcc (blind carbon copy)
        for (it = email.getRecipients(MimeMessage::Bcc).begin(), itEnd = email.getRecipients(MimeMessage::Bcc).end();
             it != itEnd; ++it)
        {
            sendMessage("RCPT TO: <" + (*it)->getAddress() + ">");
            waitForResponse();

            if (responseCode != 250) return false;
        }

        // Send DATA command
        sendMessage("DATA");
        waitForResponse();

        if (responseCode != 354) return false;

        sendMessage(email.toString());

        // Send \r\n.\r\n to end the mail data
        sendMessage(".");

        waitForResponse();

        if (responseCode != 250) return false;
    }
    catch (ResponseTimeoutException)
    {
        return false;
    }

    return true;
}

void SmtpClient::quit()
{
    sendMessage("QUIT");
}

/* [3] --- */


/* [4] Protected methods */
void SmtpClient::waitForResponse() throw (ResponseTimeoutException)
{
    if (!socket->waitForReadyRead(responseTimeout))
    {
        emit smtpError(ResponseTimeoutError);
        throw ResponseTimeoutException();
    }

    processResponse();
}

void SmtpClient::changeState(ClientState state) {
    this->state = state;

#ifdef QT_NO_DEBUG
    // Emit stateChanged signal only for non-internal states
    if (state <= DisconnectingState) {
        emit stateChanged(state);
    }
#else
    // emit all in debug mode
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

    case _EHLO_State:
        // Service ready. Send EHLO message and chage the state
        sendMessage("EHLO " + name);
        break;

    case _READY_Connected:
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
    case AuthenticatingState:
        changeState(authMethod == AuthPlain ? _AUTH_PLAIN_0 : _AUTH_LOGIN_0);
        break;

    case _AUTH_PLAIN_0:
        // Sending command: AUTH PLAIN base64('\0' + username + '\0' + password)
        sendMessage("AUTH PLAIN " + QByteArray().append((char) 0).append(user)
                    .append((char) 0).append(password).toBase64());
        break;

    case _AUTH_LOGIN_0:
        sendMessage("AUTH LOGIN");
        break;

    case _AUTH_LOGIN_1_USER:
        // Send the username in base64
        sendMessage(QByteArray().append(user).toBase64());
        break;

    case _AUTH_LOGIN_2_PASS:
        // Send the password in base64
        sendMessage(QByteArray().append(password).toBase64());
        break;

    case _READY_Authenticated:
        changeState(ReadyState);
        emit authenticated();
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
            emit smtpError(ServerError); return;
        }
        changeState(_EHLO_State);
        break;

    case _EHLO_State:
        // The response code needs to be 250.
        if (responseCode != 250) {
            emit smtpError(ServerError); return;
        }

        changeState((connectionType != TlsConnection) ? _READY_Connected : _TLS_State);
        break;

    /* --- TLS --- */
    case _TLS_0_STARTTLS:
        // The response code needs to be 220.
        if (responseCode != 220) {
            emit smtpError(ServerError);
            return;
        }
        changeState(_TLS_1_ENCRYPT);
        break;

    case _TLS_2_EHLO:
        // The response code needs to be 250.
        if (responseCode != 250) {
            emit smtpError(ServerError);
            return;
        }
        changeState(_READY_Encrypted);
        break;

    /* --- AUTH --- */
    case _AUTH_PLAIN_0:
        // If the response is not 235 then the authentication was failed
        if (responseCode != 235) {
            emit smtpError(AuthenticationFailedError);
            return;
        }
        changeState(_READY_Authenticated);
        break;

    case _AUTH_LOGIN_0:
        if (responseCode != 334) {
            emit smtpError(AuthenticationFailedError);
            return;
        }
        changeState(_AUTH_LOGIN_1_USER);
        break;

    case _AUTH_LOGIN_1_USER:
        if (responseCode != 334) {
            emit smtpError(AuthenticationFailedError);
            return;
        }
        changeState(_AUTH_LOGIN_2_PASS);
        break;

    case _AUTH_LOGIN_2_PASS:
        if (responseCode != 235) {
            emit smtpError(AuthenticationFailedError);
            return;
        }
        changeState(_READY_Authenticated);
        break;

    default:
        ;
    }
}

void SmtpClient::sendMessage(const QString &text)
{
    socket->flush();
    socket->write(text.toUtf8() + "\r\n");
}

/* [4] --- */


/* [5] Slots for the socket's signals */

void SmtpClient::socketStateChanged(QAbstractSocket::SocketState state) {
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
    qDebug() << "this is sparta" << endl;
    emit smtpError(SocketError);
}

void SmtpClient::socketReadyRead()
{
    QString responseLine;

    while (socket->canReadLine()) {
        // Save the server's response
        responseLine = socket->readLine();
        tempResponse += responseLine;

        // Extract the respose code from the server's responce (first 3 digits)
        responseCode = responseLine.left(3).toInt();

        // Check for server error
        if (responseCode / 100 == 4) {
            emit smtpError(ServerError);
            return;
        }

        // Check for client error
        if (responseCode / 100 == 5) {
            emit smtpError(ClientError);
            return;
        }
    }

    // Is this the last line of the response
    if (responseLine[3] == ' ') {
        responseText = tempResponse;
        processResponse();

    }
}

void SmtpClient::socketEncrypted() {
    if (state == _TLS_1_ENCRYPT) {
        changeState(_TLS_2_EHLO);
    }
}

/* [5] --- */




