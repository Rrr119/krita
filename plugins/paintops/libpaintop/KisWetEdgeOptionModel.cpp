/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "KisWetEdgeOptionModel.h"

KisWetEdgeOptionModel::KisWetEdgeOptionModel(lager::cursor<KisWetEdgeOptionMixIn> optionData)
    : wetEdgeOptionData(optionData)
    , LAGER_QT(edgeWidth) {wetEdgeOptionData[&KisWetEdgeOptionMixIn::edgeWidth]}
{
}
