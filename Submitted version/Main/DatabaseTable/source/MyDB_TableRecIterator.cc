//
// Created by xlinush on 2019/2/14.
//

#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableRecIterator.h"


void MyDB_TableRecIterator::getNext() {
    child->getNext();
}

bool MyDB_TableRecIterator::hasNext() {
    if (child->hasNext()) return true;
    if (pageId == targetTablePtr->lastPage()) return false;
    child = parent[++pageId].getIterator(holderRecPtr);
    return this->hasNext();
}

MyDB_TableRecIterator::MyDB_TableRecIterator(MyDB_TableReaderWriter &forParent, MyDB_RecordPtr iterateIntoMe)
        : parent(forParent), holderRecPtr(iterateIntoMe) {
    this->pageId = 0;
    this->targetTablePtr = parent.getTable();
    this->child = parent[pageId].getIterator(holderRecPtr);
}

MyDB_TableRecIterator::~MyDB_TableRecIterator() {

}
