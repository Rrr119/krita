/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "KisWetEdgeOption.h"

#include <vector>

#include <kis_properties_configuration.h>
#include <kis_fixed_paint_device.h>
#include <KoColorSpace.h>

#include <KisWetEdgeOptionData.h>

#include <KisPaintOpOptionUtils.h>
namespace kpou = KisPaintOpOptionUtils;


KisWetEdgeOption::KisWetEdgeOption(const KisPropertiesConfiguration *setting)
    : KisWetEdgeOption(kpou::loadOptionData<KisWetEdgeOptionData>(setting))
{
}

KisWetEdgeOption::KisWetEdgeOption(const KisWetEdgeOptionData &data)
    : KisCurveOption(data)
    , m_edgeWidth(data.edgeWidth)
{
}

void KisWetEdgeOption::apply(KisFixedPaintDeviceSP dab, const KisPaintInformation &info) const
{
    if (!isChecked()) return;

    const qreal strength = computeSizeLikeValue(info);
    if (strength <= 0.0 || m_edgeWidth <= 0) return;

    const QRect rc = dab->bounds();
    const int w = rc.width();
    const int h = rc.height();
    if (w < 3 || h < 3) return;

    const KoColorSpace *cs = dab->colorSpace();
    const quint32 pixelSize = dab->pixelSize();
    const int pixelCount = w * h;

    // 1. Fetch the alpha values of the dab
    std::vector<quint8> alpha(pixelCount);
    {
        const quint8 *ptr = dab->data();
        for (int i = 0; i < pixelCount; i++, ptr += pixelSize) {
            alpha[i] = cs->opacityU8(ptr);
        }
    }

    // 2. Two-pass chamfer distance transform (3-4 weights, fixed point).
    //    The result is the distance to the nearest transparent pixel,
    //    measured in 1/3 pixel units.
    const int INF = 1 << 24;
    std::vector<int> dist(pixelCount);

    for (int i = 0; i < pixelCount; i++) {
        dist[i] = alpha[i] > 0 ? INF : 0;
    }

    for (int y = 0; y < h; y++) {
        const int row = y * w;
        for (int x = 0; x < w; x++) {
            const int i = row + x;
            int d = dist[i];
            if (x > 0) {
                d = qMin(d, dist[i - 1] + 3);
                if (y > 0) d = qMin(d, dist[i - 1 - w] + 4);
            }
            if (y > 0) {
                d = qMin(d, dist[i - w] + 3);
                if (x + 1 < w) d = qMin(d, dist[i + 1 - w] + 4);
            }
            dist[i] = d;
        }
    }

    for (int y = h - 1; y >= 0; y--) {
        const int row = y * w;
        for (int x = w - 1; x >= 0; x--) {
            const int i = row + x;
            int d = dist[i];
            if (x + 1 < w) {
                d = qMin(d, dist[i + 1] + 3);
                if (y + 1 < h) d = qMin(d, dist[i + 1 + w] + 4);
            }
            if (y + 1 < h) {
                d = qMin(d, dist[i + w] + 3);
                if (x > 0) d = qMin(d, dist[i - 1 + w] + 4);
            }
            dist[i] = d;
        }
    }

    // 3. Attenuate the interior of the dab, keeping the rim intact.
    //    The rim then reads as a darker "wet edge" against the
    //    lighter interior, like pigment collected at the boundary
    //    of a drying wash. Note: we deliberately do **not** boost
    //    the rim itself, because per-dab boosting causes visible
    //    banding on overlapping dabs.
    const qreal radius = 0.5 * qMin(w, h);
    const qreal edgeWidthPx = qMax(1.0, radius * m_edgeWidth / 100.0);

    quint8 *ptr = dab->data();
    for (int i = 0; i < pixelCount; i++, ptr += pixelSize) {
        const quint8 a = alpha[i];
        if (a == 0) continue;

        const qreal d = dist[i] / 3.0;
        const qreal t = qBound(0.0, d / edgeWidthPx, 1.0);
        // smoothstep profile: 0 at the rim, 1 in the deep interior
        const qreal interiorFactor = t * t * (3.0 - 2.0 * t);

        const qreal keep = qBound(0.0, 1.0 - 0.8 * strength * interiorFactor, 1.0);
        cs->setOpacity(ptr, quint8(qRound(a * keep)), 1);
    }
}
