#include "ListCtrl.h"
#include "duilib/Control/ListCtrlReportView.h"
#include "duilib/Control/ListCtrlIconView.h"
#include "duilib/Control/ListCtrlListView.h"
#include "duilib/Core/GlobalManager.h"
#include "duilib/Core/Keyboard.h"
#include <set>

namespace ui
{

ListCtrl::ListCtrl(Window* pWindow):
    VBox(pWindow),
    m_pHeaderCtrl(nullptr),
    m_pReportView(nullptr),
    m_pIconView(nullptr),
    m_pListView(nullptr),
    m_bEnableHeaderDragOrder(true),
    m_bShowHeaderCtrl(true),
    m_bEnableRefresh(true),
    m_bMultiSelect(true),
    m_bEnableColumnWidthAuto(true),
    m_bAutoCheckSelect(false),
    m_bHeaderShowCheckBox(false),
    m_bDataItemShowCheckBox(false),
    m_listCtrlType(ListCtrlType::Report),
    m_pRichEdit(nullptr),
    m_bEnableItemEdit(true),
    m_nItemHeight(0),
    m_nHeaderHeight(0),
    m_pData(nullptr),
    m_nSortedColumnId(Box::InvalidIndex),
    m_bSortedUp(false)
{
    size_t nCount = sizeof(m_imageList) / sizeof(m_imageList[0]);
    for (size_t i = 0; i < nCount; ++i) {
        m_imageList[i] = nullptr;
    }

    m_pData = new ListCtrlData;
    m_pData->SetAutoCheckSelect(IsAutoCheckSelect());

    m_pReportView = new ListCtrlReportView(pWindow);
    m_pReportView->SetListCtrl(this);
    m_pReportView->SetDataProvider(m_pData);

    SetDataItemHeight(32, true);
    SetHeaderHeight(32, true);
}

ListCtrl::~ListCtrl()
{
    if (m_pData != nullptr) {
        delete m_pData;
        m_pData = nullptr;
    }
    if (!IsInited() && (m_pReportView != nullptr)) {
        delete m_pReportView;
        m_pReportView = nullptr;
    }
    if (!IsInited() && (m_pIconView != nullptr)) {
        delete m_pIconView;
        m_pIconView = nullptr;
    }
    if (!IsInited() && (m_pListView != nullptr)) {
        delete m_pListView;
        m_pListView = nullptr;
    }
    if (!IsInited() && (m_pRichEdit != nullptr)) {
        delete m_pRichEdit;
        m_pRichEdit = nullptr;
    }
}

DString ListCtrl::GetType() const { return DUI_CTR_LISTCTRL; }

void ListCtrl::SetImageList(ListCtrlType type, const ImageListPtr& spImageList)
{
    if (type == ListCtrlType::Report) {
        m_imageList[0] = spImageList;
    }
    else if (type == ListCtrlType::Icon) {
        m_imageList[1] = spImageList;
    }
    else if (type == ListCtrlType::List) {
        m_imageList[2] = spImageList;
    }
    else {
        ASSERT(0);
    }
}

ImageListPtr ListCtrl::GetImageList(ListCtrlType type) const
{
    if (type == ListCtrlType::Report) {
        return m_imageList[0];
    }
    else if (type == ListCtrlType::Icon) {
        return m_imageList[1];
    }
    else if (type == ListCtrlType::List) {
        return m_imageList[2];
    }
    else {
        return nullptr;
    }
}

void ListCtrl::SetAttribute(const DString& strName, const DString& strValue)
{
    if (strName == _T("header_class")) {
        SetHeaderClass(strValue);
    }
    else if (strName == _T("header_item_class")) {
        SetHeaderItemClass(strValue);
    }
    else if (strName == _T("header_split_box_class")) {
        SetHeaderSplitBoxClass(strValue);
    }
    else if (strName == _T("header_split_control_class")) {
        SetHeaderSplitControlClass(strValue);
    }
    else if (strName == _T("enable_header_drag_order")) {
        SetEnableHeaderDragOrder(strValue == _T("true"));
    }
    else if (strName == _T("check_box_class")) {
        SetCheckBoxClass(strValue);
    }
    else if (strName == _T("data_item_class")) {
        SetDataItemClass(strValue);
    }
    else if (strName == _T("data_sub_item_class")) {
        SetDataSubItemClass(strValue);
    }
    else if (strName == _T("row_grid_line_width")) {
        SetRowGridLineWidth(StringUtil::StringToInt32(strValue), true);
    }
    else if (strName == _T("row_grid_line_color")) {
        SetRowGridLineColor(strValue);
    }
    else if (strName == _T("column_grid_line_width")) {
        SetColumnGridLineWidth(StringUtil::StringToInt32(strValue), true);
    }
    else if (strName == _T("column_grid_line_color")) {
        SetColumnGridLineColor(strValue);
    }
    else if (strName == _T("report_view_class")) {
        SetReportViewClass(strValue);
    }
    else if (strName == _T("header_height")) {
        SetHeaderHeight(StringUtil::StringToInt32(strValue), true);
    }
    else if (strName == _T("data_item_height")) {
        SetDataItemHeight(StringUtil::StringToInt32(strValue), true);
    }
    else if (strName == _T("show_header")) {
        SetHeaderVisible(strValue == _T("true"));
    }
    else if (strName == _T("multi_select")) {
        SetMultiSelect(strValue == _T("true"));
    }
    else if (strName == _T("enable_column_width_auto")) {
        SetEnableColumnWidthAuto(strValue == _T("true"));
    }
    else if (strName == _T("auto_check_select")) {
        SetAutoCheckSelect(strValue == _T("true"));
    }
    else if (strName == _T("show_header_checkbox")) {
        SetHeaderShowCheckBox(strValue == _T("true"));
    }
    else if (strName == _T("show_data_item_checkbox")) {
        SetDataItemShowCheckBox(strValue == _T("true"));
    }
    else if (strName == _T("type")) {
        if (strValue == _T("report")) {
            SetListCtrlType(ListCtrlType::Report);
        }
        else if (strValue == _T("icon")) {
            SetListCtrlType(ListCtrlType::Icon);
        }
        else if (strValue == _T("list")) {
            SetListCtrlType(ListCtrlType::List);
        }
    }
    else if (strName == _T("icon_view_class")) {
        SetIconViewClass(strValue);
    }
    else if (strName == _T("icon_view_item_class")) {
        SetIconViewItemClass(strValue);
    }
    else if (strName == _T("icon_view_item_image_class")) {
        SetIconViewItemImageClass(strValue);
    }
    else if (strName == _T("icon_view_item_label_class")) {
        SetIconViewItemLabelClass(strValue);
    }
    else if (strName == _T("list_view_class")) {
        SetListViewClass(strValue);
    }
    else if (strName == _T("list_view_item_class")) {
        SetListViewItemClass(strValue);
    }
    else if (strName == _T("list_view_item_image_class")) {
        SetListViewItemImageClass(strValue);
    }
    else if (strName == _T("list_view_item_label_class")) {
        SetListViewItemLabelClass(strValue);
    }
    else if (strName == _T("enable_item_edit")) {
        SetEnableItemEdit(strValue == _T("true"));
    }
    else if (strName == _T("list_ctrl_richedit_class")) {
        SetRichEditClass(strValue);
    }
    else {
        BaseClass::SetAttribute(strName, strValue);
    }
}

void ListCtrl::ChangeDpiScale(uint32_t nOldDpiScale, uint32_t nNewDpiScale)
{
    ASSERT(nNewDpiScale == Dpi().GetScale());
    if (nNewDpiScale != Dpi().GetScale()) {
        return;
    }

    //首先禁止界面刷新
    bool bOldValue = SetEnableRefresh(false);
    int32_t iValue = GetRowGridLineWidth();
    iValue = Dpi().GetScaleInt(iValue, nOldDpiScale);
    SetRowGridLineWidth(iValue, false);

    iValue = GetColumnGridLineWidth();
    iValue = Dpi().GetScaleInt(iValue, nOldDpiScale);
    SetColumnGridLineWidth(iValue, false);

    if (m_nHeaderHeight > 0) {
        m_nHeaderHeight = Dpi().GetScaleInt(m_nHeaderHeight, nOldDpiScale);
    }

    iValue = GetDataItemHeight();
    iValue = Dpi().GetScaleInt(iValue, nOldDpiScale);
    SetDataItemHeight(iValue, false);

    if (m_pData != nullptr) {
        m_pData->ChangeDpiScale(Dpi(), nOldDpiScale);
    }

    size_t nCount = sizeof(m_imageList) / sizeof(m_imageList[0]);
    for (size_t i = 0; i < nCount; ++i) {
        if (m_imageList[i] != nullptr) {
            m_imageList[i]->ChangeDpiScale(Dpi(), nOldDpiScale);
        }
    }
    BaseClass::ChangeDpiScale(nOldDpiScale, nNewDpiScale);

    //更新列宽
    size_t nColumnCount = GetColumnCount();
    for (size_t columnIndex = 0; columnIndex < nColumnCount; ++columnIndex) {
        int32_t nColumnWidth = GetColumnWidth(columnIndex);
        if (nColumnWidth > 0) {
            SetColumnWidth(columnIndex, nColumnWidth, false);
        }        
    }
    //刷新数据
    SetEnableRefresh(bOldValue);
    Refresh();
}

void ListCtrl::HandleEvent(const EventArgs& msg)
{
    BaseClass::HandleEvent(msg);
    if ((msg.eventType > kEventKeyBegin) && (msg.eventType < kEventKeyEnd)) {
        OnViewKeyboardEvents(msg);
    }
    else if ((msg.eventType > kEventMouseBegin) && (msg.eventType < kEventMouseEnd)) {
        OnViewMouseEvents(msg);
    }
}

void ListCtrl::OnInit()
{
    if (IsInited()) {
        return;
    }
    BaseClass::OnInit();
    //初始化Body
    ASSERT(m_pReportView != nullptr);
    AddItem(m_pReportView);
    m_pReportView->SetClass(GetReportViewClass());

    //初始化Header
    ASSERT(m_pHeaderCtrl == nullptr);
    if (m_pHeaderCtrl == nullptr) {
        m_pHeaderCtrl = new ListCtrlHeader(GetWindow());
    }
    m_pHeaderCtrl->SetListCtrl(this);
    // Header添加到数据视图中管理，作为第一个元素，在Layout的实现中控制显示属性
    m_pReportView->AddItem(m_pHeaderCtrl);

    if (!m_headerClass.empty()) {
        m_pHeaderCtrl->SetClass(m_headerClass.c_str());
    }
    m_pHeaderCtrl->SetAutoCheckSelect(false);
    m_pHeaderCtrl->SetShowCheckBox(m_bHeaderShowCheckBox); //是否显示CheckBox
    int32_t nHeaderHeight = m_nHeaderHeight;
    if (nHeaderHeight < 0) {
        nHeaderHeight = GetDataItemHeight();
    }
    m_pHeaderCtrl->SetFixedHeight(UiFixedInt(nHeaderHeight), true, false);
    if (!m_bShowHeaderCtrl) {
        SetHeaderVisible(false);
    }

    //同步单选和多选的状态
    m_pReportView->SetDataProvider(m_pData);

    //更新默认的文本属性
    SetDataSubItemClass(GetDataSubItemClass());

    //更新默认的行高
    m_pData->SetDefaultItemHeight(GetDataItemHeight());

    InitReportView();
    SetListCtrlType(GetListCtrlType());
}

void ListCtrl::InitReportView()
{
    ASSERT(m_pReportView != nullptr);
    if (m_pReportView == nullptr) {
        return;
    }
    m_pReportView->SetMultiSelect(IsMultiSelect());

    //事件转接函数
    auto OnReportViewEvent = [this](const EventArgs& args) {
            size_t nItemIndex = args.wParam;
            Control* pControl = m_pReportView->GetItemAt(nItemIndex);
            ListCtrlItem* pItem = nullptr;
            if (pControl != nullptr) {
                pItem = dynamic_cast<ListCtrlItem*>(pControl);
            }
            if (pItem != nullptr) {
                EventArgs msg = args;
                msg.wParam = (WPARAM)pItem;
                msg.lParam = pItem->GetElementIndex();
                msg.SetSender(this);
                SendEventMsg(msg);
            }
            else if (args.eventType == kEventSelChange) {
                EventArgs msg = args;
                msg.SetSender(this);
                SendEventMsg(msg);
            }
        };

    //挂载事件，转接给外层
    m_pReportView->AttachSelect([this, OnReportViewEvent](const EventArgs& args) {
        OnReportViewEvent(args);
        return true;
        });
    m_pReportView->AttachSelChange([this, OnReportViewEvent](const EventArgs& args) {
        OnReportViewEvent(args);
        return true;
        });
    m_pReportView->AttachDoubleClick([this, OnReportViewEvent](const EventArgs& args) {
        OnReportViewEvent(args);
        return true;
        });
    m_pReportView->AttachClick([this, OnReportViewEvent](const EventArgs& args) {
        OnReportViewEvent(args);
        return true;
        });
    m_pReportView->AttachRClick([this, OnReportViewEvent](const EventArgs& args) {
        OnReportViewEvent(args);
        return true;
        });
    m_pReportView->AttachReturn([this, OnReportViewEvent](const EventArgs& args) {
        OnReportViewEvent(args);
        return true;
        });
}

void ListCtrl::InitIconView()
{
    ASSERT(m_pIconView != nullptr);
    if (m_pIconView == nullptr) {
        return;
    }
    m_pIconView->SetMultiSelect(IsMultiSelect());

    //事件转接函数
    auto OnIconViewEvent = [this](const EventArgs& args) {
            size_t nItemIndex = args.wParam;
            Control* pControl = m_pIconView->GetItemAt(nItemIndex);
            ListCtrlIconViewItem* pItem = nullptr;
            if (pControl != nullptr) {
                pItem = dynamic_cast<ListCtrlIconViewItem*>(pControl);
            }
            if (pItem != nullptr) {
                EventArgs msg = args;
                msg.wParam = (WPARAM)pItem;
                msg.lParam = pItem->GetElementIndex();
                msg.SetSender(this);
                SendEventMsg(msg);
            }
            else if (args.eventType == kEventSelChange) {
                EventArgs msg = args;
                msg.SetSender(this);
                SendEventMsg(msg);
            }
        };

    //挂载事件，转接给外层
    m_pIconView->AttachSelect([this, OnIconViewEvent](const EventArgs& args) {
        OnIconViewEvent(args);
        return true;
        });
    m_pIconView->AttachSelChange([this, OnIconViewEvent](const EventArgs& args) {
        OnIconViewEvent(args);
        return true;
        });
    m_pIconView->AttachDoubleClick([this, OnIconViewEvent](const EventArgs& args) {
        OnIconViewEvent(args);
        return true;
        });
    m_pIconView->AttachClick([this, OnIconViewEvent](const EventArgs& args) {
        OnIconViewEvent(args);
        return true;
        });
    m_pIconView->AttachRClick([this, OnIconViewEvent](const EventArgs& args) {
        OnIconViewEvent(args);
        return true;
        });
    m_pIconView->AttachReturn([this, OnIconViewEvent](const EventArgs& args) {
        OnIconViewEvent(args);
        return true;
        });
}

void ListCtrl::InitListView()
{
    ASSERT(m_pListView != nullptr);
    if (m_pListView == nullptr) {
        return;
    }
    if (m_pIconView != nullptr) {
        m_pIconView->SetMultiSelect(IsMultiSelect());
    }
    
    //事件转接函数
    auto OnListViewEvent = [this](const EventArgs& args) {
            size_t nItemIndex = args.wParam;
            Control* pControl = m_pListView->GetItemAt(nItemIndex);
            ListCtrlListViewItem* pItem = nullptr;
            if (pControl != nullptr) {
                pItem = dynamic_cast<ListCtrlListViewItem*>(pControl);
            }
            if (pItem != nullptr) {
                EventArgs msg = args;
                msg.wParam = (WPARAM)pItem;
                msg.lParam = pItem->GetElementIndex();
                msg.SetSender(this);
                SendEventMsg(msg);
            }
            else if (args.eventType == kEventSelChange) {
                EventArgs msg = args;
                msg.SetSender(this);
                SendEventMsg(msg);
            }
        };

    //挂载事件，转接给外层
    m_pListView->AttachSelect([this, OnListViewEvent](const EventArgs& args) {
        OnListViewEvent(args);
        return true;
        });
    m_pListView->AttachSelChange([this, OnListViewEvent](const EventArgs& args) {
        OnListViewEvent(args);
        return true;
        });
    m_pListView->AttachDoubleClick([this, OnListViewEvent](const EventArgs& args) {
        OnListViewEvent(args);
        return true;
        });
    m_pListView->AttachClick([this, OnListViewEvent](const EventArgs& args) {
        OnListViewEvent(args);
        return true;
        });
    m_pListView->AttachRClick([this, OnListViewEvent](const EventArgs& args) {
        OnListViewEvent(args);
        return true;
        });
    m_pListView->AttachReturn([this, OnListViewEvent](const EventArgs& args) {
        OnListViewEvent(args);
        return true;
        });
}

void ListCtrl::SetListCtrlType(ListCtrlType type)
{
    m_listCtrlType = type;
    if (!IsInited()) {
        return;
    }
    if (m_listCtrlType == ListCtrlType::Report) {
        //切换为Report视图
        ASSERT(m_pReportView != nullptr);
        if (m_pReportView != nullptr) {
            m_pReportView->SetDataProvider(m_pData);
            m_pData->SetListView(m_pReportView);
            m_pReportView->SetVisible(true);
            if (m_pReportView->IsMultiSelect() != IsMultiSelect()) {
                m_pReportView->SetMultiSelect(IsMultiSelect());
            }
        }
        if (m_pIconView != nullptr) {
            m_pIconView->SetVisible(false);
            m_pIconView->SetDataProvider(nullptr);            
        }
        if (m_pListView != nullptr) {
            m_pListView->SetVisible(false);
            m_pListView->SetDataProvider(nullptr);            
        }
    }
    else if (m_listCtrlType == ListCtrlType::Icon) {
        //切换为Icon视图
        if (m_pIconView == nullptr) {
            m_pIconView = new ListCtrlIconView(GetWindow());
            m_pIconView->SetListCtrl(this);
            AddItem(m_pIconView);
            m_pIconView->SetClass(GetIconViewClass());
            InitIconView();
        }
        m_pIconView->SetDataProvider(m_pData);
        m_pData->SetListView(m_pIconView);
        m_pIconView->SetVisible(true);
        if (m_pIconView->IsMultiSelect() != IsMultiSelect()) {
            m_pIconView->SetMultiSelect(IsMultiSelect());
        }
        if (m_pReportView != nullptr) {
            m_pReportView->SetVisible(false);
            m_pReportView->SetDataProvider(nullptr);
        }
        if (m_pListView != nullptr) {
            m_pListView->SetVisible(false);
            m_pListView->SetDataProvider(nullptr);
        }
    }
    else if (m_listCtrlType == ListCtrlType::List) {
        //切换为List视图
        if (m_pListView == nullptr) {
            m_pListView = new ListCtrlListView(GetWindow());
            m_pListView->SetListCtrl(this);
            AddItem(m_pListView);
            m_pListView->SetClass(GetListViewClass());
            InitListView();
        }
        m_pListView->SetDataProvider(m_pData);
        m_pData->SetListView(m_pListView);
        m_pListView->SetVisible(true);
        if (m_pListView->IsMultiSelect() != IsMultiSelect()) {
            m_pListView->SetMultiSelect(IsMultiSelect());
        }
        if (m_pReportView != nullptr) {
            m_pReportView->SetVisible(false);
            m_pReportView->SetDataProvider(nullptr);
        }
        if (m_pIconView != nullptr) {
            m_pIconView->SetVisible(false);
            m_pIconView->SetDataProvider(nullptr);
        }
    }
}

ListCtrlType ListCtrl::GetListCtrlType() const
{
    return m_listCtrlType;
}

ListCtrlReportView* ListCtrl::GetReportView() const
{
    return m_pReportView;
}

ListCtrlIconView* ListCtrl::GetIconView() const
{
    return m_pIconView;
}

ListCtrlListView* ListCtrl::GetListView() const
{
    return m_pListView;
}

void ListCtrl::SetHeaderClass(const DString& className)
{
    m_headerClass = className;
    if (IsInited() && (m_pHeaderCtrl != nullptr)) {
        m_pHeaderCtrl->SetClass(className);
    }
}

void ListCtrl::SetHeaderItemClass(const DString& className)
{
    m_headerItemClass = className;
}

DString ListCtrl::GetHeaderItemClass() const
{
    return m_headerItemClass.c_str();
}

void ListCtrl::SetHeaderSplitBoxClass(const DString& className)
{
    m_headerSplitBoxClass = className;
}

DString ListCtrl::GetHeaderSplitBoxClass() const
{
    return m_headerSplitBoxClass.c_str();
}

void ListCtrl::SetHeaderSplitControlClass(const DString& className)
{
    m_headerSplitControlClass = className;
}

DString ListCtrl::GetHeaderSplitControlClass() const
{
    return m_headerSplitControlClass.c_str();
}

void ListCtrl::SetCheckBoxClass(const DString& className)
{
    m_checkBoxClass = className;
}

DString ListCtrl::GetCheckBoxClass() const
{
    return m_checkBoxClass.c_str();
}

void ListCtrl::SetReportViewClass(const DString& className)
{
    m_reportViewClass = className;
    if (IsInited() && (m_pReportView != nullptr)) {
        m_pReportView->SetClass(className);
    }
}

DString ListCtrl::GetReportViewClass() const
{
    return m_reportViewClass.c_str();
}

void ListCtrl::SetDataItemClass(const DString& className)
{
    m_dataItemClass = className;
}

DString ListCtrl::GetDataItemClass() const
{
    return m_dataItemClass.c_str();
}

void ListCtrl::SetDataSubItemClass(const DString& className)
{
    m_dataSubItemClass = className;
    if (IsInited() && !className.empty()) {
        ListCtrlSubItem defaultSubItem(GetWindow());
        defaultSubItem.SetClass(className);
        m_pData->SetDefaultTextStyle(defaultSubItem.GetTextStyle());
    }
}

DString ListCtrl::GetDataSubItemClass() const
{
    return m_dataSubItemClass.c_str();
}

void ListCtrl::SetIconViewClass(const DString& className)
{
    m_iconViewClass = className;
    if (IsInited() && (m_pIconView != nullptr)) {
        m_pIconView->SetClass(className);
    }
}

DString ListCtrl::GetIconViewClass() const
{
    return m_iconViewClass.c_str();
}

void ListCtrl::SetIconViewItemClass(const DString& className)
{
    m_iconViewItemClass = className;
}

DString ListCtrl::GetIconViewItemClass() const
{
    return m_iconViewItemClass.c_str();
}

void ListCtrl::SetIconViewItemImageClass(const DString& className)
{
    m_iconViewItemImageClass = className;
}

DString ListCtrl::GetIconViewItemImageClass() const
{
    return m_iconViewItemImageClass.c_str();
}

void ListCtrl::SetIconViewItemLabelClass(const DString& className)
{
    m_iconViewItemLabelClass = className;
}

DString ListCtrl::GetIconViewItemLabelClass() const
{
    return m_iconViewItemLabelClass.c_str();
}

void ListCtrl::SetListViewClass(const DString& className)
{
    m_listViewClass = className;
    if (IsInited() && (m_pListView != nullptr)) {
        m_pListView->SetClass(className);
    }
}

DString ListCtrl::GetListViewClass() const
{
    return m_listViewClass.c_str();
}

void ListCtrl::SetListViewItemClass(const DString& className)
{
    m_listViewItemClass = className;
}

DString ListCtrl::GetListViewItemClass() const
{
    return m_listViewItemClass.c_str();
}

void ListCtrl::SetListViewItemImageClass(const DString& className)
{
    m_listViewItemImageClass = className;
}

DString ListCtrl::GetListViewItemImageClass() const
{
    return m_listViewItemImageClass.c_str();
}

void ListCtrl::SetListViewItemLabelClass(const DString& className)
{
    m_listViewItemLabelClass = className;
}

DString ListCtrl::GetListViewItemLabelClass() const
{
    return m_listViewItemLabelClass.c_str();
}

void ListCtrl::SetRichEditClass(const DString& richEditClass)
{
    if (m_listCtrlRichEditClass != richEditClass) {
        m_listCtrlRichEditClass = richEditClass;
        if (IsInited() && (m_pRichEdit != nullptr)) {
            m_pRichEdit->SetClass(richEditClass);
        }
    }
}

void ListCtrl::SetEnableItemEdit(bool bEnableItemEdit)
{
    m_bEnableItemEdit = bEnableItemEdit;
}

bool ListCtrl::IsEnableItemEdit() const
{
    return m_bEnableItemEdit;
}

RichEdit* ListCtrl::GetRichEdit() const
{
    return m_pRichEdit;
}

DString ListCtrl::GetRichEditClass() const
{
    return m_listCtrlRichEditClass.c_str();
}

void ListCtrl::SetRowGridLineWidth(int32_t nLineWidth, bool bNeedDpiScale)
{
    m_pReportView->SetRowGridLineWidth(nLineWidth, bNeedDpiScale);
}

int32_t ListCtrl::GetRowGridLineWidth() const
{
    return m_pReportView->GetRowGridLineWidth();
}

void ListCtrl::SetRowGridLineColor(const DString& color)
{
    m_pReportView->SetRowGridLineColor(color);
}

DString ListCtrl::GetRowGridLineColor() const
{
    return m_pReportView->GetRowGridLineColor();
}

void ListCtrl::SetColumnGridLineWidth(int32_t nLineWidth, bool bNeedDpiScale)
{
    m_pReportView->SetColumnGridLineWidth(nLineWidth, bNeedDpiScale);
}

int32_t ListCtrl::GetColumnGridLineWidth() const
{
    return m_pReportView->GetColumnGridLineWidth();
}

void ListCtrl::SetColumnGridLineColor(const DString& color)
{
    m_pReportView->SetColumnGridLineColor(color);
}

DString ListCtrl::GetColumnGridLineColor() const
{
    return m_pReportView->GetColumnGridLineColor();
}

void ListCtrl::SetEnableColumnWidthAuto(bool bEnable)
{
    m_bEnableColumnWidthAuto = bEnable;
}

bool ListCtrl::IsEnableColumnWidthAuto() const
{
    return m_bEnableColumnWidthAuto;
}

ListCtrlHeaderItem* ListCtrl::InsertColumn(int32_t columnIndex, const ListCtrlColumn& columnInfo)
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return nullptr;
    }
    else {
        return m_pHeaderCtrl->InsertColumn(columnIndex, columnInfo);
    }
}

