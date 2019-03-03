//
// Created by 56862 on 2019/2/24.
//

#ifndef A3_ITERATORCOMPARATOR_H
#define A3_ITERATORCOMPARATOR_H

# include "MyDB_Record.h"
# include "MyDB_TableRecIteratorAlt.h"
# include<iostream>
using namespace std;

// While the given comparator compares records, your priority queue needs a comparator
// that compares record iterators (alt), since your priority queue is made up of those iterators instead of records.
// However, the new comparator should use the bool func comparator for records,
// because we want to find the record iterator with the smallest record
// (so we will use it to ultimately compare records as well).

class IteratorComparator {

public:
    IteratorComparator (function <bool ()> comparatorIn, MyDB_RecordPtr lhsIn,  MyDB_RecordPtr rhsIn) {
        comparator = comparatorIn;
        lhs = lhsIn;
        rhs = rhsIn;
    }

    bool operator() (const MyDB_RecordIteratorAltPtr leftIter, const MyDB_RecordIteratorAltPtr rightIter) const {
        leftIter->getCurrent(lhs);
        rightIter->getCurrent(rhs);
        return !comparator();
    }

private:
    function <bool()> comparator;
    MyDB_RecordPtr lhs, rhs;
};

#endif //A3_ITERATORCOMPARATOR_H

