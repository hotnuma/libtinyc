#ifndef DELETEFUNC_H
#define DELETEFUNC_H

typedef void (*CDeleteFunc) (void *data);

#if 0
#define DELETEFUNC(t) \
    void delete##t(t *item) \
    { \
        if (item) \
            delete item; \
    }
#endif

#if 0
#define SETDELETEFUNC(l,t) \
    (l)->setDeleteFunc((CDeleteFunc) delete##t)
#endif

#endif // DELETEFUNC_H


