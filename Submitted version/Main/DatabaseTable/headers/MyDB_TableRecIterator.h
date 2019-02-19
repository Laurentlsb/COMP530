//
// Created by xlinush on 2019/2/14.
//

#ifndef TABLE_REC_ITERATOR_H
#define TABLE_REC_ITERATOR_H

#include "MyDB_RecordIterator.h"
#include "MyDB_TableReaderWriter.h"

class MyDB_TableRecIterator : public MyDB_RecordIterator {

public:
    void getNext() override;

    bool hasNext() override;

    MyDB_TableRecIterator(MyDB_TableReaderWriter &forParent, MyDB_RecordPtr iterateIntoMe);

    ~MyDB_TableRecIterator();

private:
    // a reference to the tableRw containing this iterator
    MyDB_TableReaderWriter &parent;

    // pointer to the placeholder record
    MyDB_RecordPtr holderRecPtr;

    // actual iterator that scans records
    MyDB_RecordIteratorPtr child;

    // target table to iterate
    MyDB_TablePtr targetTablePtr;

    int pageId;

};

#endif //A2_MYDB_TABLERECITERATOR_H
