#include "slab.h"
#include "heap.h"

slab_t* slab_create(size_t obj_size) {
    slab_t *s = (slab_t*) kmalloc(sizeof(slab_t));
    if (!s) return NULL;

    // Ensure minimum object size can hold pointer
    if (obj_size < sizeof(void*))
        obj_size = sizeof(void*);

    s->obj_size = obj_size;
    s->free_list = NULL;

    return s;
}

void* slab_alloc(slab_t *s) {
    if (!s) return NULL;

    if (s->free_list) {
        void *obj = s->free_list;
        s->free_list = *(void**)obj;
        return obj;
    }

    return kmalloc(s->obj_size);
}

void slab_free(slab_t *s, void *ptr) {
    if (!s || !ptr) return;

    *(void**)ptr = s->free_list;
    s->free_list = ptr;
}
