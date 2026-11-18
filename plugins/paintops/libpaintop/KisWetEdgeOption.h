/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */
#ifndef KISWETEDGEOPTION_H
#define KISWETEDGEOPTION_H


#include <KisCurveOption.h>

struct KisWetEdgeOptionData;

class PAINTOP_EXPORT KisWetEdgeOption : public KisCurveOption
{
public:
    KisWetEdgeOption(const KisPropertiesConfiguration *setting);

    /**
     * Darken the rim of the dab to imitate the "wet edge" effect of
     * water-based media. The strength of the effect is controlled by
     * the sensors assigned to the option (e.g. pressure and speed),
     * the width of the affected rim is controlled by the edge width
     * parameter.
     *
     * Should be applied after the texture option, so that the
     * textured dab gets its rim darkened.
     */
    void apply(KisFixedPaintDeviceSP dab, const KisPaintInformation &info) const;

private:
    KisWetEdgeOption(const KisWetEdgeOptionData &data);
private:
    int m_edgeWidth;
};

#endif // KISWETEDGEOPTION_H
