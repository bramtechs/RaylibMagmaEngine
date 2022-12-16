#include "array.h"

Array* MakeArray(size_t itemSize){
    Array* array = new(Array);
    array->itemSize = itemSize;
    array->capacity = 10;
    array->count  = 0;
    array->data = M_MemAlloc(itemSize * array->capacity);
    return array;
}

void* GetArrayItemPtr(Array* array, size_t index){
    assert(array);
    assert (index < array->count);

    return (void*) &((char*)array->data)[index*array->itemSize];
}

void* PushArrayRaw(Array* array, void* item){
    assert(array);
    if (array->count > array->capacity){
        // resize the array!
        array->capacity *= 2;
        array->data = MemRealloc(array->data, array->capacity * array->itemSize);
        DEBUG("Heap array of item size %d resized to %d slots.", array->itemSize, array->capacity);
    }

    void* dest = (void*) &((char*)array->data)[array->count*array->itemSize];
    memcpy(dest,item,array->itemSize);
    array->count++;

    return dest;
}

void TestArrays(){
    Array* arr = MakeArray(sizeof(int));

    for (int i = 0; i < 1000; i++) {
		PushArray(arr,int,&i);
    }

    INFO("arrays made");
}
