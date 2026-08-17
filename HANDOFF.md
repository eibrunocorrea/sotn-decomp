# Handoff

## 2026-08-17 — Codex

Objective: Finish the in-progress merge of `upstream/master` into `sync/upstream-2026-08-17`, preserving local PSP wiring and decompilation work.

ENTREGUE: Resolved all 17 remaining file contents in the worktree; completed the conflict-marker, overlay-export macro, persistent-drop rename, and duplicate `INCLUDE_ASM` sweeps. Removed the obsolete BO6 PSP `unk_10420` duplicate TU and its stale segment entry.

ARQUIVOS: The 17 requested conflict paths, `src/boss/bo7/bo7.h`, `src/st/rno0/e_medusa_head.c`, `src/boss/bo6_psp/unk_10420.c`, and `config/splat.pspeu.bobo6.yaml`.

DECISÕES: Used upstream as the base for BO0, BO1, NZ1, RCEN, RNO0 PSP, and Ctulhu; preserved local real-C Richter, RBO1 polar/cluster helpers, RNO2 Flying Zombie/Azaghal support, RNO4 decompilation, and PSP-only wiring. Kept the actual upstream `func_us_801BA030` name because `MERGE_HEAD` does not contain `PlayOlroxDroolSfx`.

COMO TESTAR: Run the four requested grep/duplicate sweeps, then `git diff --check`; no build or Docker run is authorized for this merge.

PENDENTE: Git metadata is read-only in the current sandbox. `git add` failed with `fatal: Unable to create '/Users/brunocorrea/Projetos/SOTN Decompiled/sotn-decomp/.git/index.lock': Operation not permitted`. Stage all resolved paths, commit with Bruno's exact two-paragraph message, and verify `git status` is clean.

ÁRVORE: Worktree contents are resolved and swept, but the index still reports the original 17 unmerged paths because `.git` could not be written. No commit or push was performed.
