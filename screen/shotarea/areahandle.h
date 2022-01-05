#ifndef AREAHANDLE_H
#define AREAHANDLE_H

#include "core/handle.h"
#include "screen/shotarea/arealocator.h"
#include "core/utils.h"

/**
 * @class : AreaHandle
 * @brief : 截图区域控制手柄类定义
 * @note  : 图形选中后出现，鼠标拖拽控制手柄可以修改图形
 */
class AreaHandle : public Handle
{
public:
    explicit AreaHandle(AreaLocator *locator);

    void draw(QPainter &painter);
    QRect boundary();

    void flipAnchorX();
    void flipAnchorY();
    void setCursor(QWidget *targetUI);

private:
    Utils::RectAnchor m_anchor;
};

#endif // AREAHANDLE_H