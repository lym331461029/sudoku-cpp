#include "SudokuElem.h"
#include <assert.h>


SudokuElem::SudokuElem()
{
    internel_ = 0;
}

int8_t SudokuElem::GetValue()
{
    return internel_ >> 12;
}

void SudokuElem::SetValue(int8_t value)
{
    assert(GetValue() == 0);
    assert(value >= 0 && value <= 9);
    uint16_t _Tp = value;
    _Tp = _Tp << 12;
    internel_ |= _Tp;
}


int8_t SudokuElem::CacheNum()
{
    int8_t _Ret = 0;
    for(int8_t i = 0; i < 9; ++i)
    {
        _Ret += (internel_ >> i) & 0X01;
    }
    return _Ret;
}

void SudokuElem::PushCacheBack(int8_t num)
{
    assert(num > 0 && num <= 9);
    internel_ |= ( 0x01 << (num - 1) );
}

void SudokuElem::RemoveFromCache(int8_t num)
{
    assert(num > 0 && num <= 9);
    uint16_t _Tp = ( 0x01 << (num - 1) );
    _Tp = ~_Tp;
    internel_ &= _Tp;
}

int8_t SudokuElem::PopCacheFront()
{
    int8_t num = 1;
    for(; num <= 9; ++num)
    {
        if( (0x01 << (num -1)) & internel_)
        {
            internel_ &= ~(0x01 << (num -1));
            return num;
        }
    }
    return 0;
}

void SudokuElem::RemoveAllCache()
{
    uint16_t _Tp = 511;
    internel_ &= ~_Tp;
}
