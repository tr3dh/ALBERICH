#include "VOID.h"

namespace types{

    std::unique_ptr<IObject> _VOID::nullRef = nullptr;
    const std::string _VOID::nullRefKeyword;
    
    bool _VOID::setUpClass(){

        // register in TypeRegister
        if(!init("void", [](){ return new _VOID(); })){ return false; }

        // Keyword
        _VOID::nullRef = std::make_unique<_VOID>();
        _VOID::nullRef.reset(new _VOID);

        return true;
    }
}

bool IsReferenceValid(std::unique_ptr<IObject>* ptr){

    return ptr != &g_nullRefs[ptr->get()->getTypeIndex()];
}