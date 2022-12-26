#include "list.h"

List* MakeList(){
    List *list = new(List);

    list->capacity = 1000;
    list->data = M_MemAlloc(list->capacity);

    return list;
}

void DisposeList(List* list){
    M_MemFree(list->data);
    M_MemFree(list);
}

void PushListRaw(List* list, void* data, size_t size, ItemType type){
    assert(list->data);

    // make sure action has enough memory
    size_t expectedSize = list->size + sizeof(ListItem) + size;
    if (expectedSize >= list->capacity) {
        // !!! IMPORTANT TODO resize corrupts memory!!!
        list->capacity *= 2;
        TraceLog(LOG_DEBUG, "Reallocated list to capacity of %d bytes", list->capacity);
        list->data = (char*) MemRealloc(list->data, list->capacity);
    }

    ListItem* newItem = (ListItem*) (list->data + list->size);
    newItem->size = size;
    newItem->type = type;
    char* target = (char*) newItem + sizeof(ListItem);
    memcpy(target, data, size);
    list->size += sizeof(ListItem) + size;
    list->count++;
}

ListIterator IterateListItems(List* list){
    assert(list);

    ListItem* item = NULL;
    if (list->count > 0){
        item = (ListItem*) list->data;
    }

    return (ListIterator) {
        list,
        item,
        0,
    };
}

bool IterateNextItem(ListIterator* it, ItemType* type, void** result){
    List* list = it->list;

    if (it->current == NULL){
        return false; // list is empty, do nothing
    }

    if (it->curIndex < list->count){
        // get cur item data
        ListItem* curItem = it->current;
        *type = curItem->type;

        char* curItemPtr = curItem;
        *result = (void*) (curItemPtr + sizeof(ListItem));

        // register next item
        char* nextItemPtr = (char*) (curItemPtr + sizeof(ListItem) + curItem->size);
        it->current = (ListItem*) nextItemPtr;

        it->curIndex++;
        return true;
    }

    return false; // reached end of list
}

void TestList(){
    List* list = MakeList();

    for (int i = 0; i < 500; i++){
        int intNumber = i;
        PushListRaw(list,&intNumber,sizeof(int),0);
        printf("--> %d\n",intNumber);
    }

    for (int i = 0; i < 500; i++){
        long longNumber = 1000000000 + i;
        PushListRaw(list,&longNumber,sizeof(long),1);
        printf("--> %d\n",longNumber);
    }

    printf("done, now reading...\n");

    ListIterator it = IterateListItems(list);

    ItemType type = 0;
    void* data = NULL;
    while (IterateNextItem(&it,&type,&data)){
        switch (type){
            case 0:
                printf("<-- %d\n",*(int*)data);
                break;
            case 1:
                printf("<-- %d\n",*(long*)data);
                break;
            default:
                printf("<-- UNKNOWN TYPE\n");
                assert(false);
                break;
        }
    }
}
