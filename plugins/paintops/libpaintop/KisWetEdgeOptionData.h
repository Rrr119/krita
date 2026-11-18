/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */
#ifndef KISWETEDGEOPTIONDATA_H
#define KISWETEDGEOPTIONDATA_H

#include "KisCurveOptionData.h"

#include <KisOptionTuple.h>
#include <KisPrefixedOptionDataWrapper.h>


struct PAINTOP_EXPORT KisWetEdgeOptionMixInImpl
    : boost::equality_comparable<KisWetEdgeOptionMixInImpl>
{
    inline friend bool operator==(const KisWetEdgeOptionMixInImpl &lhs, const KisWetEdgeOptionMixInImpl &rhs) {
            return lhs.edgeWidth == rhs.edgeWidth;
    }

    /// width of the darkened edge band, in percents of the dab radius
    int edgeWidth {30};

    bool read(const KisPropertiesConfiguration *setting);
    void write(KisPropertiesConfiguration *setting) const;
};

using KisWetEdgeOptionMixIn = KisPrefixedOptionDataWrapper<KisWetEdgeOptionMixInImpl>;

struct PAINTOP_EXPORT KisWetEdgeOptionData : KisOptionTuple<KisCurveOptionData, KisWetEdgeOptionMixIn>
{
    KisWetEdgeOptionData(const QString &prefix = "");
};

#endif // KISWETEDGEOPTIONDATA_H