size_t ListCtrl::GetColumnCount() const
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return 0;
    }
    else {
        return m_pHeaderCtrl->GetColumnCount();
    }
}

int32_t ListCtrl::GetColumnWidth(size_t columnIndex) const
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return 0;
    }
    else {
        return m_pHeaderCtrl->GetColumnWidth(columnIndex);
    }
}

int32_t ListCtrl::GetColumnWidthById(size_t columnId) const
{
    size_t columnIndex = GetColumnIndex(columnId);
    if (Box::IsValidItemIndex(columnIndex)) {
        return GetColumnWidth(columnIndex);
    }
    return 0;
}

bool ListCtrl::SetColumnWidth(size_t columnIndex, int32_t nWidth, bool bNeedDpiScale)
{
    bool bRet = false;
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl != nullptr) {
        bRet = m_pHeaderCtrl->SetColumnWidth(columnIndex, nWidth, bNeedDpiScale);
    }
    if(bRet) {
        OnColumnWidthChanged(GetColumnId(columnIndex), Box::InvalidIndex);
    }
    return true;
}

bool ListCtrl::SetColumnWidthById(size_t columnId, int32_t nWidth, bool bNeedDpiScale)
{
    size_t columnIndex = GetColumnIndex(columnId);
    if (Box::IsValidItemIndex(columnIndex)) {
        return SetColumnWidth(columnIndex, nWidth, bNeedDpiScale);
    }
    return false;
}

