#include "list.h"

List::~List(){
    for (auto it = begin(); it != end(); ++it) {
        free(it->second);
    }
}

template <typename T>
void List::Push(ItemType type, T& data){
    void* slot = new T;
    memcpy(slot,(void*)&data,sizeof(T));
    this->insert(std::pair<ItemType,void*>(type,slot));
}

void TestList(){
    List list;

    for (int i = 0; i < 500; i++){
        int intNumber = i;
        list.Push(0,intNumber);
        printf("--> %d\n",intNumber);
    }

    for (int i = 0; i < 500; i++){
        long longNumber = 1000000000 + i;
        list.Push(1,longNumber);
        printf("--> %d\n",longNumber);
    }

    printf("done, now reading...\n");

    for (auto it = list.begin(); it != list.end(); ++it) {
        switch (it->first){
            case 0:
                printf("<-- %d\n",*(int*)it->second);
                break;
            case 1:
                printf("<-- %d\n",*(long*)it->second);
                break;
            default:
                printf("<-- UNKNOWN TYPE\n");
                assert(false);
                break;
        }
    }
}
