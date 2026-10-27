typedef struct DestructorChain {
    struct DestructorChain* next;
    void* destructor;
    void* object;
} DestructorChain;

// A complete-object destructor takes -1 as its second argument.
#define DTORCALL(dtor, objptr) (((void (*)(void*, int))dtor)(objptr, -1))

DestructorChain* __global_destructor_chain;

void* __register_global_object(void* object, void* destructor, void* regmem)
{
    ((DestructorChain*)regmem)->next = __global_destructor_chain;
    ((DestructorChain*)regmem)->destructor = destructor;
    ((DestructorChain*)regmem)->object = object;
    __global_destructor_chain = (DestructorChain*)regmem;

    return object;
}

void __destroy_global_chain(void)
{
    DestructorChain* iter;

    while ((iter = __global_destructor_chain) != 0) {
        __global_destructor_chain = iter->next;
        DTORCALL(iter->destructor, iter->object);
    }
}

__declspec(section ".dtors") static void* const __destroy_global_chain_reference = __destroy_global_chain;
