#include "Sudoku.h"
#include <assert.h>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace std;

Sudoku::Sudoku()
{
}

void Sudoku::InitSudoku(Assign initSudoku[])
{
    for(int8_t i = 0; i < 9; ++i)
    {
        for(int8_t j = 0; j < 9; ++j)
        {
            sudokuElem_[i][j].SetValue(0); // = 0;
            for(int8_t may = 1; may <= 9; ++may)
            {
                sudokuElem_[i][j].PushCacheBack(may);
            }
        }
    }

    for(Assign* pInit = initSudoku; NotEndList(pInit); ++pInit)
    {
        sudokuElem_[pInit->x][pInit->y].SetValue( pInit->v );
        sudokuElem_[pInit->x][pInit->y].RemoveAllCache();
        assert(sudokuElem_[pInit->x][pInit->y].CacheNum() == 0);
    }
}


static boost::property_tree::ptree Ptree_;
void Sudoku::InitFromJson(const std::string& filename)
{
    boost::property_tree::read_json(filename, Ptree_);
    problemType_ = Ptree_.get<char>("problemType",'S');
    boost::property_tree::ptree input;
    input = Ptree_.get_child("input");

    int8_t i = 0;
    for(boost::property_tree::ptree::iterator iti = input.begin();
        iti != input.end(); ++iti)
    {
        int8_t j = 0;
        for(boost::property_tree::ptree::iterator __iti = iti->second.begin();
            __iti != iti->second.end(); ++__iti)
        {
            sudokuElem_[i][j].SetValue(__iti->second.get_value<int8_t>());
            if(sudokuElem_[i][j].GetValue() == 0)
            {
                for(int8_t may = 1; may <= 9; ++may)
                {
                    sudokuElem_[i][j].PushCacheBack(may);
                }
            }
            assert(j >= 0 && j < 9);
            ++j;
        }
        assert(i >= 0 && i < 9);
        ++i;
    }
}

int8_t Sudoku::GetCandidateNum(int8_t x, int8_t y)
{
    if(sudokuElem_[x][y].GetValue() > 0)
        return 1;
    return sudokuElem_[x][y].CacheNum();
}

bool Sudoku::UpdateSudoku(BlockingQueue<SudokuPtr> *bq)
{
    int8_t MinX,MinY,MinC;
    int8_t MaxX,MaxY,MaxC;
    bool next = false;
    int8_t tpCand;
    MinX = MinY = MaxX = MaxY = 0;
    do
    {
        next = false;
        MinC = 9;
        MaxC = 1;
        for(int8_t i = 0; i < 9; ++i)
        {
            for(int8_t j = 0; j < 9; ++j)
            {
                int CandidateNum = GetCandidateNum(i,j);
                if(CandidateNum > 1)
                {
                    int8_t tpNum;
                    tpNum = NineRestrict(i,j);

                    if(tpNum > 0)
                        tpNum = RowRestrict(i,j);

                    if(tpNum > 0)
                        tpNum = ColRestrict(i,j);

                    if(tpNum > 0 && problemType_ == 'X')
                        tpNum = XRestrict(i,j);

                    if(tpNum > 0 && problemType_ == 'P')
                        tpNum = PercentumRestrict(i,j);

                    if(tpNum > 0 && problemType_ == 'U')
                        tpNum = SuperRestrict(i,j);

                    if(tpNum > 0 && problemType_ == 'C')
                        tpNum = ColorRestrict(i,j);

                    if(tpNum == 0)
                    {
                        return false;
                    }

                    if(tpNum == 1)
                    {
                        sudokuElem_[i][j].SetValue(sudokuElem_[i][j].PopCacheFront());
                        assert(sudokuElem_[i][j].CacheNum() == 0);
                    }


                    tpCand = GetCandidateNum(i,j);

                    if(tpCand < CandidateNum)
                        next = true;

                    if(tpCand < MinC && tpCand > 1)
                    {
                        MinC = tpCand;
                        MinX = i;
                        MinY = j;
                    }

                    if(tpCand > MaxC)
                    {
                        MaxC = tpCand;
                        MaxX = i;
                        MaxY = j;
                    }
                }
            }
        }
    } while(next);

    if(MaxC > 1)
    {
        assert(MinC > 1 && MinC < 9);
        while(sudokuElem_[MinX][MinY].CacheNum())
        {
            SudokuPtr _tpSudokuPtr(new Sudoku(*this));
            _tpSudokuPtr->sudokuElem_[MinX][MinY].SetValue(
                        _tpSudokuPtr->sudokuElem_[MinX][MinY].PopCacheFront());
            _tpSudokuPtr->sudokuElem_[MinX][MinY].RemoveAllCache();

            bq->put(_tpSudokuPtr);

            (void)sudokuElem_[MinX][MinY].PopCacheFront();
        }
    }
    else if(MaxC == 1 && MinC == 9)
    {
        Print();
    }

    return true;
}


int8_t Sudoku::NineRestrict(int8_t x, int8_t y)
{
    assert(sudokuElem_[x][y].CacheNum() > 1);
    int8_t rS = x - x % 3;
    int8_t lS = y - y % 3;

    for(int8_t i = rS; i < rS + 3; ++i)
    {
        for(int8_t j = lS; j < lS + 3; ++j)
        {
            if(i == x && j == y)
                continue;
            if(sudokuElem_[i][j].GetValue() != 0)
            {
                assert(sudokuElem_[i][j].CacheNum() == 0);
                sudokuElem_[x][y].RemoveFromCache(sudokuElem_[i][j].GetValue());
            }
        }
    }

    return sudokuElem_[x][y].CacheNum();
}


