//
// Created by xlinush on 2019/2/14.
//

#ifndef PAGE_REC_ITERATOR_H
#define PAGE_REC_ITERATOR_H

#include "MyDB_PageHandle.h"
#include "MyDB_Record.h"
#include "MyDB_RecordIterator.h"

using namespace std;

class MyDB_PageRecIterator : public MyDB_RecordIterator {

public:
    void getNext() override;

    bool hasNext() override;

    MyDB_PageRecIterator(MyDB_PageHandle page, MyDB_RecordPtr iterateIntoMe);

    ~MyDB_PageRecIterator();

private:
    // handle of the page to iterate
    MyDB_PageHandle targetPage;

    // pointer to the placeholder record
    MyDB_RecordPtr holderRecPtr;

    // keep track of all visited bytes
    int lastByte;

};

#endif //A2_MYDB_PAGERECITERATOR_H
