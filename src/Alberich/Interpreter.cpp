#include "Interpreter.h"
#include "Evaluation/EvaluateExpression.h"

std::map<std::string, Script> g_Scripts = {};

Script* g_currentlyEvaluatedScript = nullptr;
ASTNode* g_currentlyEvaluatedNode = nullptr;

int countOccurrences(const std::string& str, const std::string& sub) {

    if (sub.empty()) return 0;

    int count = 0;
    size_t pos = str.find(sub);

    while (pos != std::string::npos) {
        ++count;
        pos = str.find(sub, pos + sub.length());
    }

    return count;
}

std::string getErrorContext(){

    std::string res;

    if(g_currentlyEvaluatedScript != nullptr && g_currentlyEvaluatedNode != nullptr){

        std::ostringstream oss;

        // Bereich in tokenfolge : g_currentlyEvaluatedNode->begin - g_currentlyEvaluatedNode->end
        const Token& firstToken = g_currentlyEvaluatedScript->tokens[g_currentlyEvaluatedNode->begin];
        const Token& lastToken = g_currentlyEvaluatedScript->tokens[g_currentlyEvaluatedNode->end - 1];

        size_t position = 0, len = 0, linePos = 0, lineSpan = 0;

        // Zeile i geht von lineBreaks[i] bis lineBreaks[i+1]-1
        for(size_t i = 0; i < g_currentlyEvaluatedScript->lineBreaks.size() - 1; i++){
            
            size_t lineStart = g_currentlyEvaluatedScript->lineBreaks[i];
            size_t lineEnd = g_currentlyEvaluatedScript->lineBreaks[i + 1] - 1; // -1 weil lineBreaks[i+1] bereits nächste Zeile ist
            
            // Zeile in der sich firstToken befindet
            if(lineStart <= firstToken.position && firstToken.position < g_currentlyEvaluatedScript->lineBreaks[i + 1]){
                position = lineStart;
                linePos = i;
            }
            
            // Zeile in der sich lastToken befindet
            if(lineStart <= lastToken.position && lastToken.position < g_currentlyEvaluatedScript->lineBreaks[i + 1]){
                len = lineEnd - position;
                lineSpan = i - linePos;
            }
        }
        
        //
        res += "\n>> Context : ";

        // Header mit Dateiname und Zeilennummer
        std::string normalizedPath = fs::absolute(g_currentlyEvaluatedScript->scriptPath).string();
        std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
        res += normalizedPath + ":" + std::to_string(linePos + 1) + "\n";

        // Debug output
        // res += std::to_string(position) + " " + std::to_string(len) + "\n";
        // res += std::to_string(linePos) + " " + std::to_string(lineSpan) + "\n";

        // alle betroffenen Zeilen mit Markierungen
        for(size_t i = linePos; i <= linePos + lineSpan; i++){

            //
            RETURNING_ASSERT(i < g_currentlyEvaluatedScript->lineBreaks.size(),
                "Assertion Context kann aufgrund fehlerhafter Skriptformatierung nicht erzeugt werden", "");
            
            //
            std::string lineNum = std::to_string(i + 1);
            std::string line = g_currentlyEvaluatedScript->getLine(i);
            res += lineNum + "   | " + line + "\n";

            // Berechne Markierung für diese Zeile
            size_t prefix = lineNum.length() + 5; // Zeilennummer + 3 Spaces + " | "
            
            size_t lineStart = g_currentlyEvaluatedScript->lineBreaks[i];
            size_t lineEnd = g_currentlyEvaluatedScript->lineBreaks[i + 1] - 1;
            
            // Start und Ende der Markierung
            size_t markStart, markEnd;
            
            if(i == linePos){

                // Erste Zeile
                markStart = firstToken.position - lineStart;

            } else {

                // Folgezeilen
                markStart = line.find_first_not_of(" \t");
            }
            
            if(i == linePos + lineSpan){

                // Letzte Zeile
                markEnd = lastToken.position + lastToken.length - lineStart;
            } else {

                // Zwischenzeilen
                markEnd = line.length();
            }
            
            // Erzeuge Markierung
            res += std::string(prefix + markStart, ' ');
            
            if(markEnd > markStart){

                res += (i == linePos) ? "^" : "~";
                if(markEnd - markStart > 1){
                    res += std::string(markEnd - markStart - 1, '~');
                }
            }
            
            res += "\n";
        }
    }

    return res;
}

