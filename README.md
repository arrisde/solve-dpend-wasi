# solve_dpend_wasi

Demo for compiling and running a simple double pendulum simulation as WebAssembly module.

## Prerequisites
* Recent 64bit Linux (tested on Ubuntu 18.04)
* GCC or Clang for the native build
* Python 3, pipenv
* NodeJS
* [Wasmer](http://wasmer.io), [wasmtime](https://github.com/bytecodealliance/wasmtime), [wasm3](https://github.com/wasm3/wasm3), ... whichever WASI-enabled runtime you'd like to try :smiley:

## Install
1. `pipenv sync`
2. `pipenv run wasienv install-sdk 8`
3. `npm i`

## Compile (native)
`./build_gcc.sh`

## Run (native)
`time ./solve_dpend`

## Compile (WASM)
`pipenv run sh ./build_wasicc.sh`

## Run (WASM)
* `time node solve_dpend.js`
* `time wasmer run solve_dpend.wasm`
* `time wasmtime solve_dpend.wasm`
* `time wasm3 solve_dpend.wasm` (don't be surprised, wasm3 is an interpreter!)
* Go to http://wasi.dev/polyfill, upload _solve_dpend.wasm_ file
* Go to http://webassembly.sh, type `wapm upload`, select _solve_dpend.wasm_ file, then type `solve_dpend`

## Credits
* C implementation of the double pendulum solver by [Mike Wheatland](http://www.physics.usyd.edu.au/~wheat/dpend_html/)


