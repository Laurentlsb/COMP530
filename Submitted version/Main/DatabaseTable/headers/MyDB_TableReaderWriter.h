
#ifndef TABLE_RW_H
#define TABLE_RW_H

#include <memory>
#include <string.h>
#include "MyDB_BufferManager.h"
#include "MyDB_Record.h"
#include "MyDB_RecordIterator.h"
#include "MyDB_Table.h"

// create a smart pointer for the catalog
using namespace std;

class MyDB_PageReaderWriter;

class MyDB_TableReaderWriter;

typedef shared_ptr<MyDB_TableReaderWriter> MyDB_TableReaderWriterPtr;

typedef shared_ptr<MyDB_PageReaderWriter> MyDB_PageReaderWriterPtr;

class MyDB_TableReaderWriter {

public:

    // ANYTHING ELSE YOU NEED HERE

    // getter of target table
    MyDB_TablePtr getTable();

    // getter of buffer manager
    MyDB_BufferManagerPtr getManager();

    // create a table reader/writer for the specified table, using the specified
    // buffer manager
    MyDB_TableReaderWriter(MyDB_TablePtr forMe, MyDB_BufferManagerPtr myBuffer);

    // gets an empty record from this table
    MyDB_RecordPtr getEmptyRecord();

    // append a record to the table
    void append(MyDB_RecordPtr appendMe);

    // return an iterator over this table... each time returnVal->next () is
    // called, the resulting record will be placed into the record pointed to
    // by iterateIntoMe
    MyDB_RecordIteratorPtr getIterator(MyDB_RecordPtr iterateIntoMe);

    // load a text file into this table... overwrites the current contents
    void loadFromTextFile(string fromMe);

    // dump the contents of this table into a text file
    void writeIntoTextFile(string toMe);

    // access the i^th page in this file
    MyDB_PageReaderWriter operator[](size_t i);

    // access the last page in the file
    MyDB_PageReaderWriter last();

private:

    // ANYTHING YOU NEED HERE

    // pointer to the table this table reader/writer operates on
    MyDB_TablePtr targetTablePtr;

    // a copy of the BufferMgrPtr
    MyDB_BufferManagerPtr bufferMgrPtr;

    // pointer to the page r/w
    // always points to the last page of the table (add-only)
    MyDB_PageReaderWriterPtr targetPageRwPtr;

};

#endif
