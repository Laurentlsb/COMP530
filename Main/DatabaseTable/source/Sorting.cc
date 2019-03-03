
#ifndef SORT_C
#define SORT_C

#include "MyDB_PageReaderWriter.h"
#include "MyDB_TableRecIterator.h"
#include "MyDB_TableRecIteratorAlt.h"
#include "MyDB_TableReaderWriter.h"
#include "Sorting.h"
#include <queue>
#include "IteratorComparator.h"

using namespace std;

void mergeIntoFile (MyDB_TableReaderWriter &sortIntoMe, vector <MyDB_RecordIteratorAltPtr> &mergeUs,
                    function <bool ()> comparator, MyDB_RecordPtr lhs, MyDB_RecordPtr rhs) {

    IteratorComparator myComparator(comparator, lhs, rhs);
    priority_queue <MyDB_RecordIteratorAltPtr, vector<MyDB_RecordIteratorAltPtr>, IteratorComparator> pq (myComparator);

    // put all iterators into priority queue
    for (int i=0; i<mergeUs.size(); i++) {
        pq.push(mergeUs[i]);
    }

    // pull out the record with the smallest current record
    while(pq.size() != 0) {
        MyDB_RecordIteratorAltPtr tempIter = pq.top();
        pq.pop();
        tempIter->getCurrent(lhs); //load current content into lhs
        sortIntoMe.append(lhs);

        if (tempIter->advance())
            pq.push(tempIter);
    }

}


void addRecord (MyDB_BufferManagerPtr parent, MyDB_PageReaderWriter &currentPage,
                vector <MyDB_PageReaderWriter> &returnList, MyDB_RecordPtr appendRec) {
    bool sign;
    sign = currentPage.append(appendRec);
    // if append fails since the page is full, then add to a new page
    if (!sign){
        returnList.push_back(currentPage);
        MyDB_PageReaderWriter newPage (*parent);
        currentPage = newPage;
        currentPage.append(appendRec);
    }
}


vector <MyDB_PageReaderWriter> mergeIntoList (MyDB_BufferManagerPtr parent, MyDB_RecordIteratorAltPtr leftIter,
        MyDB_RecordIteratorAltPtr rightIter, function <bool ()> comparator, MyDB_RecordPtr lhs, MyDB_RecordPtr rhs) {

    MyDB_PageReaderWriterPtr currentPage = make_shared <MyDB_PageReaderWriter> (true, *parent);   // pin or unpin?
    vector <MyDB_PageReaderWriter> resultList;

    bool leftIterIsAvailable = true;
    bool rightIterIsAvailable = true;

    if (!leftIter->advance()){
        while (rightIter->advance()){
            rightIter->getCurrent(rhs);
            addRecord(parent, *currentPage, resultList, rhs);
        }
    }
    else if (!rightIter->advance()){
        leftIter->getCurrent(lhs);  // we've already pass the first advance，so now the first record is in position.
        addRecord(parent, *currentPage, resultList, lhs);
        while (leftIter->advance()){
            leftIter->getCurrent(lhs);
            addRecord(parent, *currentPage, resultList, lhs);
        }

    }
    else {
        // if we go into this branch, which means the first record for both leftIter and rightIter are in position now
        // since the advance() has been called once for each of them
        while (true){
            if (leftIterIsAvailable){
                leftIter->getCurrent(lhs);
                leftIterIsAvailable = false;
            }
            if (rightIterIsAvailable){
                rightIter->getCurrent(rhs);
                rightIterIsAvailable = false;
            }

            if (comparator()){
                addRecord(parent, *currentPage, resultList, lhs);
                leftIterIsAvailable = true;
                // check whether left is empty
                if (!leftIter->advance()){
                    // now rhs still holds a record
                    addRecord(parent, *currentPage, resultList, rhs);
                    while (rightIter->advance()){
                        rightIter->getCurrent(rhs);
                        addRecord(parent, *currentPage, resultList, rhs);
                    }
                    break;
                }
            }
            else {
                addRecord(parent, *currentPage, resultList, rhs);
                rightIterIsAvailable = true;
                // cheak whether right is empty
                if (!rightIter->advance()){
                    // now lhs still holds a record
                    addRecord(parent, *currentPage, resultList, lhs);
                    while (leftIter->advance()){
                        leftIter->getCurrent(lhs);
                        addRecord(parent, *currentPage, resultList, lhs);
                    }
                    break;
                }
            }
        }


    }

    // note: the last page is still out there, we need to add it into vector
    resultList.push_back(*currentPage);

    return resultList;
}



void sort (int runSize, MyDB_TableReaderWriter &sortMe, MyDB_TableReaderWriter &sortIntoMe,
           function <bool ()> comparator, MyDB_RecordPtr lhs, MyDB_RecordPtr rhs) {

    vector<MyDB_RecordIteratorAltPtr> IterToBeMerge;
    queue<MyDB_RecordIteratorAltPtr> curRun;
    MyDB_BufferManagerPtr myBufferMgr = sortMe.getBufferMgr();
    MyDB_RecordIteratorAltPtr temp;


    for(int i=0;i<sortMe.getNumPages();i++){

        temp = sortMe[i].sort(comparator,lhs,rhs)->getIteratorAlt();
        curRun.push(temp);

        // note that the number of pages may not be multiple runSize, the last segment may be less than runSize
        if(curRun.size()==runSize||i==sortMe.getNumPages()-1){
            while(curRun.size()>1){
                auto first = curRun.front();
                curRun.pop();
                auto second = curRun.front();
                curRun.pop();
                auto mergeTmp = mergeIntoList(myBufferMgr,first,second,comparator,lhs,rhs);
                curRun.push(getIteratorAlt(mergeTmp));
            }
            IterToBeMerge.push_back(curRun.front());
            curRun.pop();
        }
    }


    mergeIntoFile (sortIntoMe, IterToBeMerge, comparator, lhs, rhs);
}

#endif
