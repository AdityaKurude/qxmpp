/*
 * Copyright (C) 2010 Bolloré telecom
 *
 * Author:
 *	Jeremy Lainé
 *
 * Source:
 *	http://code.google.com/p/qxmpp
 *
 * This file is a part of QXmpp library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 */

#ifndef QXMPPSTUN_H
#define QXMPPSTUN_H

#include <QObject>

#include "QXmppLogger.h"
#include "QXmppJingleIq.h"

class QUdpSocket;
class QTimer;
class QXmppStunMessage;

/// \brief The QXmppStunSocket class represents an UDP socket capable
/// of performing Interactive Connectivity Establishment (RFC 5245).
///

class QXmppStunSocket : public QObject
{
    Q_OBJECT

public:
    QXmppStunSocket(bool iceControlling, QObject *parent=0);
    ~QXmppStunSocket();

    QList<QXmppJingleCandidate> localCandidates() const;
    QString localUser() const;
    void setLocalUser(const QString &user);
    QString localPassword() const;
    void setLocalPassword(const QString &password);

    int component() const;
    void setComponent(int component);

    bool addRemoteCandidate(const QXmppJingleCandidate &candidate);
    void setRemoteUser(const QString &user);
    void setRemotePassword(const QString &password);

    void close();
    void connectToHost();
    QIODevice::OpenMode openMode() const;
    qint64 writeDatagram(const QByteArray &datagram);

private slots:
    void checkCandidates();
    void readyRead();

signals:
    // This signal is emitted when a data packet is received.
    void datagramReceived(const QByteArray &datagram, const QHostAddress &host, quint16 port);

    /// This signal is emitted to send logging messages.
    void logMessage(QXmppLogger::MessageType type, const QString &msg);

    void ready();

private:
    class Pair {
    public:
        Pair();
        QString toString() const;

        QIODevice::OpenMode checked;
        quint32 priority;
        QXmppJingleCandidate remote;
        QXmppJingleCandidate reflexive;
        QByteArray transaction;
    };

    Pair *addRemoteCandidate(const QHostAddress &host, quint16 port);
    void debug(const QString &message, QXmppLogger::MessageType = QXmppLogger::DebugMessage);
    qint64 writeStun(const QXmppStunMessage &message, QXmppStunSocket::Pair *pair);

    int m_component;

    QString m_localUser;
    QString m_localPassword;

    Pair *m_activePair;
    bool m_iceControlling;
    QList<Pair*> m_pairs;
    QString m_remoteUser;
    QString m_remotePassword;

    QUdpSocket *m_socket;
    QTimer *m_timer;
};

#endif
