#include "mosaicbar.h"
#include "core/utils.h"

#include <QVariant>

MosaicBar::MosaicBar(Workspace *workspace) : PropsBar(workspace)
{
   createWidget();
   loadProps();
   connectEvents();
}

void MosaicBar::createWidget()
{
    //线宽选择组件构造添加
    //m_sizeWidget = new SizeWidget();
    //m_sizeWidget->setValueList(QVector<int>{Utils::MosaicSizeDefault(),12,18});
    //m_layout->addWidget(m_sizeWidget);

    //线宽选择组件构造添加
    m_sizeWidget = new SizeInputWidget();
    m_sizeWidget->setValue(Utils::MosaicSizeDefault());
    m_layout->addWidget(m_sizeWidget);
}

void MosaicBar::loadProps()
{
    int mSize = m_store->read(Utils::forMosaicKey(),Utils::MosaicSizeName(),Utils::MosaicSizeDefault()).toInt();

    m_sizeWidget->setValue(mSize);
}

void MosaicBar::saveProps()
{
    int mSize = m_sizeWidget->value();

    m_store->write(Utils::forMosaicKey(),Utils::MosaicSizeName(),mSize);
}

void MosaicBar::connectEvents()
{
    connect(m_sizeWidget, SIGNAL(sizeChanged()), this, SLOT(propsChanged()));
}