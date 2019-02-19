# COMP530 A2 2019 Spring



### Members


- Xiaochang Huang (xh24)


- Sibo Li (sl125)


### File Structures (`DatabaseTable`)


```
BufferMgr

+-- headers

|   +-- MyDB_PageReaderWriter.h

|   +-- MyDB_PageRecIterator.h

|   +-- MyDB_RecordIterator.h

|   +-- MyDB_TableReaderWriter.h
|   +-- MyDB_TableRecIterator.h
+-- source

|   +-- MyDB_PageReaderWriter.cc

|   +-- MyDB_PageRecIterator.cc

|   +-- MyDB_TableReaderWriter.cc
|   +-- MyDB_TableRecIterator.cc

```



### Overview
We  completed the record/page management by implementing classes TableReaderWriter and PageReaderWriter, using an iterator to subsequently access the data in pages or files. The architecture of this implementation is kind of like: TableReaderWriter->TableRecIterator->PageReaderWriter->PageRecIterator->Page. To keep track of the changes of pages, we also use a struct called "PageInfo" to record the meta-data.