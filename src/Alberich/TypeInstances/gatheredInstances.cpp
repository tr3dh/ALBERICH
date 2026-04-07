#include "gatheredInstances.h"

//
bool setUpCore(){

    fetchBackend("void");
    fetchBackend("args");
    fetchBackend("bool");
    fetchBackend("int");
    fetchBackend("double");
    fetchBackend("string");

    fetchBackend("argOps");
    fetchBackend("stdOps");

    return true;
}

void setUpTypes(){

    RETURNING_ASSERT(setUpBackendRegister(), "Backendregister konnte nicht aufgesetzt werden",);

    emplaceBackend("void", &types::_VOID::setUpClass);
    emplaceBackend("args", &types::ARGS::setUpClass);
    emplaceBackend("bool", &types::BOOL::setUpClass);
    emplaceBackend("int", &types::INT::setUpClass);
    emplaceBackend("double", &types::DOUBLE::setUpClass);
    emplaceBackend("string", &types::STRING::setUpClass);

    emplaceBackend("argOps", &types::ARGS::emplaceOperations);
    emplaceBackend("stdOps", &emplaceStdOperations);

    emplaceBackend("core", &setUpCore);
    fetchBackend("core");
}