std::vector<std::unique_ptr<IObject>> executeProgram(const std::string& scriptPath, Scope* parent){

    //
    CheckAllOperatorsRegistered();

    //
    // g_terminateAfterAssertionFailed = true;

    // Abhier wird als Context in den Asserts der entsprechende Code Ausschnitt aufgeführt
    g_getErrorContext = &getErrorContext;

    //
    SetUpLexer(g_UsedOperators);            

    //
    Scope* rootScope = (parent != nullptr) ? parent : new Scope();

    // //
    // std::string ExecMode = "DEBUG";
    // rootScope->constructAndReturnVariable("ExecMode")->constructByObject(new types::STRING(&ExecMode));

    // Scope befüllen
    rootScope->constructVariable("__ScriptCalledAs__", types::INT::typeIndex);

    //
    rootScope->constructVariable("__MainProc__", types::INT::typeIndex);
    rootScope->setVariable("__MainProc__", new types::INT(0));

    //
    rootScope->constructVariable("__CoProc__", types::INT::typeIndex);
    rootScope->setVariable("__CoProc__", new types::INT(1));

    //
    rootScope->constructVariable("__Include__", types::INT::typeIndex);
    rootScope->setVariable("__Include__", new types::INT(2));

    // Hier wird Skript als auszuführendes MainProc aufgerufen
    rootScope->setVariable("__ScriptCalledAs__", new types::INT(0));

    //
    rootScope->constructVariable("__script__", types::STRING::typeIndex);
    rootScope->setVariable("__script__", new types::STRING(std::filesystem::absolute(scriptPath).string()));

    //
    rootScope->constructVariable("__proc__", types::STRING::typeIndex);
    rootScope->setVariable("__proc__", new types::STRING(std::filesystem::absolute(scriptPath).string()));

    //
    rootScope->constructVariable("__args__", types::STRING::typeIndex);
    rootScope->setVariable("__args__", new types::STRING("--execute"));

    //
    rootScope->constructVariable("__CWD__", types::STRING::typeIndex);
    rootScope->setVariable("__CWD__", new types::STRING(fs::path(scriptPath).parent_path().string()));

    //
    ProcessingResult scriptReturn = executeScript(scriptPath, rootScope, ExecuteScriptAs::MainProc);

    // isolieren der Objekte aus dem EvalResult Vektor
    std::vector<std::unique_ptr<IObject>> isolatedObjects;
    isolatedObjects.reserve(scriptReturn.evalResults.size());

    // hier evtl mov und nicht clone
    // >> clean up für Structs, sodass keine invaliden parent ptrs überbleiben
    for(auto& obj : scriptReturn.evalResults){
        isolatedObjects.emplace_back(obj.getVariableRef().getData()->clone());
    }

    // Löschen der pointer auf die nullScope
    STRUCT::cleanUp();

    // nicht einfach löschen da sonst die parent ptrs in den attrib scopes sonst ungültig werden
    for(auto& [idx, scope] : g_staticScopes){ scope.parent = nullptr; }
    // g_staticScopes.clear();

    //
    g_getErrorContext = nullptr;

    //
    if(parent == nullptr){ delete rootScope; }

    return isolatedObjects;

    // nullScope wird geläscht ...
    // --- ab hier sind alle ptrs auf die nullScope ungültig 
}

