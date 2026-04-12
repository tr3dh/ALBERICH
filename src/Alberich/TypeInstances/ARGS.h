#pragma once

#include "InstanceIncludes.h"
#include "../Evaluation/EvalResult.h"

#include "INT.h"
#include "STRING.h"

#include "Operations.h"

namespace types{

    class ARGS : public INativeObject<ARGS, EvalResultVec>{

    public:

        //
        static bool setUpClass();

        // Für Funktionen die konkrete TypeIndices brauchen weil source Typen nach eigener Klasse registriert werden
        // >> OtherClass::typeIndex noch nicht verfügbar
        static bool emplaceOperations();

        ARGS() = default;

        ARGS(const FunctionParams& params){

            member->clear();
            member->reserve(params.size());

            for(const auto& param : params){

                member->emplace_back();

                if(param->isLValue()){

                    member->back().setLValue(&param->getVariableRef());
                }
                else{

                    member->back().cloneIntoRValue(param->getVariableRef());
                }
            }

            eliminateLValues();
        }

        ARGS(ARGS& other){

            //
            for(auto& res : other.getMember()){

                //
                RETURNING_ASSERT(res.isValid(), "Invalide Variable in ARGS die kopiert werden sollen",);

                if(res.getVariableRef().isReference()){

                    getMember().emplace_back(); getMember().back().reference(res);
                }
                else{

                    getMember().emplace_back(); getMember().back().cloneIntoRValue(res.getVariableRef());
                    // getMember().emplace_back(); getMember().back().constructRValueByObject(res.getData()->clone().release());
                }
            }
        }

        //
        std::unique_ptr<IObject> clone() override {

            return std::make_unique<ARGS>(*this); 
        }

        void moveFrom(const FunctionParams& params){

            member->clear();
            member->reserve(params.size());

            for(const auto& param : params){

                if(param->getTypeIndex() == IObject::ARGS_TYPE){

                    // es soll verhindert werden dass die ARGS ein Referenz auf sich selbst enthalten die
                    // bei einer zuweisung dann ungültig werden
                    // zb bei prms = args(prms)
                    // >> der rvalue args(prms) enthält eine referenz auf prms, 

                    //
                    EvalResult res;
                    res.cloneIntoRValue(param->getVariableRef());

                    //
                    member->emplace_back(std::move(res));
                }
                else if(param->isLValue()){
                    
                    member->emplace_back().setLValue(&param->getVariableRef());
                }
                else{

                    member->emplace_back().moveIntoRValue(param->getVariableRef());
                }
            }

            eliminateLValues();
        }

        //
        void emplace(const FunctionParams& params){

            //
            member->reserve(params.size());

            for(const auto& param : params){

                //
                if(this == param->getData()){

                    // Wenn zuerst der emplace passiert ist das object mit einer invaliden Varable
                    // gefüllt wenn es geclont wird. Deshalb muss eine tempräre Kopie gemacht werden

                    //
                    EvalResult res;
                    res.cloneIntoRValue(param->getVariableRef());

                    //
                    emplace({&res});
                }
                else if(param->isLValue()){

                    //
                    member->emplace_back().setLValue(&param->getVariableRef());
                }
                else if(param->getVariableRef().isReference()){

                    //
                    member->emplace_back().reference(*param);
                }
                else{

                    //
                    member->emplace_back().moveIntoRValue(param->getVariableRef());
                }
            }

            eliminateLValues();
        }

        //
        void unpackingEmplace(const FunctionParams& params){

            // ist unter Umständen nicht genug wegen ArgsParams
            member->reserve(params.size());

            for(const auto& param : params){

                // Nicht nötig, da wenn der Parameter eine Args Variable ist eh geclont werden muss

                // //
                // if(this == param->getData()){

                //     // Wenn zuerst der emplace passiert ist das object mit einer invaliden Varable
                //     // gefüllt wenn es geclont wird. Deshalb muss eine tempräre Kopie gemacht werden

                //     //
                //     EvalResult res;
                //     res.cloneIntoRValue(param->getVariableRef());

                //     //
                //     unpackingEmplace({&res});
                // }
                // else
                if(param->getTypeIndex() == IObject::ARGS_TYPE){

                    //
                    EvalResult res;
                    res.cloneIntoRValue(param->getVariableRef());

                    //
                    for(auto& subParam : static_cast<ARGS*>(res.getData())->getMember()){

                        unpackingEmplace({&subParam});
                    }
                }
                else if(param->isLValue()){

                    //
                    member->emplace_back().setLValue(&param->getVariableRef());
                }
                else if(param->getVariableRef().isReference()){

                    //
                    member->emplace_back().reference(*param);
                }
                else{

                    //
                    member->emplace_back().moveIntoRValue(param->getVariableRef());
                }
            }

            eliminateLValues();
        }

        // Vorläufig weil Einbauen von lvalue behandlung in aktuelle dref routinen zu aufwendig ist
        // >> keine verwendung von lvalues
        void eliminateLValues(){

            for(auto& res : getMember()){

                if(res.isLValue()){

                    Variable* var = &res.getVariableRef();
                    
                    res.variablePtr = nullptr;
                    res.variable.forceReference(var->getUniqueData());
                }
            }
        }

        //
        void clear() override{

            getMember().clear();
        }

        // ARGS(const EvalResultVec& params) : INativeObject(params){}
        // ARGS(EvalResultVec* paramPtr) : INativeObject(paramPtr){}

        // ARGS(ARGS& other){

        //     auto& args = getMember(), otherArgs = other.getMember();

        //     args.clear();
        //     args.reserve(otherArgs.size());

        //     for(auto& arg : otherArgs){

        //         if(arg.isLValue()){
        //             args.emplace_back(arg);
        //         }
        //         else{
        //             args.emplace_back().cloneIntoRValue(arg.getVariableRef());
        //         }
        //     }
        // }

        // virtual ist redundant, die print bleibt überscheibbar
        void print() const override{

            LOG << "ARGS [";

            for(const auto& param : getMember()){

                param.getVariableRef().getData()->print(); LOG << ", ";
            }

            LOG << "]";
        }

        // Anpassen sobald es zu MM Problemen kommt mit Gültigkeitsverletzungen bei Löschung / Verlassen von Scopes
        Variable* getAttrib(const std::string& attribLabel) override { return nullptr; }

        bool isTrivial() override { return false; }

        std::pair<bool, Variable*> containsDataReference(IObject* dataPtr) override;

        std::pair<bool, Variable*> containsDataVariableOrReference(IObject* dataPtr) override;
    };
};