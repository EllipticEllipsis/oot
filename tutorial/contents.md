# Getting started

## [Introduction to decomp](introduction.md)
- What we are doing
- Structure of the code

## Pre-decompilation
- Building the repo (read the README)
- Reference for how we use git and github in this project (forks, git commands, pull requests)
- Most of us use VSCode. (You can watch Fig's video to get an idea of how this can be used)
- Choosing a first actor

## Decompilation

- [Begining decompilation: order, Init and the actor struct](beginning_decomp.md)
	- order of decompilation
	- Init and common actor features
		- Initchains
		- Actors and dynapoly actors
		- Colliders
		- Skelanime
	
	- Matching
		- Using diff
		- control flow (branches) -> instruction ordering -> register allocation -> stack

- [The rest of the function in the actor](other_functions.md)
    - Order of decompilation
    - Action Functions and other functions

	- More on matching: the permuter

- [Draw functions](draw_functions.md)

- [Data, migration and non-migration](data.md)
	- Importing the data: early and late
	- Fake symbols
	- Inlining



## After Decompilation
- More git
- Deleting the assembly
- Format script
- Pull requests

## Appendices
- [Types, Structs and Padding](types_structs_padding.md)
- [Helper scripts](helper_scripts.md)
- Some notes on the basic structure of N64 MIPS
- Glossary
- Conventions
