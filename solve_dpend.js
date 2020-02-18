const fs       = require("fs")
const { WASI } = require("@wasmer/wasi")

const wasmFilePath = "./solve_dpend.wasm"

// Instantiate a new WASI Instance
let wasi = new WASI({
  args : [wasmFilePath]
, env  : {}
})

// *****************************************************************************
// Async function to run our WASM module/instance
const startWasiTask =
  async pathToWasmFile => {
    // Fetch our WASM File
    let wasmBytes = new Uint8Array(fs.readFileSync(pathToWasmFile)).buffer

    // Instantiate the WebAssembly file
    let { instance } = await WebAssembly.instantiate(wasmBytes, {
      wasi_unstable: wasi.wasiImport
    })

    // Start the WASI instance
    wasi.start(instance)
  }

// *****************************************************************************
// Everything starts here
startWasiTask(wasmFilePath)