#ifndef SUDOKUELEM_H
#define SUDOKUELEM_H

#include "stdint.h"

class SudokuElem
{
public:
    SudokuElem();
    ///SudokuElem(SudokuElem other);
    int8_t CacheNum();
    int8_t PopCacheFront();
    void PushCacheBack(int8_t num);
    void RemoveFromCache(int8_t num);
    void RemoveAllCache();


    int8_t GetValue();
    void SetValue(int8_t value);

private:
    uint16_t internel_;
};

#endif // SUDOKUELEM_H
