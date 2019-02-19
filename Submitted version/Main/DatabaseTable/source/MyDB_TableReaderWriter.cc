
#ifndef TABLE_RW_C
#define TABLE_RW_C

#include <fstream>
#include <MyDB_TableReaderWriter.h>
#include <MyDB_TableRecIterator.h>

#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableReaderWriter.h"

using namespace std;

MyDB_TableReaderWriter::MyDB_TableReaderWriter(MyDB_TablePtr forMe, MyDB_BufferManagerPtr myBuffer) {
    this->targetTablePtr = forMe;
    this->bufferMgrPtr = myBuffer;

    if (targetTablePtr->lastPage() == -1) { /* no pages were ever used */
        targetTablePtr->setLastPage(0);
        targetPageRwPtr = make_shared<MyDB_PageReaderWriter>(*this, 0);
        targetPageRwPtr->clear();
    } else {
        targetPageRwPtr = make_shared<MyDB_PageReaderWriter>(*this, forMe->lastPage());
    }
}

MyDB_PageReaderWriter MyDB_TableReaderWriter::operator[](size_t i) {

    // if i is larger than pages we have so far, create new pages
    while (i > targetTablePtr->lastPage()) {
        targetTablePtr->setLastPage((size_t) targetTablePtr->lastPage() + 1);
        targetPageRwPtr = make_shared<MyDB_PageReaderWriter>(*this, targetTablePtr->lastPage());
        targetPageRwPtr->clear();
    }

    MyDB_PageReaderWriter ithPageRw(*this, i);
    return ithPageRw;
}

MyDB_RecordPtr MyDB_TableReaderWriter::getEmptyRecord() {
    return make_shared<MyDB_Record>(targetTablePtr->getSchema());
}

MyDB_PageReaderWriter MyDB_TableReaderWriter::last() {
    return *targetPageRwPtr.get();
}


void MyDB_TableReaderWriter::append(MyDB_RecordPtr appendMe) {
    /* appending failed means we need a new page */
    if (!targetPageRwPtr->append(appendMe)) {
        this->targetTablePtr->setLastPage((size_t) targetTablePtr->lastPage() + 1);
        targetPageRwPtr = make_shared<MyDB_PageReaderWriter>(*this, targetTablePtr->lastPage());
        targetPageRwPtr->clear();
        targetPageRwPtr->append(appendMe);
    }
}

/**
 * Load data from a text file into this table
 */
void MyDB_TableReaderWriter::loadFromTextFile(string fileName) {
    // clear current table
    this->targetTablePtr->setLastPage(0);
    this->targetPageRwPtr = make_shared<MyDB_PageReaderWriter>(*this, 0);
    this->targetPageRwPtr->clear();

    ifstream file(fileName);
    string line;
    MyDB_RecordPtr tempRec;

    if (file.is_open()) {
        // read each line
        while (getline(file, line)) {
            tempRec = getEmptyRecord();
            tempRec->fromString(line);
            this->append(tempRec);
        }
        file.close();
    } else {
        cout << "[Error] Failed to open file " << fileName << endl;
    }
}

MyDB_RecordIteratorPtr MyDB_TableReaderWriter::getIterator(MyDB_RecordPtr iterateIntoMe) {
    return make_shared<MyDB_TableRecIterator>(*this, iterateIntoMe);
}

void MyDB_TableReaderWriter::writeIntoTextFile(string fileName) {
    ofstream file(fileName);
    if (file.is_open()) {
        MyDB_RecordPtr tempRec = this->getEmptyRecord();
        MyDB_RecordIteratorPtr it = this->getIterator(tempRec);
        while (it->hasNext()) {
            it->getNext(); /* load into tempRec */
            file << tempRec << endl;
        }
    }
    file.close();
}

MyDB_BufferManagerPtr MyDB_TableReaderWriter::getManager() {
    return this->bufferMgrPtr;
}

MyDB_TablePtr MyDB_TableReaderWriter::getTable() {
    return this->targetTablePtr;
}

#endif

