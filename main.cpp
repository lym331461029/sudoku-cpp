#include <iostream>
#include "Sudoku.h"
#include "BlockingQueue.h"

using namespace std;

static std::string filename = "./Input.json";

void ExecWork(BlockingQueue<SudokuPtr>* bq)
{
    while(1)
    {
        SudokuPtr _Tp(bq->take());
        _Tp->UpdateSudoku(bq);
    }
}

int main()
{
    ///{X,Y,Value}, X为横坐标(0-8) Y为纵坐标(0-8) Value为(X,Y)处的值，
    /// 如果没有值则不用显式初始化
    /*
    INITLIST(test,
    {0,5,2},
    {0,6,4},
    {1,0,8},
    {1,1,5},
    {1,4,7},
    {1,7,3},
    {2,2,4},
    {2,3,3},
    {2,8,5},
    {3,2,3},
    {4,1,9},
    {4,4,1},
    {5,3,8},
    {5,6,6},
    {6,2,8},
    {6,5,9},
    {6,8,2},
    {7,1,4},
    {7,5,3},
    {7,7,1},
    {8,1,7},
    {8,2,5},
    {8,8,6} );*/

    /*
    INITLIST(test2,
    {1 ,0 ,5 } ,
    {0 ,8 ,7 } ,
    {1 ,4 ,7 } ,
    {1 ,5 ,8 } ,
    {1 ,7 ,3 } ,
    {2 ,5 ,2 } ,
    {2 ,8 ,5 } ,
    {3 ,0 ,9 } ,
    {3 ,4 ,1 } ,
    {3 ,6 ,8 } ,
    {4 ,3 ,3 } ,
    {4 ,4 ,8 } ,
    {5 ,0 ,6 } ,
    {5 ,2 ,8 } ,
    {5 ,7 ,5 } ,
    {5 ,8 ,1 } ,
    {6 ,1 ,4 } ,
    {6 ,3 ,6 } ,
    {7 ,2 ,2 } ,
    {7 ,3 ,4 } ,
    {7 ,5 ,7 } ,
    {7 ,8 ,6 } ,
    {8 ,2 ,6 } );*/

    INITLIST(test3,
    {0 ,7 ,6} ,
    {0 ,8 ,3} ,
    {1 ,2 ,4} ,
    {1 ,3 ,6} ,
    {1 ,6 ,9} ,
    {2 ,1 ,1} ,
    {2 ,5 ,9} ,
    {2 ,8 ,2} ,
    {3 ,3 ,7} ,
    {3 ,6 ,6} ,
    {4 ,3 ,4} ,
    {4 ,5 ,1} ,
    {5 ,2 ,8} ,
    {5 ,5 ,6} ,
    {6 ,0 ,6} ,
    {6 ,3 ,2} ,
    {6 ,7 ,9} ,
    {7 ,2 ,1} ,
    {7 ,5 ,3} ,
    {7 ,6 ,5} ,
    {8 ,0 ,3} ,
    {8 ,1 ,9} );


    ///Sudoku sudoku;
    ///sudoku.InitSudoku(test3);
    ///sudoku.InitFromJson(filename);

    SudokuPtr _Sudoku(new Sudoku);
    ///_Sudoku->InitSudoku(test3);
    _Sudoku->InitFromJson(filename);

    BlockingQueue<SudokuPtr> _SudokuWork;
    boost::thread_group thrgr_;
    for(int i = 0; i < 8; ++i)
    {
        thrgr_.create_thread(boost::bind(&ExecWork, &_SudokuWork));
    }

    _SudokuWork.put(_Sudoku);

    thrgr_.join_all();
    return 0;
}

