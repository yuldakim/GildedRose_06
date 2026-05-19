# Defect List

## Summary

Current CTest status: Green (`25/25` passed in `cpp/build`).

The following list documents defects found during test implementation and verification. Functional defects are currently resolved in `cpp/src/GildedRose.cpp`.

| ID | Severity | ItemType | Steps | Expected | Actual | Root Cause | Fix Summary |
|---|---|---|---|---|---|---|---|
| DEF-001 | Major | Conjured | 1. Create `Item("Conjured", 10, 20)`.<br>2. Call `GildedRose::updateQuality()`.<br>3. Verify `sellIn` and `quality`. | `sellIn` decreases from `10` to `9`; `quality` decreases twice as fast as a normal item, from `20` to `18`. | Before the fix, Conjured items followed the normal item degradation path and decreased from `20` to `19`. | `updateQuality()` did not distinguish Conjured items from normal items, so the new supplier rule was not implemented. | Added Conjured detection using a C++17-compatible prefix check and applied double degradation before the sell date and quadruple degradation on/after the sell date. |
| DEF-002 | Major | Conjured | 1. Create `Item("Conjured", 0, 20)`.<br>2. Call `GildedRose::updateQuality()`.<br>3. Verify expired-item degradation. | `sellIn` decreases from `0` to `-1`; `quality` decreases by `4`, from `20` to `16`. | Before the fix, Conjured items degraded like expired normal items and decreased from `20` to `18`. | Expired Conjured handling reused the normal expired-item multiplier instead of applying the Conjured multiplier first. | Calculated the base degradation amount as `2` for Conjured items, then doubled it when `sellIn <= 0`. |
| DEF-003 | Minor | Build/Test Infrastructure | 1. Run `cmake --build build` from the repository root.<br>2. Run `ctest` only if the build succeeds. | Build command targets a valid CMake build directory and tests can be executed. | Command fails with `Error: not a CMake build directory (missing CMakeCache.txt)`. | The active CMake project is under `cpp`, and the valid build directory is `cpp/build`, not root-level `build`. | Use `cmake --build cpp/build` and `ctest --test-dir cpp/build --output-on-failure`, or run `cmake --build build` from inside `cpp`. |

## Green Confirmation Procedure

Run from the repository root:

```powershell
cmake --build cpp/build
ctest --test-dir cpp/build --output-on-failure
```

Equivalent procedure from `cpp`:

```powershell
cmake --build build
ctest --test-dir build --output-on-failure
```