int8_t Sudoku::RowRestrict(int8_t x, int8_t y)
{
    for(int8_t j = 0; j < 9; ++j)
    {
        if(j == y)
            continue;
        if(sudokuElem_[x][j].GetValue() != 0)
        {
            assert(sudokuElem_[x][j].CacheNum() == 0);
            sudokuElem_[x][y].RemoveFromCache(sudokuElem_[x][j].GetValue());
        }
    }

    return sudokuElem_[x][y].CacheNum();
}

int8_t Sudoku::ColRestrict(int8_t x, int8_t y)
{
    for(int8_t i = 0; i < 9; ++i)
    {
        if(i == x)
            continue;
        if(sudokuElem_[i][y].GetValue() != 0)
        {
            assert(sudokuElem_[i][y].CacheNum() == 0);
            sudokuElem_[x][y].RemoveFromCache(sudokuElem_[i][y].GetValue());
        }
    }

    return sudokuElem_[x][y].CacheNum();
}

int8_t Sudoku::XRestrict(int8_t x, int8_t y)
{
    if(x == y)
    {
        for(int8_t i = 0; i < 9; ++i)
        {
            if(x == i)
                continue;
            if(sudokuElem_[i][i].GetValue() != 0)
            {
                assert(sudokuElem_[i][i].CacheNum() == 0);
                sudokuElem_[x][y].RemoveFromCache(sudokuElem_[i][i].GetValue());
            }
        }
    }

    if(x + y == 8)
    {
        for(int8_t i = 0; i < 9; ++i)
        {
            if(i == x)
                continue;
            if(sudokuElem_[i][8-i].GetValue() != 0)
            {
                assert(sudokuElem_[i][8-i].CacheNum() == 0);
                sudokuElem_[x][y].RemoveFromCache(sudokuElem_[i][8-i].GetValue());
            }
        }
    }

    return sudokuElem_[x][y].CacheNum();
}


///static int8_t S1 = 1, E1 = 3, S2 = 5, E2 = 7;
enum {
    S1 = 1,
    E1 = 3,
    S2 = 5,
    E2 = 7
};

int8_t Sudoku::PercentumRestrict(int8_t x, int8_t y)
{
    OneNineRestrict<S1,E1,S1,E1>(x,y);
    OneNineRestrict<S2,E2,S2,E2>(x,y);

    if(x + y == 8)
    {
        for(int8_t i = 0; i < 9; ++i)
        {
            if(i == x)
                continue;
            if(sudokuElem_[i][8-i].GetValue() != 0)
            {
                assert(sudokuElem_[i][8-i].CacheNum() == 0);
                sudokuElem_[x][y].RemoveFromCache(sudokuElem_[i][8-i].GetValue());
            }
        }
    }

    return sudokuElem_[x][y].CacheNum();
}


int8_t Sudoku::SuperRestrict(int8_t x, int8_t y)
{
    OneNineRestrict<S1,E1,S1,E1>(x,y);
    OneNineRestrict<S2,E2,S2,E2>(x,y);
    OneNineRestrict<S1,E1,S2,E2>(x,y);
    OneNineRestrict<S2,E2,S1,E1>(x,y);
    return sudokuElem_[x][y].CacheNum();
}

int8_t Sudoku::ColorRestrict(int8_t x, int8_t y)
{
    int8_t _TpX = x % 3, _TpY = y % 3;

    for(int8_t i = 0; i < 9; i += 3)
    {
        for(int8_t j = 0; j < 9; j += 3)
        {
            if(i + _TpX == x && j + _TpY == y)
                continue;
            if(sudokuElem_[i + _TpX][j + _TpY].GetValue() != 0)
            {
                assert(sudokuElem_[i + _TpX][j + _TpY].CacheNum() == 0);
                sudokuElem_[x][y].RemoveFromCache(sudokuElem_[i + _TpX][j + _TpY].GetValue());
            }
        }
    }
    return sudokuElem_[x][y].CacheNum();
}


static boost::mutex outmutex_;
void Sudoku::Print()
{
    boost::mutex::scoped_lock _Lock(outmutex_);
    printf("\n---------------------------------\n");
    for(int8_t i = 0; i < 9; ++i)
    {
        printf("\n");
        for(int8_t j = 0; j < 9; ++j)
        {
            printf("%d  ", sudokuElem_[i][j].GetValue());
        }
        printf("\n");
    }
}


template<int8_t XS, int8_t XE, int8_t YS, int8_t YE>
void Sudoku::OneNineRestrict(int8_t x, int8_t y)
{
    if(x >= XS && x <= XE && y >= YS && y <= YE)
    {
        for(int8_t i = XS; i <= XE; ++i)
        {
            for(int8_t j = YS; j <= YE; ++j)
            {
                if(x == i && y == j)
                    continue;
                if(sudokuElem_[i][j].GetValue() != 0)
                {
                    assert(sudokuElem_[i][j].CacheNum() == 0);
                    sudokuElem_[x][y].RemoveFromCache(sudokuElem_[i][j].GetValue());
                }
            }
        }
    }
}
