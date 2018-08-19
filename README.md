# Cube

Cube is a dynamically typed, class based object oriented scripting language with a syntax similar to Ruby.

## Examples

Only simple expressions are being parsed at the moment. There is a built in REPL
which can be accessed by running the `cube` executable without a script file name.

The typical operator precedence rules apply. The currently working precedence levels are
(from highest to lowest):

1. Unary Not (!), Negation (-)
2. Exponentiation (^)
3. Multiplication (\*), Division (/), Modulo (%)
4. Addition (+), Subtraction (-)
5. Comparison (<, <=, >, >=)
6. Equality (==), Inequality (!=)

The precedence levels can be overridden with parentheses.

```
> (2 + 20) * 100
-> 2200
> 2 + 20 * 100
-> 2002
```

Use Ctrl-C to exit the REPL.

## Getting Started

### Prerequisites

Building Cube has only been tested on macOS Sierra. It is very early in development.

The GNU Readline library provided by Homebrew is required for compilation.

```bash
brew install readline
```

### Building

Cube can be built in either debug or release mode. Running `make` alone will generate
the release build. Running `make debug` will generate the debug build.

The resulting builds can be found in the `build/release` or `build/debug` directories.

There is also a `make clean` target to clear out all of the build files.

### Installation

There is no install `make` target yet, all work is being done in development. I will fill
this in where there are more installation details.

## Authors

* David J Lains - Initial Work - [dlains](https://github.com/dlains)

