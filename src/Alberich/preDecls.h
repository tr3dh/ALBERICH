#pragma once

// std includes
#include <iostream>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <array>
#include <tuple>
#include <random>
#include <stdio.h>
#include <utility>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <thread>
#include <typeindex>
#include <functional>
#include <map>
#include <string>
#include <optional>
#include <unordered_set>

namespace fs = std::filesystem;
typedef fs::file_time_type fileTime;

#include <magic_enum/magic_enum.hpp>

#ifndef LOG_RED
#define LOG_RED     "\033[31m"
#endif

#ifndef LOG_GREEN
#define LOG_GREEN   "\033[32m"
#endif

#ifndef LOG_YELLOW
#define LOG_YELLOW  "\033[93m"
#endif

#ifndef LOG_BLUE
#define LOG_BLUE    "\033[34m"
#endif

#ifndef LOG_ORANGE
#define LOG_ORANGE  "\033[38;2;255;165;0m"
#endif

#ifndef LOG_RESET
#define LOG_RESET   "\033[0m"
#endif

#ifndef LOG
#define LOG     std::cout << LOG_ORANGE
#endif

#ifndef _ERROR
#define _ERROR  std::cerr << LOG_RED
#endif

#ifndef ENDL
#define ENDL "\n" << LOG_RESET << std::flush;
#endif

// Debugging über message ausgabe und instance debugging, dass direkt überladung für string stream mit << aufruft
#ifndef mbug
#define mbug(message) LOG << LOG_YELLOW << "___Passed : " << #message << ENDL;
#endif

#ifndef ibug
#define ibug(objekt) LOG << LOG_YELLOW << "___Objekt : " << #objekt << " " << objekt << ENDL;
#endif

#include "Alberich/Drivers/__Asserts.h"
#include "Alberich/Drivers/__timeStamp.h"
#include "Alberich/Drivers/__StringProcessing.h"

#ifndef endln
#define endln ENDL
#endif

#include "ByteSequence/ByteSequence.h"

//
static void triggerAssertHandler(const char* message){
    
    RETURNING_ASSERT(TRIGGER_ASSERT, message,);
}

//
static std::string g_languageScriptSuffix = "bld";

//
template<typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T>& vec) {

    os << "Vector with " << vec.size() << " elements" << endln; 
    for(size_t idx = 0; idx < vec.size(); idx++){
        os << idx << " " << vec[idx] << endln;
    }
    return os;
}

//
template<typename Key, typename Value>
std::ostream& operator<<(std::ostream& os, const std::map<Key, Value>& map) {

    os << "Map with " << map.size() << " elements" << endln; 
    for(const auto& [k,v] : map){
        os << k << " " << v << endln;
    }
    return os;
}

template<typename T>
std::string printPlainVector(const std::vector<T>& vec, bool logParens = true, const std::string& token = ", ") {

    if(vec.empty()) {
        return logParens ? "()" : "";
    }
    
    std::string result = logParens ? "(" : "";
    for(size_t i = 0; i < vec.size(); ++i) {
        result += std::to_string(vec[i]);
        if(i != vec.size() - 1) {
            result += token;
        }
    }
    result += logParens ? ")" : "";
    return result;
}

template<typename T>
std::string printIncreasedPlainVector(const std::vector<T>& vec, bool logParens = true, const std::string& token = ", ") {

    if(vec.empty()) {
        return logParens ? "()" : "";
    }
    
    std::string result = logParens ? "(" : "";
    for(size_t i = 0; i < vec.size(); ++i) {
        result += std::to_string(vec[i] + 1);
        if(i != vec.size() - 1) {
            result += token;
        }
    }
    result += logParens ? ")" : "";
    return result;
}

template<typename T, typename Func>
std::string fprintPlainVector(std::vector<T>& vec, const Func& printFunc, bool logParens = true, const std::string& token = ", ") {

    if(vec.empty()) {
        return logParens ? "()" : "";
    }
    
    std::string result = logParens ? "(" : "";
    for(size_t i = 0; i < vec.size(); ++i) {
        
        result += printFunc(vec[i]);
        if(i != vec.size() - 1) {
            result += token;
        }
    }
    result += logParens ? ")" : "";
    return result;
}

template<typename K, typename V>
void emplaceVectorsIntoMap(std::map<K, V>& map, const std::vector<K>& keys, const std::vector<K>& vals) {

    //
    RETURNING_ASSERT(keys.size() == vals.size(), "...",);

    //
    if(keys.empty()){ return; }

    for (size_t i = 0; i < keys.size(); ++i) {

        map.try_emplace(keys[i], vals[i]);
    }
}