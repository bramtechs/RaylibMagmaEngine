#include "list.h"

List* MakeList(){
    List *list = new(List);

    // make room for first item
    list->data = new(ListItem); // TODO don't alloc twice!

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

    for (int i = 0; i < 100; i++){
        short shortNumber = 69;
        PushListRaw(list,&shortNumber,sizeof(short),2);
    }
}

ListItem* list_get_beyond_tail(List *list, size_t *totalSize){ // UwU
    ListItem* item = (ListItem*) list->data;

    // crawl to latest
    *totalSize = 0;
    while (item->continues){
        char* next = (char*)item;
        size_t size = sizeof(ListItem) + item->size;
        next += size;
        *totalSize += size;
        item = (ListItem*)next;
    }
    item->continues = true; // TODO make clear

    char* next = (char*)item;
    size_t size = sizeof(ListItem) + item->size;
    next += size;
    *totalSize += size;
    item = (ListItem*)next;

    // return pointer to imaginary list item beyond the tail
    // totalSize: returns the size of the entire list: TODO put in own function
    return item;
}

void* list_get_data_ptr(ListItem *item){
    assert(item);

    char* ptr = (char*) item;
    size_t offset = sizeof(ListItem);
    return (void*) (ptr + offset);
}

void PushListRaw(List* list, void* data, size_t size, ItemType type){

    size_t listSize = 0;

    // reallocate enough space for new item
    list_get_beyond_tail(list, &listSize);

    size_t totalSize = listSize + size;
    printf("Realloc to %d", totalSize);
    list->data = (void*) MemRealloc(list->data,totalSize);

    // !!! pointers changed !!!

    // get the pointer where the raw data will go
    ListItem* latest = list_get_beyond_tail(list, &listSize);

    // fill in new object
    latest->continues = false;

    void* dataPtr = list_get_data_ptr(latest);
    memcpy(dataPtr,data,size);

    latest->type = type;
    latest->size = size;
}