void (*g_handleScriptBeforeExecution)(const std::string&) = nullptr;
void (*g_handleScriptAfterExecution)(const std::string&) = nullptr;

ProcessingResult executeScript(const std::string& scriptPath, Scope* nullScope, ExecuteScriptAs execAs){

    //
    if(g_handleScriptBeforeExecution != nullptr){ (*g_handleScriptBeforeExecution)(scriptPath); }

    //
    RETURNING_ASSERT(nullScope != nullptr, "nullScope pointer ist nullptr", {});

    //
    RETURNING_ASSERT(fs::exists(scriptPath), "Script " + scriptPath + " existiert nicht", {});

    //
    std::ifstream file(scriptPath);
    RETURNING_ASSERT(file, "Script " + scriptPath + " kann nicht geöffnet werden", {});

    bool cachePrevScript = false;
    Script* g_previouslyEvaluatedScript = nullptr;
    ASTNode* g_previouslyEvaluatedNode = nullptr;

    if(g_Scripts.contains(scriptPath)){
        
        RETURNING_ASSERT(TRIGGER_ASSERT, "Mehrfacher Include von " + scriptPath, {});
    }

    Script& src = g_Scripts.try_emplace(scriptPath).first->second;
    src.scriptPath = scriptPath;

    if(g_currentlyEvaluatedScript != nullptr){

        cachePrevScript = true;

        g_previouslyEvaluatedScript = g_currentlyEvaluatedScript;
    }

    g_currentlyEvaluatedScript = &src;
    
    std::string line;
    while (std::getline(file, line)) {

        //
        static bool fuseLine;
        fuseLine = false;

        //
        std::string lineStr;

        if(string::contains(line, "//")){
            
            if(line.find_first_of("//") == line.find_first_not_of(" \t")){
                
                lineStr = "";
            }
            else{
                
                lineStr = line.substr(0, line.find_first_of("//"));
            }
        }
        else if(string::contains(line, "\\") && line.find_last_of("\\") == line.find_last_not_of(" \t")){

            lineStr = line.substr(0, line.find_last_of("\\"));
            fuseLine = true;
        }
        else{

            lineStr = line;
        }

        if(fuseLine){

        }
        else if(lineStr.find_first_not_of(" \t;{") == lineStr.npos){
            
        }
        else if(lineStr[lineStr.find_last_not_of(" \t")] != ';'){
            
            lineStr += ";";
        }
        else{
            
        }

        src.scriptContent += lineStr;
        src.scriptContent += "\n";
    }

    //
    file.close();

    //
    src.cacheLineBreaks();

    src.tokens = lexExpression(src.scriptContent);
    src.Expr.end = src.tokens.size();

    convertTokensToAST(src.Expr, src.tokens, src.scriptContent);

    //
    ProcessingResult prc = evaluateExpression(src.Expr, *nullScope, *nullScope, Context::NONE);

    if(cachePrevScript){

        g_currentlyEvaluatedScript = g_previouslyEvaluatedScript;
    }

    //
    if(g_handleScriptAfterExecution != nullptr){ (*g_handleScriptAfterExecution)(scriptPath); }

    //
    return prc;
}

bool CheckAllOperatorsRegistered()
{
    bool allFound = true;

    auto check = [&](const std::map<std::string, std::string>& ops, const std::string& mapName)
    {
        for (const auto& [op, _] : ops)
        {
            if (std::find(g_UsedOperators.begin(), g_UsedOperators.end(), op) == g_UsedOperators.end())
            {
                RETURNING_ASSERT(TRIGGER_ASSERT, "Operator '" + op + "' kommt in PREFIX, INFIX oder FOLD vor wird im LEXICON aber nicht angegeben",);
                allFound = false;
            }
        }
    };

    check(g_OneArgOperations,   "g_OneArgOperations");
    check(g_TwoArgOperations,   "g_TwoArgOperations");
    check(g_ArgChainOperations, "g_ArgChainOperations");

    return allFound;
}