# Handoff

## 2026-10-10 — Codex

Objective: Migrate the 28 T5 upstream-diff symbol aliases from C defines into version/overlay symbols.

ENTREGUE: Migrated all 28 aliases; added 28 US bindings and renamed one PSP placeholder in place. Updated the BO3 extern declarations and the shared RBO1 data definition. PSP EU seal passes: BUILD_RC=0, FAILED_UNITS=0, OK=66 TOTAL=66, empty key-error section. Final diff check is clean.

ARQUIVOS: Eight C sources and six overlay symbols files; full 28-row mapping and seal outputs in `.superpowers/sdd/01-plano/t5-relatorio.md`, with four adjacent seal logs. Progress ledger appended.

DECISÕES: YAML `.data` entries compile RBO1 `e_init.c` in both US and PSP; the C definition and both symbol bindings must share `g_EInitBossDoor`. No aliases were retained. Global symbols, YAMLs, and `tools/psyz` remain unchanged. No Mac Docker was used.

COMO TESTAR: Verify no Mac `src/**/gen/` or `src/.assets_build_done_*`; run `./.superpowers/sdd/01-plano/seal.sh us` and `./.superpowers/sdd/01-plano/seal.sh pspeu`. Acceptance requires BUILD_RC=0, FAILED_UNITS=0, an empty key-error section, and US 115/115 plus PSP 66/66.

PENDENTE: US acceptance is blocked. Attempt 1 (113/115) found a baseline RBO1 undefined reference to `func_us_80194714`; commit `1bd120545` removed the address binding without replacing it with upstream's `func_us_8019D260_from_rcen = 0x80194714;`. That separate function-symbol repair was not made under the 28-alias scope. Attempt 2 hit the YAML generator `IndexError: string index out of range`, then Ninja's missing `src/.assets_build_done_us`; no third US attempt under the two-failure rule. Final shared RBO1 data rename followed that US invocation and requires a fresh US seal. The work-state tool rejected its write: `MCP tool call requires approval, but approval policy is never`; no bypass.

ÁRVORE: Branch `sync/upstream-2026-10`, HEAD `1bd120545`; 14 implementation files modified and unstaged, plus this handoff. Known `tools/psyz` submodule state preserved. Report/logs/progress are ignored delivery artifacts. No commit, push, reset, stash, clean, branch switch, deletion, or Mac build.

RESOLVIDO (coordenador, mesmo dia): linhas `func_us_8019D260_from_rcen` do upstream restauradas em borbo1/borbo2; commitado em e6cca909d com US 115/115 e PSP 66/66 em mundo limpo; HD 13/13 no Mac. Sync completo em `rno0-code-share` 8d90fa744 (push origin + wip).

## 2026-08-17 — Codex

Objective: Finish the in-progress merge of `upstream/master` into `sync/upstream-2026-08-17`, preserving local PSP wiring and decompilation work.

ENTREGUE: Resolved all 17 remaining file contents in the worktree; completed the conflict-marker, overlay-export macro, persistent-drop rename, and duplicate `INCLUDE_ASM` sweeps. Removed the obsolete BO6 PSP `unk_10420` duplicate TU and its stale segment entry.

ARQUIVOS: The 17 requested conflict paths, `src/boss/bo7/bo7.h`, `src/st/rno0/e_medusa_head.c`, `src/boss/bo6_psp/unk_10420.c`, and `config/splat.pspeu.bobo6.yaml`.

DECISÕES: Used upstream as the base for BO0, BO1, NZ1, RCEN, RNO0 PSP, and Ctulhu; preserved local real-C Richter, RBO1 polar/cluster helpers, RNO2 Flying Zombie/Azaghal support, RNO4 decompilation, and PSP-only wiring. Kept the actual upstream `func_us_801BA030` name because `MERGE_HEAD` does not contain `PlayOlroxDroolSfx`.

COMO TESTAR: Run the four requested grep/duplicate sweeps, then `git diff --check`; no build or Docker run is authorized for this merge.

PENDENTE: Git metadata is read-only in the current sandbox. `git add` failed with `fatal: Unable to create '/Users/brunocorrea/Projetos/SOTN Decompiled/sotn-decomp/.git/index.lock': Operation not permitted`. Stage all resolved paths, commit with Bruno's exact two-paragraph message, and verify `git status` is clean.

ÁRVORE: Worktree contents are resolved and swept, but the index still reports the original 17 unmerged paths because `.git` could not be written. No commit or push was performed.
