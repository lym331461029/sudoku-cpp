#ifndef SUDOKU_H
#define SUDOKU_H

#include <boost/shared_ptr.hpp>
#include <stdint.h>
#include <stdio.h>
#include "SudokuElem.h"

#include "BlockingQueue.h"
#include <string>

typedef struct Assign
{
    int8_t x,y,v;
} Assign;

#define INITLIST(lName, ...) Assign lName[] = { __VA_ARGS__,{0,0,0}}
#define NotEndList(pl) (pl != NULL && pl->v != 0)

class Sudoku;
typedef boost::shared_ptr<Sudoku> SudokuPtr;

class Sudoku
{
public:
    Sudoku();

    int8_t GetCandidateNum(int8_t x, int8_t y);
    void InitSudoku(Assign initSudoku[]);
    void InitFromJson(const std::string& filename);
    bool UpdateSudoku(BlockingQueue<SudokuPtr> *bq);

    int8_t NineRestrict(int8_t x, int8_t y); ///基本九宫格限定
    int8_t RowRestrict(int8_t x, int8_t y);  ///基本行限定
    int8_t ColRestrict(int8_t x, int8_t y);  ///基本列限定
    int8_t XRestrict(int8_t x, int8_t y);    ///X数独限定
    int8_t PercentumRestrict(int8_t x, int8_t y); ///百分比限定
    int8_t SuperRestrict(int8_t x, int8_t y); ///超数独限定
    int8_t ColorRestrict(int8_t x, int8_t y); ///颜色限定
    void Print();

private:
    template<int8_t XS, int8_t XE, int8_t YS, int8_t YE>
    void OneNineRestrict(int8_t x, int8_t y);

private:
    SudokuElem sudokuElem_[9][9];
    char problemType_;
};
#endif // SUDOKU_H