bool ListCtrl::SetColumnWidthAuto(size_t columnIndex)
{
    bool bRet = false;
    size_t nColumnId = GetColumnId(columnIndex);
    if (nColumnId == Box::InvalidIndex) {
        return bRet;
    }
    //计算该列的宽度
    int32_t nMaxWidth = m_pData->GetMaxColumnWidth(nColumnId);
    if (nMaxWidth > 0) {
        bRet = SetColumnWidth(columnIndex, nMaxWidth, false);
    }
    return bRet;
}

bool ListCtrl::SetColumnWidthAutoById(size_t columnId)
{
    size_t columnIndex = GetColumnIndex(columnId);
    if (Box::IsValidItemIndex(columnIndex)) {
        return SetColumnWidthAuto(columnIndex);
    }
    return false;
}

ListCtrlHeaderItem* ListCtrl::GetColumn(size_t columnIndex) const
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return nullptr;
    }
    else {
        return m_pHeaderCtrl->GetColumn(columnIndex);
    }
}

ListCtrlHeaderItem* ListCtrl::GetColumnById(size_t columnId) const
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return nullptr;
    }
    else {
        return m_pHeaderCtrl->GetColumnById(columnId);
    }
}

size_t ListCtrl::GetColumnIndex(size_t columnId) const
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return Box::InvalidIndex;
    }
    else {
        return m_pHeaderCtrl->GetColumnIndex(columnId);
    }
}

size_t ListCtrl::GetColumnId(size_t columnIndex) const
{
    ListCtrlHeaderItem* pHeaderItem = GetColumn(columnIndex);
    if (pHeaderItem != nullptr) {
        return pHeaderItem->GetColumnId();
    }
    return Box::InvalidIndex;
}

bool ListCtrl::IsValidColumnId(size_t columnId) const
{
    size_t nColumnIndex = GetColumnIndex(columnId);
    return nColumnIndex != Box::InvalidIndex;
}

bool ListCtrl::DeleteColumn(size_t columnIndex)
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return false;
    }
    else {
        return m_pHeaderCtrl->DeleteColumn(columnIndex);
    }
}

bool ListCtrl::DeleteColumnById(size_t columnId)
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return false;
    }
    else {
        return m_pHeaderCtrl->DeleteColumnById(columnId);
    }
}

ListCtrlHeader* ListCtrl::GetHeaderCtrl() const
{
    return m_pHeaderCtrl;
}

void ListCtrl::SetEnableHeaderDragOrder(bool bEnable)
{
    m_bEnableHeaderDragOrder = bEnable;
}

bool ListCtrl::IsEnableHeaderDragOrder() const
{
    return m_bEnableHeaderDragOrder;
}

void ListCtrl::SetHeaderVisible(bool bVisible)
{
    m_bShowHeaderCtrl = bVisible;
    if (m_pHeaderCtrl != nullptr) {
        m_pHeaderCtrl->SetVisible(bVisible);
    }
    Refresh();
}

bool ListCtrl::IsHeaderVisible() const
{
    if (m_pHeaderCtrl != nullptr) {
        return m_pHeaderCtrl->IsVisible();
    }
    else if(!IsInited()){
        return m_bShowHeaderCtrl;
    }
    else {
        return false;
    }
}

void ListCtrl::SetHeaderHeight(int32_t nHeaderHeight, bool bNeedDpiScale)
{
    if (nHeaderHeight < 0) {
        nHeaderHeight = 0;
    }
    if (bNeedDpiScale) {
        Dpi().ScaleInt(nHeaderHeight);
    }
    if (m_pHeaderCtrl != nullptr) {
        m_pHeaderCtrl->SetFixedHeight(UiFixedInt(nHeaderHeight), true, false);
    }
    m_nHeaderHeight = nHeaderHeight;
    Refresh();
}

int32_t ListCtrl::GetHeaderHeight() const
{
    int32_t nHeaderHeight = 0;
    if (m_pHeaderCtrl != nullptr) {
        nHeaderHeight = m_pHeaderCtrl->GetFixedHeight().GetInt32();
    }
    else {
        nHeaderHeight = m_nHeaderHeight;
        if (nHeaderHeight < 0) {
            nHeaderHeight = GetDataItemHeight();
        }
    }
    return nHeaderHeight;
}

void ListCtrl::SetDataItemHeight(int32_t nItemHeight, bool bNeedDpiScale)
{
    ASSERT(nItemHeight > 0);
    if (nItemHeight <= 0) {
        return;
    }
    if (bNeedDpiScale) {
        Dpi().ScaleInt(nItemHeight);
    }
    if (m_nItemHeight != nItemHeight) {
        m_nItemHeight = nItemHeight;
        //更新默认的行高
        m_pData->SetDefaultItemHeight(nItemHeight);
        Refresh();
    }
}

int32_t ListCtrl::GetDataItemHeight() const
{
    return m_nItemHeight;
}

ListCtrlItem* ListCtrl::GetFirstDisplayItem() const
{
    ListCtrlItem* pItem = nullptr;
    ASSERT(m_pReportView != nullptr);
    if (m_pReportView != nullptr) {
        size_t itemCount = m_pReportView->GetItemCount();
        for (size_t index = 0; index < itemCount; ++index) {
            pItem = dynamic_cast<ListCtrlItem*>(m_pReportView->GetItemAt(index));
            if ((pItem != nullptr) && pItem->IsVisible()) {
                break;
            }
        }
    }
    return pItem;
}

