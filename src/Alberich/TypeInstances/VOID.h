#pragma once

#include "InstanceIncludes.h"

#ifdef _VOID
#undef _VOID
#endif

bool IsReferenceValid(std::unique_ptr<IObject>* ptr);

namespace types{

    class _VOID : public IIndexedObject<_VOID>{

    public:

        static bool setUpClass();

        static std::unique_ptr<IObject> nullRef;
        const static std::string nullRefKeyword;

        _VOID() = default;

        std::unique_ptr<IObject> clone() override{
            return std::make_unique<_VOID>();
        };

        // virtual ist redundant, die print bleibt überscheibbar
        void print() const override{
            LOG << "_VOID_MEMBER";
        }
    };
};