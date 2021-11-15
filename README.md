# Cli Arguments

String stack wrapper around program command line arguments.

## Build

The makefile generates the shared library in the `out/lib` folder.

### Via docker

```sh
docker run --rm -it -v $(pwd):$(pwd) --workdir $(pwd) abstrack/exo:0.2.2 make
```