ListCtrlItem* ListCtrl::GetNextDisplayItem(ListCtrlItem* pItem) const
{
    ListCtrlItem* pNextItem = nullptr;
    if (pItem == nullptr) {
        pNextItem = GetFirstDisplayItem();
    }
    else {
        ASSERT(m_pReportView != nullptr);
        if (m_pReportView != nullptr) {
            size_t itemCount = m_pReportView->GetItemCount();
            size_t nStartIndex = m_pReportView->GetItemIndex(pItem);
            if ((itemCount > 0) && (nStartIndex < (itemCount - 1))) {
                for (size_t index = nStartIndex + 1; index < itemCount; ++index) {
                    ListCtrlItem* pCheckItem = dynamic_cast<ListCtrlItem*>(m_pReportView->GetItemAt(index));
                    if ((pCheckItem != nullptr) && pCheckItem->IsVisible()) {
                        pNextItem = pCheckItem;
                        break;
                    }
                }
            }
        }
    }
    return pNextItem;
}

ListCtrlIconViewItem* ListCtrl::GetFirstDisplayIconItem() const
{
    ListCtrlIconViewItem* pItem = nullptr;
    ASSERT(m_pIconView != nullptr);
    if (m_pIconView != nullptr) {
        size_t itemCount = m_pIconView->GetItemCount();
        for (size_t index = 0; index < itemCount; ++index) {
            pItem = dynamic_cast<ListCtrlIconViewItem*>(m_pIconView->GetItemAt(index));
            if ((pItem != nullptr) && pItem->IsVisible()) {
                break;
            }
        }
    }
    return pItem;
}

ListCtrlIconViewItem* ListCtrl::GetNextDisplayIconItem(ListCtrlIconViewItem* pItem) const
{
    ListCtrlIconViewItem* pNextItem = nullptr;
    if (pItem == nullptr) {
        pNextItem = GetFirstDisplayIconItem();
    }
    else {
        ASSERT(m_pIconView != nullptr);
        if (m_pIconView != nullptr) {
            size_t itemCount = m_pIconView->GetItemCount();
            size_t nStartIndex = m_pIconView->GetItemIndex(pItem);
            if ((itemCount > 0) && (nStartIndex < (itemCount - 1))) {
                for (size_t index = nStartIndex + 1; index < itemCount; ++index) {
                    ListCtrlIconViewItem* pCheckItem = dynamic_cast<ListCtrlIconViewItem*>(m_pIconView->GetItemAt(index));
                    if ((pCheckItem != nullptr) && pCheckItem->IsVisible()) {
                        pNextItem = pCheckItem;
                        break;
                    }
                }
            }
        }
    }
    return pNextItem;
}

ListCtrlListViewItem* ListCtrl::GetFirstDisplayListItem() const
{
    ListCtrlListViewItem* pItem = nullptr;
    ASSERT(m_pListView != nullptr);
    if (m_pListView != nullptr) {
        size_t itemCount = m_pListView->GetItemCount();
        for (size_t index = 0; index < itemCount; ++index) {
            pItem = dynamic_cast<ListCtrlListViewItem*>(m_pListView->GetItemAt(index));
            if ((pItem != nullptr) && pItem->IsVisible()) {
                break;
            }
        }
    }
    return pItem;
}

ListCtrlListViewItem* ListCtrl::GetNextDisplayListItem(ListCtrlListViewItem* pItem) const
{
    ListCtrlListViewItem* pNextItem = nullptr;
    if (pItem == nullptr) {
        pNextItem = GetFirstDisplayListItem();
    }
    else {
        ASSERT(m_pListView != nullptr);
        if (m_pListView != nullptr) {
            size_t itemCount = m_pListView->GetItemCount();
            size_t nStartIndex = m_pListView->GetItemIndex(pItem);
            if ((itemCount > 0) && (nStartIndex < (itemCount - 1))) {
                for (size_t index = nStartIndex + 1; index < itemCount; ++index) {
                    ListCtrlListViewItem* pCheckItem = dynamic_cast<ListCtrlListViewItem*>(m_pListView->GetItemAt(index));
                    if ((pCheckItem != nullptr) && pCheckItem->IsVisible()) {
                        pNextItem = pCheckItem;
                        break;
                    }
                }
            }
        }
    }
    return pNextItem;
}

void ListCtrl::OnColumnWidthChanged(size_t nColumnId1, size_t nColumnId2)
{
    if ((m_pReportView == nullptr) || (m_pHeaderCtrl == nullptr)){
        return;
    }

    size_t nColumn1 = Box::InvalidIndex;
    size_t nColumn2 = Box::InvalidIndex;
    int32_t nColumnWidth1 = -1;
    int32_t nColumnWidth2 = -1;
    if (!m_pHeaderCtrl->GetColumnInfo(nColumnId1, nColumn1, nColumnWidth1)) {
        nColumnWidth1 = -1;
    }
    if (!m_pHeaderCtrl->GetColumnInfo(nColumnId2, nColumn2, nColumnWidth2)) {
        nColumnWidth2 = -1;
    }
    std::map<size_t, int32_t> subItemWidths;
    if (nColumnWidth1 >= 0) {
        subItemWidths[nColumn1] = nColumnWidth1;
    }
    if (nColumnWidth2 >= 0) {
        subItemWidths[nColumn2] = nColumnWidth2;
    }
    if (subItemWidths.empty()) {
        return;
    }
    m_pReportView->AdjustSubItemWidth(subItemWidths);
    Arrange();
}

void ListCtrl::OnHeaderColumnAdded(size_t nColumnId)
{
    m_pData->AddColumn(nColumnId);
}

void ListCtrl::OnHeaderColumnRemoved(size_t nColumnId)
{
    m_columnSortFlagMap.erase(nColumnId);
    m_pData->RemoveColumn(nColumnId);
}

void ListCtrl::OnColumnSorted(size_t nColumnId, bool bSortedUp)
{
    //对数据排序，然后刷新界面显示
    m_nSortedColumnId = nColumnId;
    m_bSortedUp = bSortedUp;
    uint8_t nSortFlag = GetColumnSortFlagById(m_nSortedColumnId);
    m_pData->SortDataItems(nColumnId, GetColumnIndex(nColumnId), bSortedUp, nSortFlag, nullptr, nullptr);
    Refresh();
}

bool ListCtrl::GetSortColumn(size_t& nSortColumnId, bool& bSortUp) const
{
    nSortColumnId = Box::InvalidIndex;
    if (Box::IsValidItemIndex(GetColumnIndex(m_nSortedColumnId))) {
        nSortColumnId = m_nSortedColumnId;
        bSortUp = m_bSortedUp;
        return true;
    }
    return false;
}

bool ListCtrl::SortDataItems(size_t columnIndex, bool bSortedUp, uint8_t nSortFlag,
                             ListCtrlDataCompareFunc pfnCompareFunc,
                             void* pUserData)
{
    size_t nColumnId = GetColumnId(columnIndex);
    ASSERT(nColumnId != Box::InvalidIndex);
    if (nColumnId == Box::InvalidIndex) {
        return false;
    }
    return SortDataItemsById(nColumnId, bSortedUp, nSortFlag, pfnCompareFunc, pUserData);
}

bool ListCtrl::SortDataItemsById(size_t columnId, bool bSortedUp, uint8_t nSortFlag,
                                 ListCtrlDataCompareFunc pfnCompareFunc, void* pUserData)
{
    size_t columnIndex = GetColumnIndex(columnId);
    ASSERT(columnIndex != Box::InvalidIndex);
    if (columnIndex == Box::InvalidIndex) {
        return false;
    }
    m_nSortedColumnId = columnId;
    m_bSortedUp = bSortedUp;
    SetColumnSortFlagById(columnId, nSortFlag);
    if (m_pHeaderCtrl != nullptr) {
        //更新UI排序显示
        m_pHeaderCtrl->SetSortColumnId(columnId, bSortedUp, false);
    }    
    return m_pData->SortDataItems(columnId, columnIndex, bSortedUp, nSortFlag, pfnCompareFunc, pUserData);
}

void ListCtrl::SetSortCompareFunction(ListCtrlDataCompareFunc pfnCompareFunc, void* pUserData)
{
    m_pData->SetSortCompareFunction(pfnCompareFunc, pUserData);
}

void ListCtrl::SetColumnSortFlag(size_t columnIndex, uint8_t nSortFlag)
{
    SetColumnSortFlagById(GetColumnId(columnIndex), nSortFlag);
}

uint8_t ListCtrl::GetColumnSortFlag(size_t columnIndex)
{
    return GetColumnSortFlagById(GetColumnId(columnIndex));
}

void ListCtrl::SetColumnSortFlagById(size_t columnId, uint8_t nSortFlag)
{
    if (IsValidColumnId(columnId)) {
        m_columnSortFlagMap[columnId] = nSortFlag;
    }
}

uint8_t ListCtrl::GetColumnSortFlagById(size_t columnId)
{
    auto iter = m_columnSortFlagMap.find(columnId);
    if (iter != m_columnSortFlagMap.end()) {
        return iter->second;
    }
    return ListCtrlSubItemSortFlag::kDefault;
}

void ListCtrl::OnHeaderColumnOrderChanged()
{
    Refresh();
}

void ListCtrl::OnHeaderColumnCheckStateChanged(size_t nColumnId, bool bChecked)
{
    //界面状态变化，同步到底层存储
    m_pData->SetColumnCheck(nColumnId, bChecked, true);
}

void ListCtrl::OnHeaderCheckStateChanged(bool bChecked)
{
    //界面状态变化，同步到底层存储
    m_pData->SetAllDataItemsCheck(bChecked);
}

void ListCtrl::OnHeaderColumnVisibleChanged()
{
    Refresh();
}

void ListCtrl::OnHeaderColumnSplitDoubleClick(ListCtrlHeaderItem* pHeaderItem)
{
    if (!IsEnableColumnWidthAuto()) {
        //功能关闭
        return;
    }
    //自动调整该列的宽度
    if (pHeaderItem != nullptr) {
        size_t nColumnId = pHeaderItem->GetColumnId();
        size_t nColumIndex = GetColumnIndex(nColumnId);
        if (nColumIndex < GetColumnCount()) {
            SetColumnWidthAuto(nColumIndex);
        }
    }
}

void ListCtrl::UpdateHeaderColumnCheckBox(size_t nColumnId)
{
    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return;
    }
    std::vector<size_t> columnIdList;
    if (nColumnId == Box::InvalidIndex) {
        const size_t columnCount = GetColumnCount();
        for (size_t columnIndex = 0; columnIndex < columnCount; ++columnIndex) {
            ListCtrlHeaderItem* pHeaderItem = m_pHeaderCtrl->GetColumn(columnIndex);
            if ((pHeaderItem != nullptr) && (pHeaderItem->IsCheckBoxVisible())) {
                columnIdList.push_back(pHeaderItem->GetColumnId());
            }
        }
    }
    else {
        size_t columnIndex = GetColumnIndex(nColumnId);
        if (columnIndex != Box::InvalidIndex) {
            ListCtrlHeaderItem* pHeaderItem = m_pHeaderCtrl->GetColumn(columnIndex);
            if ((pHeaderItem != nullptr) && (pHeaderItem->IsCheckBoxVisible())){
                columnIdList.push_back(nColumnId);
            }
        }
    }
    for (size_t columnId : columnIdList) {
        bool bChecked = false;
        bool bPartChecked = false;
        m_pData->GetColumnCheckStatus(columnId, bChecked, bPartChecked);
        ListCtrlHeaderItem* pHeaderItem = m_pHeaderCtrl->GetColumnById(columnId);
        if ((pHeaderItem != nullptr) && (pHeaderItem->IsCheckBoxVisible())) {
            pHeaderItem->SetCheckBoxCheck(bChecked, bPartChecked);
        }
    }
}

