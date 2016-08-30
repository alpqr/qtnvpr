/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtNVPR module
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QQUICKPATHITEM_P_H
#define QQUICKPATHITEM_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of a number of Qt sources files.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <QtNVPR/qtnvprglobal.h>
#include <QQuickItem>

QT_BEGIN_NAMESPACE

class QQuickPathItemPrivate;

class QNVPR_EXPORT QQuickPathItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool fillEnabled READ fillEnabled WRITE setFillEnabled NOTIFY fillEnabledChanged)
    Q_PROPERTY(QVariant fillMaterial READ fillMaterial WRITE setFillMaterial NOTIFY fillMaterialChanged)
    Q_PROPERTY(FillRule fillRule READ fillRule WRITE setFillRule NOTIFY fillRuleChanged)
    Q_PROPERTY(QVariant strokeMaterial READ strokeMaterial WRITE setStrokeMaterial NOTIFY strokeMaterialChanged)
    Q_PROPERTY(qreal strokeWidth READ strokeWidth WRITE setStrokeWidth NOTIFY strokeWidthChanged)

public:
    enum FillRule {
        OddEvenFill = Qt::OddEvenFill,
        WindingFill = Qt::WindingFill
    };
    Q_ENUM(FillRule)

    QQuickPathItem(QQuickItem *parent = nullptr);
    ~QQuickPathItem();

    Q_INVOKABLE void clear();
    Q_INVOKABLE bool isEmpty() const;
    Q_INVOKABLE void closeSubPath();

    Q_INVOKABLE void moveTo(qreal x, qreal y);
    Q_INVOKABLE void lineTo(qreal x, qreal y);
    Q_INVOKABLE void arcMoveTo(qreal x, qreal y, qreal w, qreal h, qreal angle);
    Q_INVOKABLE void arcTo(qreal x, qreal y, qreal w, qreal h, qreal startAngle, qreal arcLength);
    Q_INVOKABLE void cubicTo(qreal c1x, qreal c1y, qreal c2x, qreal c2y, qreal endX, qreal endY);
    Q_INVOKABLE void quadTo(qreal cX, qreal cY, qreal endX, qreal endY);

    Q_INVOKABLE void addRect(qreal x, qreal y, qreal w, qreal h);
    Q_INVOKABLE void addRoundedRect(qreal x, qreal y, qreal w, qreal h, qreal xr, qreal yr);
    Q_INVOKABLE void addEllipse(qreal x, qreal y, qreal rx, qreal ry);
    Q_INVOKABLE void addEllipseWithCenter(qreal cx, qreal cy, qreal rx, qreal ry);

    Q_INVOKABLE QPointF currentPosition() const;
    Q_INVOKABLE QRectF boundingRect() const;
    Q_INVOKABLE QRectF controlPointRect() const;

    bool fillEnabled() const;
    void setFillEnabled(bool enable);

    QVariant fillMaterial() const;
    void setFillMaterial(const QVariant &material);

    FillRule fillRule() const;
    void setFillRule(FillRule fillRule);

    QVariant strokeMaterial() const;
    void setStrokeMaterial(const QVariant &material);

    qreal strokeWidth() const;
    void setStrokeWidth(qreal w);

protected:
    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

signals:
    void fillEnabledChanged();
    void fillMaterialChanged();
    void fillRuleChanged();
    void strokeMaterialChanged();
    void strokeWidthChanged();

private:
    QQuickPathItemPrivate *d;
};

QT_END_NAMESPACE

#endif