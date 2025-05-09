#ifndef UI_CORE_UIESTINT_H_
#define UI_CORE_UIESTINT_H_

#include "duilib/duilib_defs.h"
#include <cstdint>

namespace ui
{

/** 控件估算长度（或宽度）数值
*/
class UILIB_API UiEstInt
{
public:
    //数值类型定义
    enum class Type
    {
        Int32   = 0,    //正常32位整型值，固定数值
        Stretch = 1     //拉伸（由外部的容器负责设置具体数值）
    };

    //数值类型
    Type type : 2;

    //具体数值:
    // 当 type 为 Int32 时, 代表为固定数值
    // 当 type 为 Stretch, 取值范围是 (0,100]，代表由父容器按所占百分比分配
    int32_t value : 30;

public:
    /** 构造一个拉伸类型的值
    */
    static UiEstInt MakeStretch()
    {
        UiEstInt estInt;
        estInt.SetStretch();
        return estInt;
    }

    /** 构造一个拉伸类型的值, 并指定值
    */
    static UiEstInt MakeStretch(int32_t iValue)
    {
        UiEstInt estInt;
        estInt.SetStretch(iValue);
        return estInt;
    }

public:
    /** 构造函数, 默认为整型值0
    */
    UiEstInt(): type(Type::Int32), value(0)
    { }

    /** 构造函数, 设置为整型值
    */
    explicit UiEstInt(int32_t iValue) : type(Type::Int32), value(iValue)
    { }

    /** 判断是否为拉伸类型
    */
    bool IsStretch() const { return type == Type::Stretch;}

    /** 判断是否为32位整型值类型
    */
    bool IsInt32() const { return type == Type::Int32; }

    /** 是否为有效值（数值类型，代表长度，必须大于等于零）
    */
    bool IsValid() const
    {
        if (type == Type::Int32) {
            return value >= 0;
        }
        return true;
    }

    /** 赋值为32位整型值
    */
    void SetInt32(int32_t iValue)
    {
        ASSERT(iValue >= 0);
        type = Type::Int32;
        value = iValue >= 0 ? iValue : 0;
    }

    /** 获取32位整型值
    */
    int32_t GetInt32() const
    {
        if (type != Type::Int32) {
            return 0;
        }
        return value >= 0 ? value : 0;
    }

    /** 获取拉伸类型值，有效值(0, 100], 代表百分之几
    */
    int32_t GetStretchPercentValue() const
    {
        if (type != Type::Stretch) {
            return 0;
        }
        if ((value > 0) && (value <= 100)) {
            return value;
        }
        return 100;
    }

    /** 赋值为拉伸类型
    */
    void SetStretch() 
    { 
        type = Type::Stretch; 
        value = 100;
    }

    /** 赋值为拉伸类型，并指定值
    */
    void SetStretch(int32_t iValue)
    {
        type = Type::Stretch;
        if ((iValue > 0) && (iValue <= 100)) {
            value = iValue;
        }
        else {
            value = 100;
        }
    }

    /** 判断是否与另外一个Size相同
    */
    bool Equals(const UiEstInt& dst) const
    {
        if (IsStretch() && dst.IsStretch()) {
            return value == dst.value;
        }
        else if (IsInt32() && dst.IsInt32()) {
            return value == dst.value;
        }
        return false;
    }

    /** 判断两个值是否相等
    */
    friend bool operator == (const UiEstInt& a, const UiEstInt& b)
    {
        return a.Equals(b);
    }

    /** 判断两个值是否不相等
    */
    friend bool operator != (const UiEstInt& a, const UiEstInt& b)
    {
        return !a.Equals(b);
    }
};

}//namespace ui

#endif // UI_CORE_UIESTINT_H_