void ListCtrl::UpdateHeaderCheckBox()
{
    if (!IsDataItemShowCheckBox()) {
        //不显示CheckBox，忽略
        return;
    }

    ASSERT(m_pHeaderCtrl != nullptr);
    if (m_pHeaderCtrl == nullptr) {
        return;
    }
    bool bChecked = false;
    bool bPartChecked = false;
    m_pData->GetDataItemsCheckStatus(bChecked, bPartChecked);
    if ((m_pHeaderCtrl->IsChecked() != bChecked) || 
        (m_pHeaderCtrl->IsPartSelected() != bPartChecked)) {
        bool bOldValue = m_pHeaderCtrl->SetEnableCheckChangeEvent(false);
        m_pHeaderCtrl->SetChecked(bChecked, false);
        m_pHeaderCtrl->SetPartSelected(bPartChecked);
        m_pHeaderCtrl->SetEnableCheckChangeEvent(bOldValue);
    }
}

size_t ListCtrl::GetDataItemCount() const
{
    return m_pData->GetDataItemCount();
}

bool ListCtrl::SetDataItemCount(size_t itemCount)
{
    bool bRet = m_pData->SetDataItemCount(itemCount);
    if (bRet) {
        UpdateHeaderColumnCheckBox(Box::InvalidIndex);
        UpdateHeaderCheckBox();
    }
    return bRet;
}

size_t ListCtrl::AddDataItem(const ListCtrlSubItemData& dataItem)
{
    size_t columnId = GetColumnId(0);
    ASSERT(columnId != Box::InvalidIndex);
    if (columnId == Box::InvalidIndex) {
        return Box::InvalidIndex;
    }
    size_t nItemIndex = m_pData->AddDataItem(columnId, dataItem);
    if (nItemIndex != Box::InvalidIndex) {
        UpdateHeaderColumnCheckBox(Box::InvalidIndex);
        UpdateHeaderCheckBox();
    }    
    return nItemIndex;
}

bool ListCtrl::InsertDataItem(size_t itemIndex, const ListCtrlSubItemData& dataItem)
{
    size_t columnId = GetColumnId(0);
    ASSERT(columnId != Box::InvalidIndex);
    if (columnId == Box::InvalidIndex) {
        return Box::InvalidIndex;
    }
    bool bRet = m_pData->InsertDataItem(itemIndex, columnId, dataItem);
    if (bRet) {
        UpdateHeaderColumnCheckBox(Box::InvalidIndex);
        UpdateHeaderCheckBox();
    }
    return bRet;
}

bool ListCtrl::SetSubItemData(size_t itemIndex, size_t columnIndex, const ListCtrlSubItemData& subItemData)
{
    return SetSubItemDataById(itemIndex, GetColumnId(columnIndex), subItemData);
}

bool ListCtrl::SetSubItemDataById(size_t itemIndex, size_t columnId, const ListCtrlSubItemData& subItemData)
{
    bool bCheckChanged = false;
    bool bRet = m_pData->SetSubItemData(itemIndex, columnId, subItemData, bCheckChanged);
    if (bCheckChanged) {
        UpdateHeaderColumnCheckBox(GetColumnId(itemIndex));
    }
    return bRet;
}

bool ListCtrl::GetSubItemData(size_t itemIndex, size_t columnIndex, ListCtrlSubItemData& subItemData) const
{
    return GetSubItemDataById(itemIndex, GetColumnId(columnIndex), subItemData);
}

bool ListCtrl::GetSubItemDataById(size_t itemIndex, size_t columnId, ListCtrlSubItemData& subItemData) const
{
    return m_pData->GetSubItemData(itemIndex, columnId, subItemData);
}

bool ListCtrl::DeleteDataItem(size_t itemIndex)
{
    bool bRet = m_pData->DeleteDataItem(itemIndex);
    if (bRet) {
        UpdateHeaderColumnCheckBox(Box::InvalidIndex);
        UpdateHeaderCheckBox();
    }
    return bRet;
}

bool ListCtrl::DeleteAllDataItems()
{
    bool bRet = m_pData->DeleteAllDataItems();
    if (bRet) {
        UpdateHeaderColumnCheckBox(Box::InvalidIndex);
        UpdateHeaderCheckBox();
    }
    return bRet;
}

bool ListCtrl::SetDataItemData(size_t itemIndex, const ListCtrlItemData& itemData)
{
    bool bChanged = false;
    bool bCheckChanged = false;
    bool bRet = m_pData->SetDataItemData(itemIndex, itemData, bChanged, bCheckChanged);
    if (bRet && bCheckChanged) {
        UpdateHeaderCheckBox();
    }
    return bRet;
}

bool ListCtrl::GetDataItemData(size_t itemIndex, ListCtrlItemData& itemData) const
{
    return m_pData->GetDataItemData(itemIndex, itemData);
}

bool ListCtrl::SetDataItemUserData(size_t itemIndex, size_t userData)
{
    return m_pData->SetDataItemUserData(itemIndex, userData);
}

size_t ListCtrl::GetDataItemUserData(size_t itemIndex) const
{
    return m_pData->GetDataItemUserData(itemIndex);
}

bool ListCtrl::SetDataItemVisible(size_t itemIndex, bool bVisible)
{
    bool bChanged = false;
    bool bRet = m_pData->SetDataItemVisible(itemIndex, bVisible, bChanged);
    if (bChanged) {
        UpdateHeaderColumnCheckBox(Box::InvalidIndex);
        UpdateHeaderCheckBox();
    }
    return bRet;
}

bool ListCtrl::IsDataItemVisible(size_t itemIndex) const
{
    return m_pData->IsDataItemVisible(itemIndex);
}

bool ListCtrl::SetDataItemSelected(size_t itemIndex, bool bSelected)
{
    bool bChanged = false;
    bool bOldChecked = m_pData->IsDataItemChecked(itemIndex);
    bool bRet = m_pData->SetDataItemSelected(itemIndex, bSelected, bChanged);
    if (bChanged) {
        if (m_pData->IsDataItemChecked(itemIndex) != bOldChecked) {
            UpdateHeaderCheckBox();
        }
    }
    return bRet;
}

bool ListCtrl::IsDataItemSelected(size_t itemIndex) const
{
    return m_pData->IsDataItemSelected(itemIndex);
}

bool ListCtrl::SetDataItemImageId(size_t itemIndex, int32_t imageId)
{
    bool bChanged = false;
    bool bRet = m_pData->SetDataItemImageId(itemIndex, imageId, bChanged);
    if (bChanged) {
        Refresh();
    }
    return bRet;
}

int32_t ListCtrl::GetDataItemImageId(size_t itemIndex) const
{
    return m_pData->GetDataItemImageId(itemIndex);
}

bool ListCtrl::SetDataItemAlwaysAtTop(size_t itemIndex, int8_t nAlwaysAtTop)
{
    bool bChanged = false;
    bool bRet = m_pData->SetDataItemAlwaysAtTop(itemIndex, nAlwaysAtTop, bChanged);
    if (bChanged) {
        Refresh();
    }
    return bRet;
}

int8_t ListCtrl::GetDataItemAlwaysAtTop(size_t itemIndex) const
{
    return m_pData->GetDataItemAlwaysAtTop(itemIndex);
}

bool ListCtrl::SetDataItemHeight(size_t itemIndex, int32_t nItemHeight, bool bNeedDpiScale)
{
    bool bChanged = false;
    if (nItemHeight < 0) {
        nItemHeight = 0;
    }
    if (bNeedDpiScale) {
        Dpi().ScaleInt(nItemHeight);
    }
    bool bRet = m_pData->SetDataItemHeight(itemIndex, nItemHeight, bChanged);
    if (bChanged) {
        Refresh();
    }
    return bRet;
}

int32_t ListCtrl::GetDataItemHeight(size_t itemIndex) const
{
    return m_pData->GetDataItemHeight(itemIndex);
}

bool ListCtrl::SetSubItemText(size_t itemIndex, size_t columnIndex, const DString& text)
{
    return SetSubItemTextById(itemIndex, GetColumnId(columnIndex), text);
}

bool ListCtrl::SetSubItemTextById(size_t itemIndex, size_t columnId, const DString& text)
{
    return m_pData->SetSubItemText(itemIndex, columnId, text);
}

DString ListCtrl::GetSubItemText(size_t itemIndex, size_t columnIndex) const
{
    return GetSubItemTextById(itemIndex, GetColumnId(columnIndex));
}

DString ListCtrl::GetSubItemTextById(size_t itemIndex, size_t columnId) const
{
    return m_pData->GetSubItemText(itemIndex, columnId);
}

bool ListCtrl::SetSubItemSortGroup(size_t itemIndex, size_t columnIndex, int32_t nSortGroup)
{
    return SetSubItemSortGroupById(itemIndex, GetColumnId(columnIndex), nSortGroup);
}

bool ListCtrl::SetSubItemSortGroupById(size_t itemIndex, size_t columnId, int32_t nSortGroup)
{
    return m_pData->SetSubItemSortGroup(itemIndex, columnId, nSortGroup);
}

int32_t ListCtrl::GetSubItemGroup(size_t itemIndex, size_t columnIndex) const
{
    return GetSubItemGroupById(itemIndex, GetColumnId(columnIndex));
}

int32_t ListCtrl::GetSubItemGroupById(size_t itemIndex, size_t columnId) const
{
    return m_pData->GetSubItemSortGroup(itemIndex, columnId);
}

bool ListCtrl::SetSubItemUserDataN(size_t itemIndex, size_t columnIndex, uint64_t userDataN)
{
    return SetSubItemUserDataNById(itemIndex, GetColumnId(columnIndex), userDataN);
}

bool ListCtrl::SetSubItemUserDataNById(size_t itemIndex, size_t columnId, uint64_t userDataN)
{
    return m_pData->SetSubItemUserDataN(itemIndex, columnId, userDataN);
}

uint64_t ListCtrl::GetSubItemUserDataN(size_t itemIndex, size_t columnIndex) const
{
    return GetSubItemUserDataNById(itemIndex, GetColumnId(columnIndex));
}

uint64_t ListCtrl::GetSubItemUserDataNById(size_t itemIndex, size_t columnId) const
{
    return m_pData->GetSubItemUserDataN(itemIndex, columnId);
}

bool ListCtrl::SetSubItemUserDataS(size_t itemIndex, size_t columnIndex, const DString& userDataS)
{
    return SetSubItemUserDataSById(itemIndex, GetColumnId(columnIndex), userDataS);
}

bool ListCtrl::SetSubItemUserDataSById(size_t itemIndex, size_t columnId, const DString& userDataS)
{
    return m_pData->SetSubItemUserDataS(itemIndex, columnId, userDataS);
}

DString ListCtrl::GetSubItemUserDataS(size_t itemIndex, size_t columnIndex) const
{
    return GetSubItemUserDataSById(itemIndex, GetColumnId(columnIndex));
}

DString ListCtrl::GetSubItemUserDataSById(size_t itemIndex, size_t columnId) const
{
    return m_pData->GetSubItemUserDataS(itemIndex, columnId);
}

bool ListCtrl::SetSubItemTextColor(size_t itemIndex, size_t columnIndex, const UiColor& textColor)
{
    return SetSubItemTextColorById(itemIndex, GetColumnId(columnIndex), textColor);
}

bool ListCtrl::SetSubItemTextColorById(size_t itemIndex, size_t columnId, const UiColor& textColor)
{
    return m_pData->SetSubItemTextColor(itemIndex, columnId, textColor);
}

