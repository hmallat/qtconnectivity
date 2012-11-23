/***************************************************************************
**
** Copyright (C) 2012 Research In Motion
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qbluetoothserviceinfo.h"
#include "qbluetoothserviceinfo_p.h"

#include "qrfcommserver_p.h"
#include "qrfcommserver.h"

QTBLUETOOTH_BEGIN_NAMESPACE

bool QBluetoothServiceInfo::isRegistered() const
{
    Q_D(const QBluetoothServiceInfo);

    return d->registered;
}

bool QBluetoothServiceInfo::registerService() const
{
    Q_D(const QBluetoothServiceInfo);

    if (!this->isComplete())
        return false;

    return d->registerService();
}

bool QBluetoothServiceInfo::unregisterService() const
{
    Q_D(const QBluetoothServiceInfo);

    if (!d->registered)
        return false;

    return false;
}

QBluetoothServiceInfoPrivate::QBluetoothServiceInfoPrivate()
:  registered(false)
{
}

QBluetoothServiceInfoPrivate::~QBluetoothServiceInfoPrivate()
{
}

void QBluetoothServiceInfoPrivate::setRegisteredAttribute(quint16 attributeId, const QVariant &value) const
{
    Q_UNUSED(attributeId);
    Q_UNUSED(value);

    registerService();
}

void QBluetoothServiceInfoPrivate::removeRegisteredAttribute(quint16 attributeId) const
{
    Q_UNUSED(attributeId);
    registered = false;
}

bool QBluetoothServiceInfoPrivate::registerService() const
{
    Q_Q(const QBluetoothServiceInfo);
    ppsRegisterControl();
    if (registered)
        ppsSendControlMessage("deregister_server", 0x1101,  QBluetoothUuid(QStringLiteral("00000000-1111-2222-3334-444444444443")), QString(), 0);

    //m_uuid = uuid;
    qBBBluetoothDebug() << "deregistering server";
    ppsSendControlMessage("deregister_server", 0x1101, QBluetoothUuid(QStringLiteral("00000000-1111-2222-3334-444444444443")), QString(), 0);
    qBBBluetoothDebug() << "registering server";
    ppsSendControlMessage("register_server", 0x1101, QBluetoothUuid(QStringLiteral("00000000-1111-2222-3334-444444444443")), QString(), 0);

    registered = true;
    ppsUnregisterControl(0);
    return true;
}

QTBLUETOOTH_END_NAMESPACE