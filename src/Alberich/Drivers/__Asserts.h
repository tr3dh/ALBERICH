#pragma once

// includes für den asserst Teil
#include <assert.h>
#include <iostream>
#include <functional>
#include <sstream>
#include <cstdlib>

//
#include "Alberich/Drivers/__timeStamp.h"

#ifndef LOG
#define LOG std::cout
#endif // LOG

#ifndef _ERROR
#define _ERROR std::cerr
#endif // _ERROR

#ifndef ENDL
#define ENDL "\n"
#endif // _ERROR

#ifndef TRIGGER_ASSERT
#define TRIGGER_ASSERT  0
#endif

#ifndef PASS_ASSERT
#define PASS_ASSERT     1
#endif

#ifndef RESOLVE_CONDITION
#define RESOLVE_CONDITION(condition)\
    bool resolvedCondition = false;\
    if(condition){\
        resolvedCondition = true;\
    }
#endif

extern std::string (*g_getErrorContext)();
extern bool g_suppressAssertionWarnings, g_storedSuppressionFlag;
extern bool g_terminateAfterAssertionFailed;

//
void DISABLE_ASSERTION_LOGGING();
void ENABLE_ASSERTION_LOGGING();
void RESET_ASSERTION_LOGGING();

#ifndef ASSERT
#define ASSERT(condition, message)\
    do{\
        if (!(condition) && !(g_suppressAssertionWarnings)) {\
            std::ostringstream oss;\
            oss << "[" << getTimestamp() << "] Assertion Failed : '" << message << "'\n" << \
            ">> Source : " << __FILE__ << ":" << __LINE__ << " >> " << __FUNCTION__;\
            \
            if(g_getErrorContext != nullptr){ \
                oss << g_getErrorContext(); \
            } \
            _ERROR << oss.str() << ENDL;\
            if(!(condition) && g_terminateAfterAssertionFailed){ \
                std::terminate();\
            } \
        }}\
    while(0);
#endif

#ifndef CRITICAL_ASSERT
#define CRITICAL_ASSERT(condition, message)\
    do{\
        RESOLVE_CONDITION(condition);\
        ASSERT(resolvedCondition, message);\
    }\
    while(0);
#endif

#ifndef RETURNING_ASSERT
#define RETURNING_ASSERT(condition, message, returnVal)\
    do{\
        RESOLVE_CONDITION(condition);\
        ASSERT(resolvedCondition, message);\
        if (!(resolvedCondition)) {\
            return returnVal;\
        }\
    }\
    while(0)
#endif

#ifndef RETURNING__VOID_ASSERT
#define RETURNING__VOID_ASSERT(condition, message, returnVal)\
    RETURNING_ASSERT(condition,message,)
#endif