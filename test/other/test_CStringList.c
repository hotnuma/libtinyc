#include "CStringList.h"
#include "libtest.h"

#include "print.h"

// untested.
//CStringList(CStringList &&list);
//CStringList& operator =(CStringList &&list);
//CStringList(int size);
//~CStringList();

void test_CStringList()
{
    CStringList listA;
    ASSERT(listA.isEmpty() == true);

    CString strA = "a,b,c,d,e,f,g,h,i";
    listA = strA.split(",");
    ASSERT(listA.size() == 9);

    listA.append("j");
    ASSERT(listA.size() == 10);

    // a b c d e f g h i j

    CString *strB = nullptr;

    strB = listA.takeAt(4);
    ASSERT(strB->compare("e") == 0);
    delete strB;

    listA.removeAt(4);
    strA = listA.at(4);
    ASSERT(strA.compare("g") == 0);

    // a b c d g h i j

    strB = listA.takeFirst();
    ASSERT(strB->compare("a") == 0);
    delete strB;

    listA.removeFirst();
    strA = listA.at(0);
    ASSERT(strA.compare("c") == 0);

    strB = listA.takeLast();
    ASSERT(strB->compare("j") == 0);
    delete strB;

    listA.removeLast();
    strA = listA.at(listA.size() - 1);
    ASSERT(strA.compare("h") == 0);

    // c d g h

    listA.insert(1, "z");
    ASSERT(listA[1].compare("z") == 0);
    ASSERT(listA.find("z") == 1);

    // c z d g h

    int size = listA.capacity();
    listA.resize(size + 1);
    ASSERT(listA.capacity() == (size * 2));

    listA.move(3, 1);
    strA = listA.at(1);
    ASSERT(strA.compare("g") == 0);

    // c g z d h

    strA = listA.join(",");
    ASSERT(strA.compare("c,g,z,d,h") == 0);

    listA.clear();
    ASSERT(listA.size() == 0);

}


