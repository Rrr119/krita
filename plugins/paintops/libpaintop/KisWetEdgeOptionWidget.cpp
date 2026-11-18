/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "KisWetEdgeOptionWidget.h"


#include <KisLager.h>

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <kis_slider_spin_box.h>

#include <KisWetEdgeOptionModel.h>
#include <KisWidgetConnectionUtils.h>

struct KisWetEdgeOptionWidget::Private
{
    Private(lager::cursor<KisWetEdgeOptionData> optionData)
        : model(optionData.zoom(
                    kislager::lenses::to_base<KisWetEdgeOptionMixIn>)
                )
    {
    }

    KisWetEdgeOptionModel model;
};

KisWetEdgeOptionWidget::KisWetEdgeOptionWidget(lager::cursor<KisWetEdgeOptionData> optionData)
    : KisCurveOptionWidget(optionData.zoom(kislager::lenses::to_base<KisCurveOptionDataCommon>), KisPaintOpOption::GENERAL)
    , m_d(new Private(optionData))
{
    using namespace KisWidgetConnectionUtils;

    QWidget *page = new QWidget;

    QLabel* edgeWidthLbl = new QLabel(i18n("Edge width:"), page);
    KisSliderSpinBox *edgeWidth = new KisSliderSpinBox(page);
    edgeWidth->setRange(1, 100);
    edgeWidth->setSingleStep(1);
    edgeWidth->setSuffix(i18n("%"));

    QHBoxLayout* edgeWidthHL = new QHBoxLayout;
    edgeWidthHL->setContentsMargins(9, 9, 9, 9);
    edgeWidthHL->addWidget(edgeWidthLbl);
    edgeWidthHL->addWidget(edgeWidth, 1);

    QVBoxLayout* pageLayout = new QVBoxLayout(page);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->addLayout(edgeWidthHL);
    pageLayout->addWidget(configurationPage());

    setConfigurationPage(page);

    connectControl(edgeWidth, &m_d->model, "edgeWidth");

    m_d->model.wetEdgeOptionData.bind(std::bind(&KisWetEdgeOptionWidget::emitSettingChanged, this));
}

KisWetEdgeOptionWidget::~KisWetEdgeOptionWidget()
{
}

void KisWetEdgeOptionWidget::writeOptionSetting(KisPropertiesConfigurationSP setting) const
{
    KisCurveOptionWidget::writeOptionSetting(setting);
    m_d->model.wetEdgeOptionData->write(setting.data());
}

void KisWetEdgeOptionWidget::readOptionSetting(const KisPropertiesConfigurationSP setting)
{
    KisWetEdgeOptionMixIn data = *m_d->model.wetEdgeOptionData;
    data.read(setting.data());
    m_d->model.wetEdgeOptionData.set(data);

    KisCurveOptionWidget::readOptionSetting(setting);
}
