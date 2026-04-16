<!-- Sprachumschalter -->
<p align="right">
🌐 <a href="./README.md">English</a> | <a href="./README.de.md">Deutsch</a>
</p>

# Assembling Library for Building Embedded Runtime and independent Interpreters Controlling native High Performance Resources (ALBERICH)
<img align="left" style="width:360px" src="Recc/textures/Alberich_Logo_whiteBG.svg">

Alberich is an interpreter framework that enables the activation of native backends via a minimalist language distribution. With Alberich, you can create both embedded runtime interpreters that manipulate runtime objects in real time without the need for compilation, and standalone interpreters that independently manage instances of activated types. Hybrid interpreters can also be implemented. In addition, Alberich implements a fully configurable lexer and parser that can be set up with arbitrary sets of operators, hierarchies, and overloads. The Alberich interpreter framework was developed as part of a [master’s thesis](Recc/Thesis/Masterthesis_Rother_2026_public.pdf) for the Institute of Continuum Mechanics (IKM) at Leibniz University Hannover (LUH).

# 🧬 Development

- Language standard: C++20
- Compiler: gcc/g++
- Subsystem: MinGw64
- Platform: Windows
- Build system: CMake

# 🔧 Feature Set

- Enabling native backends (types, functions, ...)
- Creating runtime interpreters
- Creating standalone interpreters
- Creating hybrid interpreters

# 🛠️ Scope of the Base Language

- Fully configurable operator selection, hierarchy, and overloading
- Imports (scripts, backends)
- Guards
- Metavariables
- Alternative implicit and explicit semantics (construction, memory management, ...)
- Simultaneous operations
- Input-connecting if conventions (nIf, rIf, xIf)
- Fully recursive syntax
- Minimal OOP
- Enableable automatic dereferencing
- Extensive diagnostics at various hierarchy levels (scopes, evalresults, variables, objects, members, ...)
- Script-specific error context generation for custom error handling

# 🛼 Getting Started

The [master's thesis](Recc/Thesis/Masterthesis_Rother_2026_public.pdf) in which Alberich was developed provides a good introduction to the project as well as information about its structure and intended use. In addition, the specialized interpreter [Baldur](https://github.com/tr3dh/BALDUR) was created as part of the thesis using Alberich. The source code for the [backend activation](https://github.com/tr3dh/BALDUR/tree/main/src/AlberichDistro), the [language support](https://github.com/tr3dh/BALDUR/blob/main/Procs/BaldurLSP.cpp), and the [interpreter](https://github.com/tr3dh/BALDUR/tree/main/src/AlberichDistro/Interpreter.h) can be used as an application example. In addition, Alberich independently [enables standard types](src/Alberich/TypeInstances/) and registers them as the `core` backend. These enablements can also be used for orientation.

# 🤝 Acknowledgments

I would like to extend my special thanks to my advisor, Dr. Hendrik Geisler, who was a great help throughout the development process.

Hendrik Geisler was funded by the European Union (ERC, Gen-TSM, project number 101124463) during the period of supervision. However, the views and opinions expressed are solely those of the author(s) and do not necessarily reflect those of the European Union or the European Research Council Executive Agency. Neither the European Union nor the granting authority can be held responsible for them.

## 📚 Libraries and Assets Used
I would also like to thank the respective developers and maintainers of the open-source libraries used in the project, as well as the designers of the open-source assets used in the project.
These are listed below. The corresponding license texts are stored in the [thirdPartyLicenses](/thirdPartyLicenses/) folder.

| Library / Asset       | License                          |
|-------------------|----------------------------------|
| [magic_enum](https://github.com/Neargye/magic_enum) | MIT                              |
| [Boost.PFR](https://github.com/boostorg/pfr)        | Boost Software License 1.0       |
| [lsp-framework](https://github.com/leon-bckl/lsp-framework) | MIT                            |
| [Anvil Icon (Logo)](https://www.svgrepo.com/svg/308950/anvil-hard-metal-metalworking-tool)   | [CC0](https://creativecommons.org/publicdomain/zero/1.0/legalcode.txt) |