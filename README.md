# FeatherDb

This repository contains a small educational project for the third-semester software lab: a minimal SQLite-like database engine written in C++ with a tiny, feature set. The codebase currently provides a clean object-oriented skeleton and placeholders for parser, query handling, storage, and utilities.

Key goals
- Provide a minimal, learnable codebase for database internals (parser, storage, query executor).
- Keep implementations small and explicit that can extend and implement features.

Current Repository layout 
- `src/` — source code (entrypoint: `src/main.cpp`).
- `src/parser/` — tokenizer, parser, AST.
- `src/query/` — query objects and executor.
- `src/storage/` — database, storage engine, file manager.
- `src/access_control/` — RBAC interfaces.
- `src/utils/` — logging and validators.
- `tests/` — basic test scaffolding.
- `data/` — example `.dbtable` files used by the project.

Build & run (basic)
1. From the project root build using your preferred C++ toolchain (example with g++):

```bash
g++ -std=c++17 -I./src -O2 src/main.cpp -o spl1
```

2. Run the produced binary:

```bash
./spl1
```

Notes
- The current files are intentionally skeletons and stubs: no real database logic is implemented yet. Use this as a starting point for implementing parsing, execution, and storage behavior.

Team
- [safwansatil](github.com/safwansatil)
- [saif al sarker](https://github.com/SaifSarker11)
- [wasiomar](https://github.com/WasiOmar)

"I've got no choice. We're cornered, so I have to teach you. First off, Johnny, let me tell you one thing. Starting now, you can only say the words, 'There is no way I can do this!' four times, and four times only. Alright? Four times. That's what my father taught me when I was a kid."


