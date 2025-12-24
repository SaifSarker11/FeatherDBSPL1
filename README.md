# SPL1 — Minimal SQLite-like Engine (Lab 1)

This repository contains a small educational project for the third-semester software lab: a minimal SQLite-like database engine written in C++ with a tiny, intentionally incomplete feature set. The codebase currently provides a clean object-oriented skeleton and placeholders for parser, query handling, storage, and utilities.

Key goals
- Provide a minimal, learnable codebase for database internals (parser, storage, query executor).
- Keep implementations small and explicit so students can extend and implement features.

Repository layout (selected)
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
- `safwansatil`
- `wasiomar`
- `saifsarker11`

Good luck with Lab 1 — Inshallah this helps you get started.

# FeatherDb
