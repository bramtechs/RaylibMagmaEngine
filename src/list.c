#include "list.h"

List* MakeList(){
    List *list = new(List);

    list->capacity = 1000;
    list->data = M_MemAlloc(list->capacity);

    return list;
}

void TestList(){
    List* list = MakeList();

    int number = 26;
    PushListRaw(list,&number,sizeof(int),0);

    long longNumber = 100000000;
    PushListRaw(list,&longNumber,sizeof(long),1);

    short shortNumber = 120;
    PushListRaw(list,&shortNumber,sizeof(short),2);

    for (int i = 0; i < 1000; i++){
        short shortNumber = i;
        PushListRaw(list,&shortNumber,sizeof(short),2);
    }
    printf("done\n");
}

void PushListRaw(List* list, void* data, size_t size, ItemType type){
    assert(list->data);

    // make sure action has enough memory
    size_t expectedSize = list->size + sizeof(ListItem) + size;
    if (expectedSize > list->capacity) {
        list->capacity *= 2;
        TraceLog(LOG_DEBUG, "Reallocated list to capacity of %d bytes", list->capacity);
        list-> data = MemRealloc(list->data, list->capacity);
    }

    // TODO check if exists
    ListItem* newItem = (ListItem*) (list->data + list->size);
    newItem->size = size;
    char* target = (char*) newItem + sizeof(ListItem);
    memcpy(target, data, size);
    list->size += sizeof(ListItem) + size;
}
