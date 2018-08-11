# Cube

Cube is a dynamic, class-based, scripting language with a syntax similar to Ruby.

## Examples

Only simple expressions are being parsed at the moment. There is a built in REPL
which can be accessed by running the `cube` executable without a script file name.

```
> (2 + 20) * 100
 => 2200
> 2 + 20 * 100
 => 2002
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