UiColor ListCtrl::GetSubItemTextColor(size_t itemIndex, size_t columnIndex) const
{
    return GetSubItemTextColorById(itemIndex, GetColumnId(columnIndex));
}

UiColor ListCtrl::GetSubItemTextColorById(size_t itemIndex, size_t columnId) const
{
    UiColor textColor;
    m_pData->GetSubItemTextColor(itemIndex, columnId, textColor);
    return textColor;
}

bool ListCtrl::SetSubItemTextFormat(size_t itemIndex, size_t columnIndex, int32_t nTextFormat)
{
    return SetSubItemTextFormatById(itemIndex, GetColumnId(columnIndex), nTextFormat);
}

bool ListCtrl::SetSubItemTextFormatById(size_t itemIndex, size_t columnId, int32_t nTextFormat)
{
    return m_pData->SetSubItemTextFormat(itemIndex, columnId, nTextFormat);
}

int32_t ListCtrl::GetSubItemTextFormat(size_t itemIndex, size_t columnIndex) const
{
    return GetSubItemTextFormatById(itemIndex, GetColumnId(columnIndex));
}

int32_t ListCtrl::GetSubItemTextFormatById(size_t itemIndex, size_t columnId) const
{
    return m_pData->GetSubItemTextFormat(itemIndex, columnId);
}

bool ListCtrl::SetSubItemBkColor(size_t itemIndex, size_t columnIndex, const UiColor& bkColor)
{
    return SetSubItemBkColorById(itemIndex, GetColumnId(columnIndex), bkColor);
}

bool ListCtrl::SetSubItemBkColorById(size_t itemIndex, size_t columnId, const UiColor& bkColor)
{
    return m_pData->SetSubItemBkColor(itemIndex, columnId, bkColor);
}

UiColor ListCtrl::GetSubItemBkColor(size_t itemIndex, size_t columnIndex) const
{
    return GetSubItemBkColorById(itemIndex, GetColumnId(columnIndex));
}

UiColor ListCtrl::GetSubItemBkColorById(size_t itemIndex, size_t columnId) const
{
    UiColor bkColor;
    m_pData->GetSubItemBkColor(itemIndex, columnId, bkColor);
    return bkColor;
}

bool ListCtrl::IsSubItemShowCheckBox(size_t itemIndex, size_t columnIndex) const
{
    return IsSubItemShowCheckBoxById(itemIndex, GetColumnId(columnIndex));
}

bool ListCtrl::IsSubItemShowCheckBoxById(size_t itemIndex, size_t columnId) const
{
    return m_pData->IsSubItemShowCheckBox(itemIndex, columnId);
}

bool ListCtrl::SetSubItemShowCheckBox(size_t itemIndex, size_t columnIndex, bool bShowCheckBox)
{
    return SetSubItemShowCheckBoxById(itemIndex, GetColumnId(columnIndex), bShowCheckBox);
}

bool ListCtrl::SetSubItemShowCheckBoxById(size_t itemIndex, size_t columnId, bool bShowCheckBox)
{
    return m_pData->SetSubItemShowCheckBox(itemIndex, columnId, bShowCheckBox);
}

bool ListCtrl::SetSubItemCheck(size_t itemIndex, size_t columnIndex, bool bChecked)
{
    return SetSubItemCheckById(itemIndex, GetColumnId(columnIndex), bChecked);
}

bool ListCtrl::SetSubItemCheckById(size_t itemIndex, size_t columnId, bool bChecked)
{
    return m_pData->SetSubItemCheck(itemIndex, columnId, bChecked, true);
}

bool ListCtrl::IsSubItemChecked(size_t itemIndex, size_t columnIndex) const
{
    return IsSubItemCheckedById(itemIndex, GetColumnId(columnIndex));
}

bool ListCtrl::IsSubItemCheckedById(size_t itemIndex, size_t columnId) const
{
    bool bChecked = false;
    m_pData->GetSubItemCheck(itemIndex, columnId, bChecked);
    return bChecked;
}

bool ListCtrl::SetSubItemImageId(size_t itemIndex, size_t columnIndex, int32_t imageId)
{
    return SetSubItemImageIdById(itemIndex, GetColumnId(columnIndex), imageId);
}

bool ListCtrl::SetSubItemImageIdById(size_t itemIndex, size_t columnId, int32_t imageId)
{
    return m_pData->SetSubItemImageId(itemIndex, columnId, imageId);
}

int32_t ListCtrl::GetSubItemImageId(size_t itemIndex, size_t columnIndex) const
{
    return GetSubItemImageIdById(itemIndex, GetColumnId(columnIndex));
}

int32_t ListCtrl::GetSubItemImageIdById(size_t itemIndex, size_t columnId) const
{
    return m_pData->GetSubItemImageId(itemIndex, columnId);
}

bool ListCtrl::SetSubItemEditable(size_t itemIndex, size_t columnIndex, bool bEditable)
{
    return SetSubItemEditableById(itemIndex, GetColumnId(columnIndex), bEditable);
}

bool ListCtrl::SetSubItemEditableById(size_t itemIndex, size_t columnId, bool bEditable)
{
    return m_pData->SetSubItemEditable(itemIndex, columnId, bEditable);
}

bool ListCtrl::IsSubItemEditable(size_t itemIndex, size_t columnIndex) const
{
    return IsSubItemEditableById(itemIndex, GetColumnId(columnIndex));
}

bool ListCtrl::IsSubItemEditableById(size_t itemIndex, size_t columnId) const
{
    return m_pData->IsSubItemEditable(itemIndex, columnId);
}

bool ListCtrl::IsMultiSelect() const
{
    return m_bMultiSelect;
}

void ListCtrl::SetMultiSelect(bool bMultiSelect)
{
    m_bMultiSelect = bMultiSelect;
    if (m_pReportView != nullptr) {
        m_pReportView->SetMultiSelect(bMultiSelect);
    }
    if (m_pIconView != nullptr) {
        m_pIconView->SetMultiSelect(bMultiSelect);
    }
    if (m_pListView != nullptr) {
        m_pListView->SetMultiSelect(bMultiSelect);
    }
    UpdateHeaderCheckBox();
}

void ListCtrl::SetSelectedDataItems(const std::vector<size_t>& selectedIndexs, bool bClearOthers)
{
    std::vector<size_t> refreshIndexs;
    m_pData->SetSelectedElements(selectedIndexs, bClearOthers, refreshIndexs);
    if (!refreshIndexs.empty()) {
        RefreshDataItems(refreshIndexs);
        UpdateHeaderCheckBox();
    }
}

void ListCtrl::GetSelectedDataItems(std::vector<size_t>& itemIndexs) const
{
    m_pData->GetSelectedElements(itemIndexs);
}

void ListCtrl::SetCheckedDataItems(const std::vector<size_t>& itemIndexs, bool bClearOthers)
{
    std::vector<size_t> refreshIndexs;
    m_pData->SetCheckedDataItems(itemIndexs, bClearOthers, refreshIndexs);    
    if (!refreshIndexs.empty()) {
        RefreshDataItems(refreshIndexs);
        UpdateHeaderCheckBox();
    }
}

void ListCtrl::GetCheckedDataItems(std::vector<size_t>& itemIndexs) const
{
    m_pData->GetCheckedDataItems(itemIndexs);
}

void ListCtrl::SetSelectAll()
{
    std::vector<size_t> refreshIndexs;
    m_pData->SelectAll(refreshIndexs);
    if (!refreshIndexs.empty()) {
        RefreshDataItems(refreshIndexs);
        if (IsAutoCheckSelect()) {
            UpdateHeaderCheckBox();
        }
    }    
}

void ListCtrl::SetSelectNone()
{
    std::vector<size_t> refreshIndexs;
    m_pData->SelectNone(refreshIndexs);
    if (!refreshIndexs.empty()) {
        RefreshDataItems(refreshIndexs);
        if (IsAutoCheckSelect()) {
            UpdateHeaderCheckBox();
        }
    }
}

void ListCtrl::GetDisplayDataItems(std::vector<size_t>& itemIndexList) const
{
    itemIndexList.clear();
    if (m_listCtrlType == ListCtrlType::Report) {
        if (m_pReportView != nullptr) {
            m_pReportView->GetDisplayDataItems(itemIndexList);
        }
    }
    else if (m_listCtrlType == ListCtrlType::Icon) {
        if (m_pIconView != nullptr) {
            m_pIconView->GetDisplayDataItems(itemIndexList);
        }
    }
    else if (m_listCtrlType == ListCtrlType::List) {
        if (m_pListView != nullptr) {
            m_pListView->GetDisplayDataItems(itemIndexList);
        }
    }
}

size_t ListCtrl::GetTopDataItem() const
{
    size_t nTopItemIndex = Box::InvalidIndex;
    if (m_listCtrlType == ListCtrlType::Report) {
        if (m_pReportView != nullptr) {
            nTopItemIndex = m_pReportView->GetTopElementIndex();
        }
    }
    else if (m_listCtrlType == ListCtrlType::Icon) {
        if (m_pIconView != nullptr) {
            nTopItemIndex = m_pIconView->GetTopElementIndex();
        }
    }
    else if (m_listCtrlType == ListCtrlType::List) {
        if (m_pListView != nullptr) {
            nTopItemIndex = m_pListView->GetTopElementIndex();
        }
    }
    return nTopItemIndex;
}

bool ListCtrl::IsDataItemDisplay(size_t itemIndex) const
{
    bool bItemVisible = false;
    if (m_listCtrlType == ListCtrlType::Report) {
        if (m_pReportView != nullptr) {
            bItemVisible = m_pReportView->IsDataItemDisplay(itemIndex);
        }
    }
    else if (m_listCtrlType == ListCtrlType::Icon) {
        if (m_pIconView != nullptr) {
            bItemVisible = m_pIconView->IsDataItemDisplay(itemIndex);
        }
    }
    else if (m_listCtrlType == ListCtrlType::List) {
        if (m_pListView != nullptr) {
            bItemVisible = m_pListView->IsDataItemDisplay(itemIndex);
        }
    }
    return bItemVisible;
}

bool ListCtrl::EnsureDataItemVisible(size_t itemIndex, bool bToTop)
{
    bool bRet = false;
    if (m_listCtrlType == ListCtrlType::Report) {
        if (m_pReportView != nullptr) {
            bRet = m_pReportView->EnsureDataItemVisible(itemIndex, bToTop);
        }
    }
    else if (m_listCtrlType == ListCtrlType::Icon) {
        if (m_pIconView != nullptr) {
            bRet = m_pIconView->EnsureDataItemVisible(itemIndex, bToTop);
        }
    }
    else if (m_listCtrlType == ListCtrlType::List) {
        if (m_pListView != nullptr) {
            bRet = m_pListView->EnsureDataItemVisible(itemIndex, bToTop);
        }
    }
    return bRet;
}

void ListCtrl::Refresh()
{
    if (!IsInited()) {
        return;
    }
    if (m_bEnableRefresh) {
        if (m_listCtrlType == ListCtrlType::Report) {
            if (m_pReportView != nullptr) {
                m_pReportView->Refresh();
            }
        }
        else if (m_listCtrlType == ListCtrlType::Icon) {
            if (m_pIconView != nullptr) {
                m_pIconView->Refresh();
            }
        }
        else if (m_listCtrlType == ListCtrlType::List) {
            if (m_pListView != nullptr) {
                m_pListView->Refresh();
            }
        }
    }
}

