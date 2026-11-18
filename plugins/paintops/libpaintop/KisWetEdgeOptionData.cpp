/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "KisWetEdgeOptionData.h"

const QString WET_EDGE_EDGE_WIDTH = "WetEdge/edgeWidth";


bool KisWetEdgeOptionMixInImpl::read(const KisPropertiesConfiguration *setting)
{
    edgeWidth = setting->getInt(WET_EDGE_EDGE_WIDTH, 30);

    return true;
}

void KisWetEdgeOptionMixInImpl::write(KisPropertiesConfiguration *setting) const
{
    setting->setProperty(WET_EDGE_EDGE_WIDTH, edgeWidth);
}

KisWetEdgeOptionData::KisWetEdgeOptionData(const QString &prefix)
    : KisOptionTuple<KisCurveOptionData, KisWetEdgeOptionMixIn>(prefix, KoID("WetEdge", i18n("Wet Edge")))
{
}
