#include "textassist.h"
#include "screen/workspace.h"
#include "core/useroper.h"
#include "commands/addcommand.h"
#include "commands/deletecommand.h"
#include "commands/textcontentcommand.h"

TextAssist::TextAssist(Workspace *workspace)
{
    m_editing = false;
    this->m_workspace = workspace;
    m_textWidget = new TextWidget(workspace->widget());
    m_textWidget->hide();
}

bool TextAssist::editing() const
{
    return m_editing;
}

void TextAssist::attach(Text *shape)
{
    m_editing = true;
    this->m_shape = shape;

    //m_workspace->removeShape(shape,true);
    m_workspace->refreshDraw();

    QPointF location = shape->location();
    m_textWidget->move(location.x()-2,location.y()-2);
    m_textWidget->setFont(shape->font());
    m_textWidget->setTextColor(shape->color());
    m_textWidget->setText(shape->content());

    m_preContent = shape->content();

    m_textWidget->show();
    m_textWidget->makeFocus();
}

void TextAssist::unAttach()
{
    if(m_editing == false)
        return;

    m_editing = false;
    QString content = m_textWidget->text();
    if(content.isEmpty() == false)
    {
        m_shape->setContent(content);
        //m_workspace->addShape(m_shape);
        m_workspace->refreshDraw();

        if(m_preContent.isEmpty() == true)
        {
            //执行的是新增文本编辑功能
            //AddCommand *addComm = new AddCommand(m_workspace,m_shape);
            //UserOper::add(addComm);
        }
        else
        {
            //执行的是修改文本内容功能
            TextContentCommand *txtComm = new TextContentCommand(m_workspace,m_shape,m_preContent);
            UserOper::add(txtComm);
        }
    }
    else
    {
        if(m_preContent.isEmpty() == true)
        {
            //执行的是新增文本图形编辑功能，但未输入任何文本内容
            //这里等于什么都没做，也没法取消重做
            m_workspace->removeShape(m_shape);
        }
        else
        {
            //执行的是删除文本图形功能
            DeleteCommand *delComm = new DeleteCommand(m_workspace,m_shape);
            UserOper::add(delComm);
        }
    }

    m_textWidget->hide();
}

void TextAssist::refreshProps()
{
    m_textWidget->setFont(m_shape->font());

    m_textWidget->setTextColor(m_shape->color());
    m_textWidget->show();
}



