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

#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <QObject>
#include <QtNetwork/QSslSocket>

#include "mimemessage.h"


class SmtpClient : public QObject
{
    Q_OBJECT
public:

    /* [0] Enumerations */

    enum AuthMethod
    {
        AuthPlain,
        AuthLogin
    };

    enum SmtpError
    {
        ConnectionTimeoutError,
        ResponseTimeoutError,
        AuthenticationFailedError,
        ServerError,    // 4xx smtp error
        ClientError,    // 5xx smtp error
        SocketError
    };

    enum ConnectionType
    {
        TcpConnection,
        SslConnection,
        TlsConnection       // STARTTLS
    };

    enum ClientState {
        UnconnectedState = 0,
        ConnectingState = 1,
        ConnectedState = 2,
        ReadyState = 3,
        AuthenticatingState = 4,
        MailSendingState = 5,
        DisconnectingState = 6,

        /* Internal States */
        _ELHO_State = 50,
        _TLS_State = 51,

        /* Internal Substates */
        _TLS_0_START = 60,
        _TLS_1_ENCRYPT = 61,
        _TLS_2_ELHO = 62

    };

    /* [0] --- */


    /* [1] Constructors and Destructors */

    SmtpClient(const QString & host = "locahost", int port = 25, ConnectionType ct = TcpConnection);

    ~SmtpClient();

    /* [1] --- */


    /* [2] Getters and Setters */

    const QString& getHost() const;
    void setHost(QString &host);

    int getPort() const;
    void setPort(int port);

    const QString& getName() const;
    void setName(const QString &name);

    ConnectionType getConnectionType() const;
    void setConnectionType(ConnectionType ct);

    const QString & getUser() const;
    void setUser(const QString &host);

    const QString & getPassword() const;
    void setPassword(const QString &password);

    SmtpClient::AuthMethod getAuthMethod() const;
    void setAuthMethod(AuthMethod method);

    const QString & getResponseText() const;
    int getResponseCode() const;

    QTcpSocket* getSocket();


    /* [2] --- */


    /* [3] Public methods */

    bool connectToHost();

    bool login();
    bool login(const QString &user, const QString &password, AuthMethod method = AuthLogin);

    bool sendMail(MimeMessage& email);

    void quit();


    /* [3] --- */

protected:

    /* [4] Protected members */

    QTcpSocket *socket;
    ClientState state;
    bool syncMode;

    QString host;
    int port;
    ConnectionType connectionType;
    QString name;

    QString user;
    QString password;
    AuthMethod authMethod;

    int connectionTimeout;
    int responseTimeout;

    QString responseText;
    int responseCode;


    class ResponseTimeoutException {};

    /* [4] --- */


    /* [5] Protected methods */
    void changeState(ClientState state);

    void processResponse();

    void waitForResponse() throw (ResponseTimeoutException);

    void sendMessage(const QString &text);

    /* [5] --- */

protected slots:

    /* [6] Protected slots */

    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketError(QAbstractSocket::SocketError error);
    void socketReadyRead();

    /* [6] --- */


signals:

    /* [7] Signals */

    void smtpError(SmtpError e);
    void stateChanged(ClientState s);

    /* [7] --- */

};

#endif // SMTPCLIENT_H
