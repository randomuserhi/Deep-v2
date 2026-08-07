import { readFile } from "node:fs/promises";
import { WASI } from "node:wasi";
import process from "node:process";
import path from "node:path";

const wasmPath =
    process.argv[2] ?? path.join(import.meta.dirname, "Tests.wasm");

const wasi = new WASI({
    version: "preview1",
    args: [
        "Tests.wasm",
        ...process.argv.slice(3),
    ],
    env: process.env,
    preopens: {},
    returnOnExit: true,
});

let instance = null;

const decoder = new TextDecoder();

function readCString(pointer) {
    if (pointer === 0) {
        return null;
    }

    /*
     * Recreate the view for every call because wasm memory growth can
     * replace the underlying ArrayBuffer.
     */
    const bytes = new Uint8Array(
        instance.exports.memory.buffer
    );

    let end = pointer;

    while (end < bytes.length && bytes[end] !== 0) {
        ++end;
    }

    if (end === bytes.length) {
        throw new RangeError(
            `Unterminated C string at address ${pointer}`
        );
    }

    return decoder.decode(
        bytes.subarray(pointer, end)
    );
}

const imports = {
    ...wasi.getImportObject(),

    deep_wasm: {
        on_assert_fail_impl(
            expressionPointer,
            filePointer,
            line,
            messagePointer
        ) {
            const expression =
                readCString(expressionPointer);

            const file =
                readCString(filePointer);

            const message =
                readCString(messagePointer);

            console.error(
                `${file}:${line}: assertion failed: ${expression}` +
                (message ? ` — ${message}` : "")
            );

            // Change to 0 to not trigger trap on assertion fail
            return 1;
        },
    },
};

const wasmModule = await WebAssembly.compile(
    await readFile(wasmPath)
);

instance = await WebAssembly.instantiate(
    wasmModule,
    imports
);

const exitCode = wasi.start(instance);

process.exitCode =
    Number.isInteger(exitCode) ? exitCode : 0;