void ListCtrl::RefreshDataItems(const std::vector<size_t>& dataItemIndexs)
{
    if (m_bEnableRefresh && !dataItemIndexs.empty()) {
        if (m_listCtrlType == ListCtrlType::Report) {
            if (m_pReportView != nullptr) {
                m_pReportView->RefreshElements(dataItemIndexs);
            }
        }
        else if (m_listCtrlType == ListCtrlType::Icon) {
            if (m_pIconView != nullptr) {
                m_pIconView->RefreshElements(dataItemIndexs);
            }
        }
        else if (m_listCtrlType == ListCtrlType::List) {
            if (m_pListView != nullptr) {
                m_pListView->RefreshElements(dataItemIndexs);
            }
        }        
    }
}

bool ListCtrl::SetEnableRefresh(bool bEnable)
{
    bool bOldEnable = m_bEnableRefresh;
    m_bEnableRefresh = bEnable;
    return bOldEnable;
}

bool ListCtrl::IsEnableRefresh() const
{
    return m_bEnableRefresh;
}

void ListCtrl::SetAutoCheckSelect(bool bAutoCheckSelect)
{
    m_bAutoCheckSelect = bAutoCheckSelect;
    m_pData->SetAutoCheckSelect(bAutoCheckSelect);
}

bool ListCtrl::IsAutoCheckSelect() const
{
    return m_bAutoCheckSelect;
}

bool ListCtrl::SetHeaderShowCheckBox(bool bShow)
{
    m_bHeaderShowCheckBox = bShow;
    bool bRet = false;
    if (IsInited()) {
        ListCtrlHeader* pHeaderCtrl = GetHeaderCtrl();
        ASSERT(pHeaderCtrl != nullptr);
        if (pHeaderCtrl != nullptr) {
            bRet = pHeaderCtrl->SetShowCheckBox(bShow);
        }
    }
    else {
        bRet = true;
    }
    return bRet;
}

bool ListCtrl::IsHeaderShowCheckBox() const
{
    bool bRet = false;
    if (IsInited()) {
        ListCtrlHeader* pHeaderCtrl = GetHeaderCtrl();
        ASSERT(pHeaderCtrl != nullptr);
        if (pHeaderCtrl != nullptr) {
            bRet = pHeaderCtrl->IsShowCheckBox();
        }
    }
    else {
        bRet = m_bHeaderShowCheckBox;
    }
    return bRet;
}

void ListCtrl::SetDataItemShowCheckBox(bool bShow)
{
    if (m_bDataItemShowCheckBox != bShow) {
        m_bDataItemShowCheckBox = bShow;
        if (IsInited()) {
            Refresh();
        }
    }
}

bool ListCtrl::IsDataItemShowCheckBox() const
{
    return m_bDataItemShowCheckBox;
}

bool ListCtrl::SetDataItemCheck(size_t itemIndex, bool bCheck)
{
    bool bChanged = false;
    bool bRet = m_pData->SetDataItemChecked(itemIndex, bCheck, bChanged);
    if (bChanged) {
        UpdateHeaderCheckBox();
    }
    return bRet;
}

bool ListCtrl::IsDataItemCheck(size_t itemIndex) const
{
    return m_pData->IsDataItemChecked(itemIndex);
}

void ListCtrl::OnItemEnterEditMode(size_t itemIndex, size_t nColumnId,
                                   IListBoxItem* pItem, ListCtrlLabel* pSubItem)
{
    ASSERT(itemIndex < GetDataItemCount());
    ASSERT(GetColumnIndex(nColumnId) < GetColumnCount());
    ASSERT((pItem != nullptr) && (pSubItem != nullptr));
    ASSERT(pItem->GetElementIndex() == itemIndex);
    if (itemIndex >= GetDataItemCount()) {
        return;
    }
    if (pItem->GetElementIndex() != itemIndex) {
        return;
    }
    size_t nColumnIndex = GetColumnIndex(nColumnId);
    if (nColumnIndex >= GetColumnCount()) {
        return;
    }

    ListCtrlEditParam editParam;
    editParam.listCtrlType = m_listCtrlType;
    editParam.nItemIndex = itemIndex;
    editParam.nColumnId = nColumnId;
    editParam.nColumnIndex = nColumnIndex;
    editParam.pItem = pItem;
    editParam.pSubItem = pSubItem;

    if (!IsValidItemEditState(editParam) && !IsValidItemEditParam(editParam)) {
        ASSERT(0);
        return;
    }

    DString editClass = GetRichEditClass();
    ASSERT(!editClass.empty());
    if (editClass.empty()) {
        return;
    }

    //启动定时器, 只执行一次(使用定时器的原因：避免影响双击操作)
    m_editModeFlag.Cancel();
    std::function<void()> editModeCallback = UiBind(&ListCtrl::OnItemEditMode, this, editParam);
    TimerManager& timer = GlobalManager::Instance().Timer();
    timer.AddTimer(m_editModeFlag.GetWeakFlag(), editModeCallback, 600, 1);
}

bool ListCtrl::IsValidItemEditState(const ListCtrlEditParam& editParam) const
{
    if (!IsEnableItemEdit()) {
        return false;
    }
    if (editParam.listCtrlType != m_listCtrlType) {
        return false;
    }

    if (editParam.listCtrlType == ListCtrlType::Icon) {
        //Icon视图
        ListCtrlIconViewItem* pItem = dynamic_cast<ListCtrlIconViewItem*>(editParam.pItem);
        ASSERT((pItem != nullptr) && pItem->IsVisible() && pItem->IsSelected() && pItem->IsFocused());
        if ((pItem == nullptr) || !pItem->IsVisible() || !pItem->IsSelected() || !pItem->IsFocused()) {
            return false;
        }
        ASSERT(m_pIconView != nullptr);
        if (m_pIconView == nullptr) {
            return false;
        }
    }
    else if (editParam.listCtrlType == ListCtrlType::List) {
        //List视图
        ListCtrlListViewItem* pItem = dynamic_cast<ListCtrlListViewItem*>(editParam.pItem);
        ASSERT((pItem != nullptr) && pItem->IsVisible() && pItem->IsSelected() && pItem->IsFocused());
        if ((pItem == nullptr) || !pItem->IsVisible() || !pItem->IsSelected() || !pItem->IsFocused()) {
            return false;
        }
        ASSERT(m_pListView != nullptr);
        if (m_pListView == nullptr) {
            return false;
        }
    }
    else {
        //Report视图
        ListCtrlItem* pItem = dynamic_cast<ListCtrlItem*>(editParam.pItem);
        ASSERT((pItem != nullptr) && pItem->IsVisible() && pItem->IsSelected() && pItem->IsFocused());
        if ((pItem == nullptr) || !pItem->IsVisible() || !pItem->IsSelected() || !pItem->IsFocused()) {
            return false;
        }
        ASSERT(m_pReportView != nullptr);
        if (m_pReportView == nullptr) {
            return false;
        }
    }
    return true;
}

bool ListCtrl::IsValidItemEditParam(const ListCtrlEditParam& editParam) const
{
    if (editParam.listCtrlType != m_listCtrlType) {
        return false;
    }
    if (editParam.listCtrlType == ListCtrlType::Icon) {
        //Icon视图
        ListCtrlIconViewItem* pItem = dynamic_cast<ListCtrlIconViewItem*>(editParam.pItem);
        ASSERT(pItem != nullptr);
        if (pItem == nullptr) {
            return false;
        }
        size_t nDataItemIndex = editParam.nItemIndex;
        ListCtrlIconViewItem* pDestItem = nullptr;
        ListCtrlIconViewItem* pNextItem = GetFirstDisplayIconItem();
        while (pNextItem != nullptr) {
            if (pNextItem->GetElementIndex() == nDataItemIndex) {
                pDestItem = pNextItem;
                break;
            }
            pNextItem = GetNextDisplayIconItem(pNextItem);
        }
        ASSERT(pDestItem == pItem);
        if (pDestItem != pItem) {
            //已经发生变化
            return false;
        }

        size_t nSubItemIndex = pItem->GetItemIndex(editParam.pSubItem);
        ASSERT(nSubItemIndex < pItem->GetItemCount());
        if (nSubItemIndex >= pItem->GetItemCount()) {
            return false;
        }

        size_t nColumnIndex = editParam.nColumnIndex;
        ASSERT(GetColumnId(nColumnIndex) == editParam.nColumnId);
        if (GetColumnId(nColumnIndex) != editParam.nColumnId) {
            return false;
        }
    }
    else if (editParam.listCtrlType == ListCtrlType::List) {
        //List视图
        ListCtrlListViewItem* pItem = dynamic_cast<ListCtrlListViewItem*>(editParam.pItem);
        ASSERT(pItem != nullptr);
        if (pItem == nullptr) {
            return false;
        }
        size_t nDataItemIndex = editParam.nItemIndex;        
        ListCtrlListViewItem* pDestItem = nullptr;
        ListCtrlListViewItem* pNextItem = GetFirstDisplayListItem();
        while (pNextItem != nullptr) {
            if (pNextItem->GetElementIndex() == nDataItemIndex) {
                pDestItem = pNextItem;
                break;
            }
            pNextItem = GetNextDisplayListItem(pNextItem);
        }
        ASSERT(pDestItem == pItem);
        if (pDestItem != pItem) {
            //已经发生变化
            return false;
        }

        size_t nSubItemIndex = pItem->GetItemIndex(editParam.pSubItem);
        ASSERT(nSubItemIndex < pItem->GetItemCount());
        if (nSubItemIndex >= pItem->GetItemCount()) {
            return false;
        }

        size_t nColumnIndex = editParam.nColumnIndex;
        ASSERT(GetColumnId(nColumnIndex) == editParam.nColumnId);
        if (GetColumnId(nColumnIndex) != editParam.nColumnId) {
            return false;
        }
    }
    else {
        //Report视图
        ListCtrlItem* pItem = dynamic_cast<ListCtrlItem*>(editParam.pItem);
        ListCtrlSubItem* pSubItem = dynamic_cast<ListCtrlSubItem*>(editParam.pSubItem);
        ASSERT((pItem != nullptr) && (pSubItem != nullptr));
        if ((pItem == nullptr) || (pSubItem == nullptr)) {
            return false;
        }
        size_t nDataItemIndex = editParam.nItemIndex;
        size_t nColumnIndex = editParam.nColumnIndex;
        ListCtrlItem* pDestItem = nullptr;
        ListCtrlItem* pNextItem = GetFirstDisplayItem();
        while (pNextItem != nullptr) {
            if (pNextItem->GetElementIndex() == nDataItemIndex) {
                pDestItem = pNextItem;
                break;
            }
            pNextItem = GetNextDisplayItem(pNextItem);
        }
        ASSERT(pDestItem == pItem);
        if (pDestItem != pItem) {
            //已经发生变化
            return false;
        }
        if (pItem->GetSubItemIndex(pSubItem) != nColumnIndex) {
            return false;
        }
        ASSERT(GetColumnId(nColumnIndex) == editParam.nColumnId);
        if (GetColumnId(nColumnIndex) != editParam.nColumnId) {
            return false;
        }
    }
    return true;
}

