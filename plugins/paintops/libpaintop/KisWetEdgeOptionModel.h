/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */
#ifndef KISWETEDGEOPTIONMODEL_H
#define KISWETEDGEOPTIONMODEL_H

#include <lager/cursor.hpp>
#include <lager/extra/qt.hpp>

#include <QObject>

#include "KisWetEdgeOptionData.h"

class KisWetEdgeOptionModel : public QObject
{
    Q_OBJECT
public:
    KisWetEdgeOptionModel(lager::cursor<KisWetEdgeOptionMixIn> optionData);
    lager::cursor<KisWetEdgeOptionMixIn> wetEdgeOptionData;
    LAGER_QT_CURSOR(int, edgeWidth);
};

#endif // KISWETEDGEOPTIONMODEL_H
