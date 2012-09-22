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

#include <iostream>
using namespace std;

/* [1] Constructors and destructors */

SmtpClient::SmtpClient(const QString & host, int port, ConnectionType connectionType) :
    state(UnconnectedState),
    name("localhost"),
    authMethod(AuthPlain),
    connectionTimeout(5000),
    responseTimeout(5000),
    isReadyConnected(false)
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
    this->email = &email;
    this->rcptType = 0;
    changeState(MailSendingState);

    return true;
}

void SmtpClient::quit()
{
    changeState(DisconnectingState);
}


bool SmtpClient::waitForReadyConnected(int msec) {
    QEventLoop loop;

    QTimer::singleShot(msec, &loop, SLOT(quit()));
    QObject::connect(this, SIGNAL(readyConnected()), &loop, SLOT(quit()));

    loop.exec();
    return isReadyConnected;
}

bool SmtpClient::waitForAuthenticated(int msec) {
    QEventLoop loop;

    QTimer::singleShot(msec, &loop, SLOT(quit()));
    QObject::connect(this, SIGNAL(authenticated()), &loop, SLOT(quit()));

    loop.exec();
    return isAuthenticated;
}

bool SmtpClient::waitForMailSent(int msec) {
    QEventLoop loop;

    QTimer::singleShot(msec, &loop, SLOT(quit()));
    QObject::connect(this, SIGNAL(mailSent()), &loop, SLOT(quit()));

    loop.exec();
    return isMailSent;
}

/* [3] --- */


/* [4] Protected methods */

void SmtpClient::changeState(ClientState state) {
    this->state = state;

#ifdef QT_NO_DEBUG
    // Emit stateChanged signal only for non-internal states
    if (state <= DisconnectingState) {
        emit stateChanged(state);
    }
#else
    // emit all in debug mode
    qDebug() << "State:" << state;
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
        changeState(authMethod == AuthPlain ? _AUTH_PLAIN_0 : _AUTH_LOGIN_0);
        break;

    case MailSendingState:
        isMailSent = false;
        changeState(_MAIL_0_FROM);
        break;

    case DisconnectingState:
        sendMessage("QUIT");
        socket->disconnectFromHost();
        break;

    case _EHLO_State:
        // Service ready. Send EHLO message and chage the state
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
        isAuthenticated = true;
        changeState(ReadyState);
        emit authenticated();
        break;

    /* --- MAIL --- */
    case _MAIL_0_FROM:
        sendMessage("MAIL FROM: <" + email->getSender().getAddress() + ">");
        break;

    case _MAIL_1_RCPT_INIT:
        rcptType++;
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
        changeState(_MAIL_2_RCPT);
        break;

    case _MAIL_2_RCPT:
        if (addressIt != addressList->end()) {
            sendMessage("RCPT TO: <" + (*addressIt)->getAddress() + ">");
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
            emit error(ServerError); return;
        }
        changeState(_EHLO_State);
        break;

    case _EHLO_State:
        // The response code needs to be 250.
        if (responseCode != 250) {
            emit error(ServerError); return;
        }

        changeState((connectionType != TlsConnection) ? _READY_Connected : _TLS_State);
        break;

    /* --- TLS --- */
    case _TLS_0_STARTTLS:
        // The response code needs to be 220.
        if (responseCode != 220) {
            emit error(ServerError);
            return;
        }
        changeState(_TLS_1_ENCRYPT);
        break;

    case _TLS_2_EHLO:
        // The response code needs to be 250.
        if (responseCode != 250) {
            emit error(ServerError);
            return;
        }
        changeState(_READY_Encrypted);
        break;

    /* --- AUTH --- */
    case _AUTH_PLAIN_0:
        // If the response is not 235 then the authentication was failed
        if (responseCode != 235) {
            emit error(AuthenticationError);
            return;
        }
        changeState(_READY_Authenticated);
        break;

    case _AUTH_LOGIN_0:
        if (responseCode != 334) {
            emit error(AuthenticationError);
            return;
        }
        changeState(_AUTH_LOGIN_1_USER);
        break;

    case _AUTH_LOGIN_1_USER:
        if (responseCode != 334) {
            emit error(AuthenticationError);
            return;
        }
        changeState(_AUTH_LOGIN_2_PASS);
        break;

    case _AUTH_LOGIN_2_PASS:
        if (responseCode != 235) {
            emit error(AuthenticationError);
            return;
        }
        changeState(_READY_Authenticated);
        break;

    /* --- MAIL --- */
    case _MAIL_0_FROM:
        if (responseCode != 250) {
            emit error(MailSendingError);
            return;
        }
        changeState(_MAIL_1_RCPT_INIT);
        break;

    case _MAIL_2_RCPT:
        if (responseCode != 250) {
            emit error(MailSendingError);
            return;
        }
        changeState(_MAIL_2_RCPT);
        break;

    case _MAIL_3_DATA:
        if (responseCode != 354) {
            emit error(MailSendingError);
            return;
        }
        changeState(_MAIL_4_SEND_DATA);
        break;

    case _MAIL_4_SEND_DATA:
        if (responseCode != 250) {
            emit error(MailSendingError);
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
#ifndef QT_NO_DEBUG
    qDebug() << "SocketError:" << socketError << socket->error();
#endif
    emit error(SocketError);
}

void SmtpClient::socketReadyRead()
{
    QString responseLine;

    while (socket->canReadLine()) {
        // Save the server's response
        responseLine = socket->readLine();
        tempResponse += responseLine;       
    }

    // Is this the last line of the response
    if (responseLine[3] == ' ') {
        responseText = tempResponse;
        tempResponse = "";

        // Extract the respose code from the server's responce (first 3 digits)
        responseCode = responseLine.left(3).toInt();

        // Check for server error
        if (responseCode / 100 == 4) {
            emit error(ServerError);
            return;
        }

        // Check for client error
        if (responseCode / 100 == 5) {
            emit error(ClientError);
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




