#ifndef EXAMPLES_FIND_REPLACE_FORM_H_
#define EXAMPLES_FIND_REPLACE_FORM_H_

// duilib
#include "duilib/duilib.h"

class RichEditFindReplace
{
public:
    RichEditFindReplace();

public:
    //设置关联的RichEdit控件
    void SetRichEdit(ui::RichEdit* pRichEdit);

public:
    //查找
    bool FindRichText(const DString& findText, bool bFindDown, bool bMatchCase, bool bMatchWholeWord, ui::Window* pWndDialog);

    //查找下一个
    bool FindNext();

    //替换
    bool ReplaceRichText(const DString& findText, const DString& replaceText, bool bFindDown, bool bMatchCase, bool bMatchWholeWord, ui::Window* pWndDialog);
    
    //全部替换
    bool ReplaceAllRichText(const DString& findText, const DString& replaceText, bool bFindDown, bool bMatchCase, bool bMatchWholeWord, ui::Window* pWndDialog);

protected:
    //没找到，回调函数
    virtual void OnTextNotFound(const DString& findText);

    //开始全部替换
    virtual void OnReplaceAllCoreBegin();

    //完成全部替换
    virtual void OnReplaceAllCoreEnd(int replaceCount);

private:
    //查找函数实现
    bool FindTextSimple(const DString& findText, bool bFindDown, bool bMatchCase, bool bMatchWholeWord);

    //查找并选择
    bool FindAndSelect(const ui::FindTextParam& findParam);

    //比较当前选择与文本是否匹配
    bool SameAsSelected(const DString& replaceText, bool bMatchCase);

    //没找到，回调函数
    void TextNotFound(const DString& findText);

    //调整查找替换窗口的位置
    void AdjustDialogPosition(ui::Window* pWndDialog);

private:
    //RichEdit控件接口
    ui::RichEdit* m_pRichEdit;

    //查找文字
    DString m_sFindNext;

    //替换文字
    DString m_sReplaceWith;

    //搜索方向选项
    bool m_bFindDown;

    //是否区分大小写
    bool m_bMatchCase;

    //是否全字匹配
    bool m_bMatchWholeWord;

private:
    //查找状态: 是否为第一次搜索
    bool m_bFirstSearch;

    //初始搜索的位置
    int32_t m_nInitialSearchPos;

    //旧的光标
    ui::CursorID m_nOldCursor;
};

#endif //EXAMPLES_FIND_REPLACE_FORM_H_
