/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */
#ifndef KISWETEDGEOPTIONWIDGET_H
#define KISWETEDGEOPTIONWIDGET_H

#include <KisCurveOptionWidget.h>
#include <KisWetEdgeOptionData.h>

class PAINTOP_EXPORT KisWetEdgeOptionWidget : public KisCurveOptionWidget
{
    Q_OBJECT
public:
    using data_type = KisWetEdgeOptionData;

    KisWetEdgeOptionWidget(lager::cursor<KisWetEdgeOptionData> optionData);
    ~KisWetEdgeOptionWidget();

    void writeOptionSetting(KisPropertiesConfigurationSP setting) const override;
    void readOptionSetting(const KisPropertiesConfigurationSP setting) override;

private:
    struct Private;
    const QScopedPointer<Private> m_d;
};

#endif // KISWETEDGEOPTIONWIDGET_H
