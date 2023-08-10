#include "stdafx.h"
#include "item.h"
#include "provider.h"

Item::Item()
:control_img_(nullptr)
{
}


Item::~Item()
= default;

void Item::InitSubControls(const std::wstring& img, const std::wstring& title, size_t nDataIndex)
{
	// ���� Item �µĿؼ�
	if (control_img_ == nullptr)
	{
		control_img_ = dynamic_cast<ui::Control*>(FindSubControl(L"control_img"));
		label_title_ = dynamic_cast<ui::Label*>(FindSubControl(L"label_title"));
		progress_ = dynamic_cast<ui::Progress*>(FindSubControl(L"progress"));
		btn_del_ = dynamic_cast<ui::Button*>(FindSubControl(L"btn_del"));
		// ģ�����������
		nbase::TimeDelta time_delta = nbase::TimeDelta::FromMicroseconds(nbase::Time::Now().ToInternalValue());
		t_time = time_delta.ToMilliseconds();
		progress_->SetValue((double)(time_delta.ToMilliseconds() % 100));
		// ����ͼ�����������
		control_img_->SetBkImage(img);
		// ��ɾ������������
		btn_del_->AttachClick(nbase::Bind(&Item::OnRemove, this, std::placeholders::_1));
	}


	label_title_->SetText(nbase::StringPrintf(L"%s %d%%", title.c_str(), t_time % 100));
	m_nDataIndex = nDataIndex;

}


bool Item::OnRemove(const ui::EventArgs& args)
{
	// ɾ��ʱ��ֻ��ɾ�����ݾͿ����ˣ���Ҫɾ�������ϵ�Ԫ��
	ui::VirtualListBox* pTileBox = dynamic_cast<ui::VirtualListBox*>(GetOwner());
	ASSERT(pTileBox != nullptr);
	if (pTileBox != nullptr) {
		Provider* pProvider = dynamic_cast<Provider*>(pTileBox->GetDataProvider());
		ASSERT(pProvider != nullptr);
		if (pProvider != nullptr) {
			pProvider->RemoveTask(m_nDataIndex);
			return true;
		}
	}
	return false;	
}