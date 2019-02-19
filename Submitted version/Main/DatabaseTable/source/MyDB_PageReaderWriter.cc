
#ifndef PAGE_RW_C
#define PAGE_RW_C

#include <MyDB_PageReaderWriter.h>
#include <MyDB_PageRecIterator.h>

#include "MyDB_PageReaderWriter.h"

void MyDB_PageReaderWriter::clear() {
    this->header = (PageInfo *) (targetPage->getBytes());
    header->lastByte = sizeof(int) + sizeof(MyDB_PageType);
    this->setType(MyDB_PageType::RegularPage);
    this->targetPage->wroteBytes();
}

MyDB_PageType MyDB_PageReaderWriter::getType() {
    this->header = (PageInfo *) (targetPage->getBytes());
    return header->pageType;
}

MyDB_RecordIteratorPtr MyDB_PageReaderWriter::getIterator(MyDB_RecordPtr iterateIntoMe) {
    return make_shared<MyDB_PageRecIterator>(targetPage, iterateIntoMe);
}

void MyDB_PageReaderWriter::setType(MyDB_PageType toMe) {
    this->header = (PageInfo *) (targetPage->getBytes());
    header->pageType = toMe;
    this->targetPage->wroteBytes();
}

bool MyDB_PageReaderWriter::append(MyDB_RecordPtr appendMe) {
    this->header = (PageInfo *) (targetPage->getBytes());
    size_t recSize = appendMe->getBinarySize();

    /* if no more space, let tableRw add a new page */
    if (recSize > this->pageSize - header->lastByte) {
        return false;
    }

    /* write to page */
    // appendMe->toBinary(header->data + header->lastByte);
    appendMe->toBinary((char *) targetPage->getBytes() + header->lastByte);
    header->lastByte += recSize;
    this->targetPage->wroteBytes();

    return true;
}

MyDB_PageReaderWriter::MyDB_PageReaderWriter(MyDB_TableReaderWriter &parent, long whichPage) {
    this->pageSize = parent.getManager()->getPageSize();
    this->targetPage = parent.getManager()->getPage(parent.getTable(), whichPage);
}

#endif
