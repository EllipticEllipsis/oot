# The merging process

## Optional: Documentation

It is helpful to document the functions and variables in the actor before you Pull Request it. The aim is to provide code that is sufficiently clear to be self-documenting, but it is worth leaving a comment on anything you find obscure or confusing. (Pull Request reviews will let you know if you are leaving too many comments.) Useful things to do documentation-wise:

- Name all (or most) of the functions.
- Name all the variables in the actor struct.
- Create enums for params, and any other numbers that would benefit from that sort of clarity.

You can test things using the practice rom for a retail version (watches and memory view is especially helpful), as well as the generated rom with Project 64 and something like Spectrum. The most important things to know if using a retail version to test are:

- all the addresses will be different
- actor structs in debug have 10 bytes of padding at the end, so subtract `0x10` from any subsequent offsets.

If you want to use `diff` after renaming anything, particularly functions, remember to copy the `build/` folder into `expected/` so use the correct symbols. *Make sure that `make` gives `OK` before doing this, or you're going to get very confused.*

Finally, *if you are not sure what something does, either ask or leave it unnamed: it will be less confusing later if things are unnamed than if they are wrongly named*

## Preparing to PR

### Change the `spec`

Specifically, to use the automatically generated reloc, rather than the original.

### Delete the asm files.

We have a script that will detect and delete unused asm files, `find_unused_asm.py`. Running it bare will output a list of all the unused files, while passing `-d` will delete them.

This does not distinguish comments, so remove all commented references to the files themselves first. If you have left the `#pragma` lines in but commented out, the following regular expression will find them:
```
\n//.*#pragma .*?\)
```

### Format

Run the formatting script `format.sh`, to format the C files in the standard way we use.

### Merge master

To make sure the PR builds correctly with the current master, you need to merge `upstream/master` before you make the PR. This tends to break things, that you have to fix to get it to compile correctly again.

## Pull Requests

Push commits to your fork of the repository on GitHub, and then open a pull request. Name the PR something sensible, like

- `EnJj OK and documented` (if all the functions match and your documentation is fairly complete)
- `EnJj OK` (if all the functions match)
- `EnJj (n nonmatching)` (if you couldn't get one or more functions to work, but to the best of your knowledge they are equivalent code)
- `EnJj (1 nonequivalent)` (if you couldn't get one or more functions to work, and do not believe the code has the same effect)

and so on, although these four tend to cover most cases.


### Reviews

Pull requests may be reviewed by anyone (who knows enough about the conventions of the project), but all are usually reviewed by Fig and Roman.

To implement suggestions made in reviews, it is generally easier to be consistent if you push more commits from your local branch. It is quite possible that in the meantime some other PR has gone in, and git will ask you to merge master before you add more commits. This is normally fairly painless, although often you have to resolve merge conflicts. If in doubt, backup your work before doing anything, and ask in Discord before doing anything drastic, or if you don't understand what git is telling you.

There is no need to wait for your PR to be approved and committed before working on your next actor.