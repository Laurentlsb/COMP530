
#include "MyDB_PageRecIterator.h"
#include "MyDB_PageReaderWriter.h"


void MyDB_PageRecIterator::getNext() {
    void *pos = this->lastByte + (char *) targetPage->getBytes();
    void *nextPos = this->holderRecPtr->fromBinary(pos);

    this->lastByte += ((char *) nextPos - (char *) pos);
}

bool MyDB_PageRecIterator::hasNext() {
    PageInfo *header = (PageInfo *) ((char *) targetPage->getBytes());
    return lastByte < header->lastByte;
}

MyDB_PageRecIterator::MyDB_PageRecIterator(MyDB_PageHandle page, MyDB_RecordPtr iterateIntoMe) {
    this->targetPage = page;
    this->holderRecPtr = iterateIntoMe;
    this->lastByte = sizeof(int) + sizeof(MyDB_PageType); /* starts from data[0] */
}

MyDB_PageRecIterator::~MyDB_PageRecIterator() {

}
