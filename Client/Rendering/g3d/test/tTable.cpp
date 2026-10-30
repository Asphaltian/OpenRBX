#include "../include/G3DAll.h"
#include <map>

// TODO: remove the  && (_MSC_VER < 1400)
#if defined(G3D_WIN32) && (_MSC_VER >= 1300)
#   define HAS_HASH_MAP
#endif

#ifdef HAS_HASH_MAP
#   include <hash_map>
#   if defined(_MSC_VER) && (_MSC_VER >= 1400)
        using stdext::hash_map;
#   else
        using std::hash_map;
#   endif
#endif

// For demonstrating VC8 bug
#ifdef HAS_HASH_MAP
hash_map<int, int> x;
#endif

class TableKey : public Hashable {
public:
    int value;
    
    inline bool operator==(const TableKey& other) const {
        return value == other.value;
    }
    
    inline bool operator!=(const TableKey& other) const {
        return value != other.value;
    }

    inline unsigned int hashCode() const {
        return value;
    }
};

class TableKeyB : public Hashable {
public:
    int value;
    
    inline bool operator==(const TableKey& other) const {
        return value == other.value;
    }
    
    inline bool operator!=(const TableKey& other) const {
        return value != other.value;
    }

    inline unsigned int hashCode() const {
        return 0;
    }
};

void testTable() {

    printf("G3D::Table  ");
    // Basic get/set
    {
        Table<int, int> table;
    
        table.set(10, 20);
        table.set(3, 1);
        table.set(1, 4);

        debugAssert(table[10] == 20);
        debugAssert(table[3] == 1);
        debugAssert(table[1] == 4);
        debugAssert(table.containsKey(10));
        debugAssert(!table.containsKey(0));

        debugAssert(table.debugGetDeepestBucketSize() == 1);
    }


    // Test overloaded pointer hashing
    {
        TableKey        x[4];
        x[0].value = 1;
        x[1].value = 2;
        x[2].value = 3;
        x[3].value = 4;
        Table<TableKey*, int> table;

        table.set(x, 10);
        table.set(x + 1, 20);
        table.set(x + 2, 30);
        table.set(x + 3, 40);
        debugAssert(table[x] == 10);
        debugAssert(table[x + 1] == 20);
        debugAssert(table[x + 2] == 30);
        debugAssert(table[x + 3] == 40);

        // Our current implementation should not have collisions in this case.
        debugAssert(table.debugGetDeepestBucketSize() == 1);
    }

    {
        TableKeyB        x[6];
        Table<TableKeyB*, int> table;
        for (int i = 0; i < 6; ++i) {
            x[i].value = i;
            table.set(x + i, i);
        }

        debugAssert(table.size() == 6);
        debugAssert(table.debugGetDeepestBucketSize() == 6);
        debugAssert(table.debugGetNumBuckets() == 10);
    }

    printf("passed\n");
}


template<class K, class V>
void perfTest(const char* description, const K* keys, const V* vals, int M) {
    uint64 tableSet, tableGet, tableRemove;
    uint64 mapSet, mapGet, mapRemove;
#   ifdef HAS_HASH_MAP
    uint64 hashMapSet, hashMapGet, hashMapRemove;
#   endif

    uint64 overhead;

    // Run many times to filter out startup behavior
    for (int j = 0; j < 3; ++j) {

        // There's a little overhead just for the loop and reading 
        // the values from the arrays.  Take this into account when
        // counting cycles.
        System::beginCycleCount(overhead);
        {K k; V v;
        for (int i = 0; i < M; ++i) {
            k = keys[i];
            v = vals[i];
        }
        System::endCycleCount(overhead);
        }

        {Table<K, V> t;
        System::beginCycleCount(tableSet);
        for (int i = 0; i < M; ++i) {
            t.set(keys[i], vals[i]);
        }
        System::endCycleCount(tableSet);
        
        System::beginCycleCount(tableGet);
        for (int i = 0; i < M; ++i) {
            t[keys[i]];
        }
        System::endCycleCount(tableGet);

        System::beginCycleCount(tableRemove);
        for (int i = 0; i < M; ++i) {
            t.remove(keys[i]);
        }
        System::endCycleCount(tableRemove);
        }

        /////////////////////////////////

        {std::map<K, V> t;
        System::beginCycleCount(mapSet);
        for (int i = 0; i < M; ++i) {
            t[keys[i]] = vals[i];
        }
        System::endCycleCount(mapSet);
        
        System::beginCycleCount(mapGet);
        for (int i = 0; i < M; ++i) {
            t[keys[i]];
        }
        System::endCycleCount(mapGet);

        System::beginCycleCount(mapRemove);
        for (int i = 0; i < M; ++i) {
            t.erase(keys[i]);
        }
        System::endCycleCount(mapRemove);
        }

        /////////////////////////////////

#       ifdef HAS_HASH_MAP
        {hash_map<K, V> t;
        System::beginCycleCount(hashMapSet);
        for (int i = 0; i < M; ++i) {
            t[keys[i]] = vals[i];
        }
        System::endCycleCount(hashMapSet);
        
        System::beginCycleCount(hashMapGet);
        for (int i = 0; i < M; ++i) {
            t[keys[i]];
        }
        System::endCycleCount(hashMapGet);

        System::beginCycleCount(hashMapRemove);
        for (int i = 0; i < M; ++i) {
            t.erase(keys[i]);
        }
        System::endCycleCount(hashMapRemove);
        }
#       endif
    }

    tableSet -= overhead;
    if (tableGet < overhead) {
        tableGet = 0;
    } else {
        tableGet -= overhead;
    }
    tableRemove -= overhead;

    mapSet -= overhead;
    mapGet -= overhead;
    mapRemove -= overhead;

    float N = M;
    printf("%s\n", description);
    bool G3Dwin = 
        (tableSet <= mapSet) &&
        (tableGet <= mapGet) &&
        (tableRemove <= mapRemove);
    printf("Table         %9.1f  %9.1f  %9.1f   %s\n", 
           (float)tableSet / N, (float)tableGet / N, (float)tableRemove / N,
           G3Dwin ? " ok " : "FAIL"); 
#   ifdef HAS_HASH_MAP
    printf("hash_map      %9.1f  %9.1f  %9.1f\n", (float)hashMapSet / N, (float)hashMapGet / N, (float)hashMapRemove / N); 
#   endif
    printf("std::map      %9.1f  %9.1f  %9.1f\n", (float)mapSet / N, (float)mapGet / N, (float)mapRemove / N); 
    printf("\n");
}


void perfTable() {
    printf("                insert       fetch     remove    outcome\n");

    const int M = 300;
    {
        int keys[M];
        int vals[M];
        for (int i = 0; i < M; ++i) {
            keys[i] = i * 2;
            vals[i] = i;
        }
        perfTest<int, int>("int,int", keys, vals, M);
    }

    {
        std::string keys[M];
        int vals[M];
        for (int i = 0; i < M; ++i) {
            keys[i] = format("%d", i * 2);
            vals[i] = i;
        }
        perfTest<std::string, int>("string, int", keys, vals, M);
    }

    {
        int keys[M];
        std::string vals[M];
        for (int i = 0; i < M; ++i) {
            keys[i] = i * 2;
            vals[i] = format("%d", i);
        }
        perfTest<int, std::string>("int, string", keys, vals, M);
    }

    {
        std::string keys[M];
        std::string vals[M];
        for (int i = 0; i < M; ++i) {
            keys[i] = format("%d", i * 2);
            vals[i] = format("%d", i);
        }
        perfTest<std::string, std::string>("string, string", keys, vals, M);
    }
}
