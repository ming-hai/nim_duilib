#ifndef UI_RENDER_GDIPLUS_PEN_H_
#define UI_RENDER_GDIPLUS_PEN_H_

#pragma once

#include "duilib/Render/IRender.h"

namespace Gdiplus
{
	class Pen;
}

namespace ui 
{

class UILIB_API Pen_GdiPlus : public IPen
{
public:
	explicit Pen_GdiPlus(UiColor color, int width = 1);
	Pen_GdiPlus(const Pen_GdiPlus& r);
	Pen_GdiPlus& operator=(const Pen_GdiPlus& r) = delete;

	virtual IPen* Clone() override;

	virtual void SetWidth(int width) override;
	virtual int GetWidth() override;
	virtual void SetColor(UiColor color) override;

	virtual void SetStartCap(LineCap cap) override;
	virtual void SetEndCap(LineCap cap) override;
	virtual void SetDashCap(LineCap cap) override;
	virtual LineCap GetStartCap() override;
	virtual LineCap GetEndCap() override;
	virtual LineCap GetDashCap() override;

	virtual void SetLineJoin(LineJoin join) override;
	virtual LineJoin GetLineJoin() override;

	virtual void SetDashStyle(DashStyle style) override;
	virtual DashStyle GetDashStyle() override;

	Gdiplus::Pen* GetPen() const;
private:
	std::unique_ptr<Gdiplus::Pen> pen_;
};

} // namespace ui

#endif // UI_RENDER_GDIPLUS_PEN_H_