#ifndef C_WEAPONS_H
#define C_WEAPONS_H
#ifdef _WIN32
#pragma once
#endif

#include "client_class.h"

#define IMPLEMENT_CLIENTWEAPON( entityName, className )                     \
    BEGIN_PREDICTION_DATA( className )                                      \
    END_PREDICTION_DATA();                                                  \
    LINK_ENTITY_TO_CLASS( entityName, className )

#define STUB_CLIENTWEAPON( entityName, className, baseClassName )           \
    class C_##className : public baseClassName {                            \
        DECLARE_CLASS( C_##className, baseClassName );                      \
    public:                                                                 \
        C_##className()                                                     \
        {                                                                   \
                                                                            \
        };                                                                  \
        DECLARE_PREDICTABLE();                                              \
        DECLARE_CLIENTCLASS();                                              \
    private:                                                                \
    };                                                                      \
    IMPLEMENT_CLIENTWEAPON( entityName, C_##className );                    \
    IMPLEMENT_CLIENTCLASS_DT( C_##className, DT_##className, C##className ) \
    END_RECV_TABLE()

#endif // C_WEAPON__STUBS_H