void ListCtrl::OnItemEditMode(ListCtrlEditParam editParam)
{
    DString editClass = GetRichEditClass();
    ASSERT(!editClass.empty());
    if (editClass.empty()) {
        return;
    }
    if (!IsValidItemEditState(editParam) && !IsValidItemEditParam(editParam)) {
        //状态已经失效
        return;
    }
    if (m_pRichEdit == nullptr) {
        m_pRichEdit = new RichEdit(GetWindow());
        AddItem(m_pRichEdit);
        m_pRichEdit->SetClass(editClass);
    }
    else if(GetItemCount() > 0) {
        //将Edit控件，调整到最后
        size_t nItem = GetItemIndex(m_pRichEdit);
        size_t nLastItemIndex = GetItemCount() - 1;
        if (nItem != nLastItemIndex) {
            SetItemIndex(m_pRichEdit, nLastItemIndex);
        }
    }
    if (m_pRichEdit == nullptr) {
        return;
    }

    ListCtrlLabel* pSubItem = editParam.pSubItem;
    if (pSubItem == nullptr) {
        return;
    }
    size_t nDataItemIndex = editParam.nItemIndex;

    DString sOldItemText = pSubItem->GetText();
    UiRect rcItem = pSubItem->GetTextRect();
    UiPoint offsetPt = pSubItem->GetScrollOffsetInScrollBox();
    rcItem.Offset(-offsetPt.x, -offsetPt.y);
    UiRect rect = GetRect();
    UiMargin rcMargin(rcItem.left - rect.left, rcItem.top - rect.top, 0, 0);
    m_pRichEdit->SetMargin(rcMargin, false);
    m_pRichEdit->SetFloat(true);
    m_pRichEdit->SetVisible(true);
    m_pRichEdit->SetText(sOldItemText);
    m_pRichEdit->SetSelAll();
    UpdateRichEditSize(pSubItem);

    //还原焦点：将编辑框的焦点还原会原来的列表控件
    auto RestoreItemFocus = [this, nDataItemIndex]() {
            if ((m_pRichEdit != nullptr) && m_pRichEdit->IsVisible() && m_pRichEdit->IsFocused()) {
                ListCtrlItem* pDestItem = nullptr;
                ListCtrlItem* pNextItem = GetFirstDisplayItem();
                while (pNextItem != nullptr) {
                    if (pNextItem->GetElementIndex() == nDataItemIndex) {
                        pDestItem = pNextItem;
                        break;
                    }
                    pNextItem = GetNextDisplayItem(pNextItem);
                }
                if (pDestItem != nullptr) {
                    pDestItem->SetFocus();
                }
            }
        };

    //触发事件：开始编辑
    ListCtrlEditParam enterEditParam = editParam;
    enterEditParam.sNewText.clear();
    enterEditParam.bCancelled = false;
    SendEvent(kEventEnterEdit, (WPARAM)&enterEditParam);
    if (enterEditParam.bCancelled || !IsValidItemEditState(editParam) || !IsValidItemEditParam(editParam)) {
        //状态已经失效, 或者用户取消编辑
        ClearEditEvents();
        RestoreItemFocus();
        LeaveEditMode();
        return;
    }

    m_pRichEdit->SetFloat(true);
    m_pRichEdit->SetVisible(true);
    m_pRichEdit->SetFocus();

    //文本变化的时候，自动调整编辑框的大小
    m_pRichEdit->DetachEvent(kEventTextChange);
    m_pRichEdit->AttachTextChange([this, pSubItem](const EventArgs&) {
        UpdateRichEditSize(pSubItem);
        return true;
        });

    //编辑结束的时候，触发事件
    auto OnLeaveRichEdit = [this, sOldItemText, RestoreItemFocus, editParam]() {
        DString sNewItemText;
        if ((m_pRichEdit != nullptr) && m_pRichEdit->IsVisible()) {
            sNewItemText = m_pRichEdit->GetText();
            m_pRichEdit->SetVisible(false);                     
        }
        if ((sNewItemText != sOldItemText) && IsValidItemEditParam(editParam)) {
            //文本内容发生变化
            OnItemEdited(editParam, sNewItemText);
        }
        };

    //按回车：应用修改
    m_pRichEdit->DetachEvent(kEventReturn);
    m_pRichEdit->AttachReturn([this, RestoreItemFocus, OnLeaveRichEdit](const EventArgs&) {
        ClearEditEvents();
        RestoreItemFocus();
        OnLeaveRichEdit();
        return false;
        });
    //按ESC键：取消修改
    m_pRichEdit->DetachEvent(kEventKeyDown);
    m_pRichEdit->AttachEvent(kEventKeyDown, [this, RestoreItemFocus](const EventArgs& msg) {
        if ((msg.eventType == kEventKeyDown) && (msg.vkCode == kVK_ESCAPE)) {
            ClearEditEvents();
            RestoreItemFocus();
            if ((m_pRichEdit != nullptr) && m_pRichEdit->IsVisible()) {
                m_pRichEdit->SetVisible(false);
            }
            return false;
        }
        else {
            return true;
        }
        });

    //控件失去焦点: 应用修改
    m_pRichEdit->DetachEvent(kEventKillFocus);
    m_pRichEdit->AttachKillFocus([this, RestoreItemFocus, OnLeaveRichEdit](const EventArgs&) {
        ClearEditEvents();
        RestoreItemFocus();
        OnLeaveRichEdit();
        return false;
        });

    //窗口失去焦点: 应用修改
    m_pRichEdit->DetachEvent(kEventWindowKillFocus);
    m_pRichEdit->AttachWindowKillFocus([this, RestoreItemFocus, OnLeaveRichEdit](const EventArgs&) {
        ClearEditEvents();
        RestoreItemFocus();
        OnLeaveRichEdit();
        return false;
        });
}

void ListCtrl::UpdateRichEditSize(ListCtrlLabel* pSubItem)
{
    if ((pSubItem == nullptr) || (m_pRichEdit == nullptr)) {
        return;
    }
    UiRect rcItem = pSubItem->GetTextRect();
    UiPoint offsetPt = pSubItem->GetScrollOffsetInScrollBox();
    rcItem.Offset(-offsetPt.x, -offsetPt.y);
    UiRect rect = GetRect();
    bool bSingleLine = pSubItem->IsSingleLine();
    if (bSingleLine) {
        if (m_pRichEdit->IsMultiLine()) {
            m_pRichEdit->SetMultiLine(false);
        }
        m_pRichEdit->SetFixedHeight(UiFixedInt::MakeAuto(), false, false);
        m_pRichEdit->SetFixedWidth(UiFixedInt::MakeAuto(), false, false);
        UiEstSize sz = m_pRichEdit->EstimateSize(UiSize(0, 0));
        //设置宽度
        m_pRichEdit->SetFixedWidth(UiFixedInt(sz.cx.GetInt32()), true, false);

        //设置高度
        const int32_t nEditHeight = sz.cy.GetInt32() + sz.cy.GetInt32() / 2;
        m_pRichEdit->SetFixedHeight(UiFixedInt(nEditHeight), true, false);
        m_pRichEdit->SetMinHeight(nEditHeight, false);

        //设置文本框所在位置和大小限制
        uint32_t textStyle = pSubItem->GetTextStyle();
        if (textStyle & TEXT_VCENTER) {
            //CENTER对齐
            rcItem.top = rcItem.CenterY() - nEditHeight / 2;
            rcItem.bottom = rcItem.top + nEditHeight;
        }
        else if (textStyle & TEXT_BOTTOM) {
            //BOTTOM对齐
            rcItem.top = rcItem.bottom - nEditHeight;
        }

        UiMargin rcMargin(rcItem.left - rect.left, rcItem.top - rect.top, 0, 0);
        m_pRichEdit->SetMargin(rcMargin, false);

        m_pRichEdit->SetMinWidth(rcItem.Width(), false);
        int32_t nMaxWidth = std::max(rcItem.Width(), rect.Width() - rcMargin.left);
        m_pRichEdit->SetMaxWidth(nMaxWidth, false);
    }
    else {
        if (!m_pRichEdit->IsMultiLine()) {
            m_pRichEdit->SetMultiLine(true);
        }

        //宽度固定：与原控件相同
        m_pRichEdit->SetFixedWidth(UiFixedInt(rcItem.Width()), false, false);
        m_pRichEdit->SetMaxWidth(rcItem.Width(), false);
        m_pRichEdit->SetMinWidth(rcItem.Width(), false);
        
        //设置高度和位置
        m_pRichEdit->SetFixedHeight(UiFixedInt::MakeAuto(), false, false);        
        UiEstSize sz = m_pRichEdit->EstimateSize(UiSize(rcItem.Width(), 0));
        m_pRichEdit->SetFixedHeight(UiFixedInt(sz.cy.GetInt32()), true, false);

        UiMargin rcMargin(rcItem.left - rect.left, rcItem.top - rect.top, 0, 0);
        m_pRichEdit->SetMargin(rcMargin, false);

        int32_t nMaxHeight = std::max(rcItem.Height(), rect.Height() - rcMargin.top);
        m_pRichEdit->SetMaxHeight(nMaxHeight, false);
        m_pRichEdit->SetMinHeight(rcItem.Height(), false);
    }
}

void ListCtrl::OnItemEdited(const ListCtrlEditParam& editParam, const DString& newItemText)
{
    //触发事件：结束编辑，如果用户取消编辑，那么不执行修改操作
    ListCtrlEditParam leaveEditParam = editParam;
    leaveEditParam.sNewText = newItemText;
    leaveEditParam.bCancelled = false;
    SendEvent(kEventLeaveEdit, (WPARAM)&leaveEditParam);
    if (!leaveEditParam.bCancelled) {
        //用户未取消编辑
        SetSubItemText(editParam.nItemIndex, editParam.nColumnIndex, newItemText);
    }    
}

void ListCtrl::OnViewMouseEvents(const EventArgs& msg)
{
    if ((msg.eventType == kEventMouseWheel) ||
        (msg.eventType == kEventMouseButtonDown) ||
        (msg.eventType == kEventMouseButtonUp) ||
        (msg.eventType == kEventMouseDoubleClick) ||
        (msg.eventType == kEventMouseRButtonDown) ||
        (msg.eventType == kEventMouseRButtonUp) ||
        (msg.eventType == kEventMouseRDoubleClick) ||
        (msg.eventType == kEventContextMenu)) {
        if (msg.GetSender() != m_pRichEdit) {
            LeaveEditMode();
        }
    }    
}

void ListCtrl::OnViewKeyboardEvents(const EventArgs& msg)
{
    if (msg.GetSender() != m_pRichEdit) {
        LeaveEditMode();
    }    
}

void ListCtrl::LeaveEditMode()
{
    //取消编辑状态的定时器
    m_editModeFlag.Cancel();
    if ((m_pRichEdit != nullptr) && m_pRichEdit->IsVisible() && m_pRichEdit->IsFocused()) {
        m_pRichEdit->SendEvent(kEventKillFocus);
    }
    if ((m_pRichEdit != nullptr) && m_pRichEdit->IsVisible()) {
        m_pRichEdit->SetVisible(false);
    }
    ClearEditEvents();
}

void ListCtrl::ClearEditEvents()
{
    if (m_pRichEdit != nullptr) {
        m_pRichEdit->DetachEvent(kEventWindowKillFocus);
        m_pRichEdit->DetachEvent(kEventKillFocus);
        m_pRichEdit->DetachEvent(kEventReturn);
        m_pRichEdit->DetachEvent(kEventTextChange);
        m_pRichEdit->DetachEvent(kEventKeyDown);
    }
}

}//namespace ui

