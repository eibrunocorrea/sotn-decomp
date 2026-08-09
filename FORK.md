# About this fork

This is a **staging fork** of [xeeynamo/sotn-decomp](https://github.com/Xeeynamo/sotn-decomp),
the Castlevania: Symphony of the Night decompilation. It exists for one
reason: to let a high-volume stream of PSP decompilation work accumulate and
be validated **without flooding the official project's review queue or
breaking anyone's workflow**.

Everything here belongs, in spirit and in destination, to the original
project. Nothing in this fork competes with it, replaces it, or claims credit
for the years of work it is built on.

## Why this exists

We've been working on the PSP side of the decompilation at a pace that
produces more commits per day than any reviewer should be asked to absorb.
The maintainer's guidance is clear and correct: small, self-contained PRs
that keep `git blame` useful and reviews humane.

So this fork is the pressure tank. The work lands here first — every commit
built and checksum-verified before the next — and flows into the official
repository as **small, reviewable PRs, a few at a time**. After each review
round, the next slice goes up. Nothing is held back out of possessiveness;
it's all queued for upstream, in the order and size that respects the
project's process.

If you want to see the full picture ahead of the queue, it's all here in the
open: currently **211 PSP functions (net vs upstream master) wired from
`INCLUDE_ASM` stubs to shared C across 19 overlays**, 44 stub files retired,
12 new shared headers — with `us`, `hd` and `pspeu` checksums green on every
single commit. Verifiable in ten seconds:

```
git grep -c INCLUDE_ASM <ref> -- src/ | grep _psp   # sum and compare refs
```

## The bar is the same bar

This fork follows the official project's standards, on purpose and to the
letter: same shared-code architecture, same per-overlay parametrization
patterns the repo already uses, `make format` clean, no symexport edits,
three versions byte-identical at every step. When something here diverges
from an in-flight upstream PR, **we step aside and rebase** — actively
avoiding areas other contributors are working on is part of the method (and
has already happened in practice, more than once).

## Credits — the only section that really matters

None of this would exist without the original project. The toolchain, the
splat configurations, the shared-code architecture, the matching
infrastructure, and the overwhelming majority of the decompiled code are the
work of [**@Xeeynamo**](https://github.com/Xeeynamo) and the sotn-decomp
community. This fork stands entirely on that foundation.

Thank you to every contributor of the official repository:

Xeeynamo, bismurphy, sozud, gamezter, JoshSchreuder, hohle, ProjectOblivion,
Onenutmcgee, joshlory, synamaxmusic, ser-pounce, mkst, SestrenExsis, mrmidi,
cram0, DerpPrincess, frog404, mathisto, rossimo, TomoshibiAkira, dezgeg,
Pyromuffin, MottZilla, amelenty, NotExactlySiev, Max-Clark, PancakeFriday,
bryanr1992, flafmg, piratesephiroth, roeming, smb123w64gb, nyxojaele,
gretfrage, ayushrakesh, xanderhall, TalicZealot, shresthasurav,
mishrasamiksha, Rainchus, OlivierDoriath, MrSynAckSter, kpanicc, syrofx,
Mamotromico, christianpaez, bhargavshirin —
and everyone on the project's Discord doing the unglamorous work of review,
naming, and verification. ([Full contributors list](https://github.com/Xeeynamo/sotn-decomp/graphs/contributors))

If any part of this fork overlaps work you have in flight, open an issue or
ping us — we'll yield, rebase, or hand it over. Colliding with the community
is the one failure mode this fork was created to avoid.

## License

Same terms as the official repository. All of it.
