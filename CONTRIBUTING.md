# Contributors guidelines

## Reporting Bugs

Please follow these guidelines when reporting a bug or crash:
* Please be thorough in your bug report. Tell us everything.
* If you can recreate the bug give us a step by step, and attach the place file if one is involved.
* Check whether the original 2007 client behaves the same way. If it does, say so, it changes what the fix should be.
* Make sure your bug hasn't already been reported.

## Feature Requests

Please follow these guidelines when requesting a feature:
* Tell us why you need this feature, what it adds, and what you have tried.
* Make sure your feature hasn't already been requested.

## Making Changes

### Adding new features

Before you start trying to add a new feature, it should be something people want and has been discussed in an issue ideally.

### Fixing bugs

If you're fixing a bug, make sure you reference any applicable bug reports, explain what the problem was and how it was solved.

### Porting

Replacing a Windows-only dependency with a cross-platform one is the main work ahead. Keep each replacement behind the smallest interface that works, and keep the old path building until the new one is complete.

### Guidelines

A few guidelines that will make it easier to review and merge your changes:

* **Scope**
    * Keep your pull requests in scope and avoid unnecessary changes.
* **Commits**
    * Should group relevant changes together, the message should explain concisely what it's doing, there should be a longer summary elaborating if required.
    * Remove unnecessary commits and squash commits together where appropriate.
* **Formatting**
    * Your code should follow the style set in `.clang-format`.
    * Formatting can be done with `clang-format -i` on the files you touched.
* **Content**
    * Don't submit Roblox's own assets, binaries or branding. Replacements have to be your own work or openly licensed.
