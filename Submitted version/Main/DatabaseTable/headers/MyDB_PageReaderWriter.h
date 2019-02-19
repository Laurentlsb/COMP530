
#ifndef PAGE_RW_H
#define PAGE_RW_H

#include "MyDB_PageType.h"
#include "MyDB_TableReaderWriter.h"

typedef shared_ptr<MyDB_PageReaderWriter> MyDB_PageReaderWriterPtr;

struct PageInfo {
    /* other meta data in between */
    MyDB_PageType pageType; // 4 bytes

    int lastByte; // 4 bytes

    /* points to the start of the rest of the page, use data + lastByte to append */
    char data[0];
};

class MyDB_PageReaderWriter {

public:

    // ANY OTHER METHODS YOU WANT HERE

    // Constructor
    MyDB_PageReaderWriter(MyDB_TableReaderWriter &parent, long whichPage);

    // empties out the contents of this page, so that it has no records in it
    // the type of the page is set to MyDB_PageType :: RegularPage
    void clear();

    // return an iterator over this page... each time returnVal->next () is
    // called, the resulting record will be placed into the record pointed to
    // by iterateIntoMe
    MyDB_RecordIteratorPtr getIterator(MyDB_RecordPtr iterateIntoMe);

    // appends a record to this page... return false is the append fails because
    // there is not enough space on the page; otherwise, return true
    bool append(MyDB_RecordPtr appendMe);

    // gets the type of this page... this is just a value from an enumeration
    // that is stored within the page
    MyDB_PageType getType();

    // sets the type of the page
    void setType(MyDB_PageType toMe);

private:

    // ANYTHING ELSE YOU WANT HERE

    // pointer to the target page handle
    MyDB_PageHandle targetPage;

    // page size, used to determine whether we need a new page
    size_t pageSize;

    // PageInfo header, located at the head of a page
    PageInfo *header;

};

#endif
