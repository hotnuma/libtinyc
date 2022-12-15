#ifndef DELETEFUNC_H
#define DELETEFUNC_H

typedef void (*CDeleteFunc) (void *data);

#define DELETEFUNC(t) \
    void delete##t(t *item) \
    { \
        if (item) \
            delete item; \
    }

#define SETDELETEFUNC(l,t) \
    (l)->setDeleteFunc((CDeleteFunc) delete##t)

#endif // DELETEFUNC_H


