<!-- Sprachumschalter -->
<p align="right">
🌐 <a href="./README.md">English</a> | <a href="./README.de.md">Deutsch</a>
</p>

# Assembling Library for Building Embedded Runtime and independent Interpreters Controlling native High Performance Resources (ALBERICH)
<img align="left" style="width:360px" src="Recc/textures/Alberich_Logo_whiteBG.svg">

Alberich ist eine Interpreterschmiede, die die Freischaltung von nativen Backends über eine minimalistische Sprachdistribution ermöglicht. Mit Alberich können sowohl eingebettete Laufzeitinterpreter hergestellt werden, die in Echtzeit und ohne Übersetzungswaufwand Laufzeitobjekte manipulieren, als auch eigenständige Interpreter, die Instanzen freigeschalteter Typen eigenständig verwalten. Hybride Interpreter können ebenfalls impementiert werden. Zudem implementiert Alberich einen vollständig konfigurierbaren Lexer und Parser, der mit beliebigen Operatorensätzen, -hierarchien und -überladungen aufgesetzt werden kann. Die Interpreterschmiede Alberich wurde im Rahmen einer [Masterarbeit](Recc/Thesis/Masterthesis_Rother_2026_public.pdf) für das Institut für Kontinuumsmechanik (IKM) der Leibniz Universität Hannover (LUH) entwickelt.

# 🧬 Entwicklung

- Sprach Standard: C++20
- Compiler: gcc/g++
- Subsystem: MinGw64
- Platform: Windows
- Buildsystem: CMake

# 🔧 Funktionsumfang

- Freischalten nativer Backends (Typen, Funktionen, ...)
- Herstellen von Laufzeitinterpretern
- Herstellen von eigenständigen Interpretern
- Herstellen von hybriden Interpretern

# 🛠️ Umfang der Grundsprache

- vollständig konfigurierbare Operatorenauswahl, -hierarchie und -überladung
- Imports (Skripte, Backends)
- Guards
- Metavariablen
- alternative implizite und explizite Semantiken (Konstruktion, Speicherverwaltung, ...)
- Simultanverknüpfungen
- input-verschaltende If-Konventionen (nIf, rIf, xIf)
- vollständig rekursionsfähige Syntax
- minimales OOP
- aktivierbare automatische Dereferenzierung
- umfangreiche Diagnostik auf verschiedenen Hierachieebenen (Scopes, Evalresults, Variablen, Objekte, Member, ...)
- skriptbezogene Fehlerkontext-Generierung für eigene Fehlerbehandlung

# 🛼 erste Schritte

Die [Masterarbeit](Recc/Thesis/Masterthesis_Rother_2026_public.pdf) in deren Rahmen Alberich entwickelt worden ist, bietet eine geeignete Einarbeitungsmöglichkeit sowie Informationen über den Aufbau und die vorgesehene Nutzung. Zudem ist im Rahmen der Arbeit mithilfe von Alberich der spezialisierter Interpreter [Baldur](https://github.com/tr3dh/BALDUR) hergestellt worden. Der Source-Code der dort vorgenommenen [Backendfreischaltung](https://github.com/tr3dh/BALDUR/tree/main/src/AlberichDistro), des [Language Supports](https://github.com/tr3dh/BALDUR/blob/main/Procs/BaldurLSP.cpp) und des [Interpreters](https://github.com/tr3dh/BALDUR/tree/main/src/AlberichDistro/Interpreter.h) kann als Anwendungs-Beispiel herangezogen werden. Zudem nimmt Alberich eigenständig [Freischaltungen von Standardtypen](src/Alberich/TypeInstances/) vor und registriert sie als Backend `core`. Diese Freischaltungen können ebenfalls zur Einarbeitung herangezogen werden.

# 🤝 Danksagung

Mein besonderer Dank gilt meinem Betreuer Dr. Hendrik Geisler, der während der Entwicklung eine große Hilfe war.

Hendrik Geisler wurde während der Betreuungszeit von der Europäischen Union (ERC, Gen-TSM, project number 101124463) finanziert. Die geäußerten Ansichten und Meinungen sind jedoch ausschließlich die des Autors/der Autoren und spiegeln nicht unbedingt die der Europäischen Union oder der Exekutivagentur des Europäischen Forschungsrats wider. Weder die Europäische Union noch die Bewilligungsbehörde können für sie verantwortlich gemacht werden.

## 📚 Verwendete Bibliotheken und Assets
Bedanken möchte ich mich zudem bei den jeweiligen Entwicklern und Maintainern der im Rahmen des Projekts verwendeten Open-Source Bibliotheken und Designern der im Rahmen des Projekts verwendeten Open-Source Assets.
Diese sind im Folgenden aufgeführt. Die zugehörigen Lizenztexte sind im Ordner [thirdPartyLicenses](/thirdPartyLicenses/) hinterlegt.

| Library / Asset       | License                          |
|-------------------|----------------------------------|
| [magic_enum](https://github.com/Neargye/magic_enum) | MIT                              |
| [Boost.PFR](https://github.com/boostorg/pfr)        | Boost Software License 1.0       |
| [lsp-framework](https://github.com/leon-bckl/lsp-framework) | MIT                            |
| [Anvil Icon (Logo)](https://www.svgrepo.com/svg/308950/anvil-hard-metal-metalworking-tool)   | [CC0](https://creativecommons.org/publicdomain/zero/1.0/legalcode.txt) |