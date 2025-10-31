#!/usr/bin/env node

import yargs from "yargs";
import { hideBin } from "yargs/helpers";
import fs from "fs";
import path from "path";

import GabisaNgoding_compile from "../src/gabisa-ngoding/gabisa-ngoding.compile";
import { GabisaNgoding_lexer } from "../src/gabisa-ngoding/gabisa-ngoding.lexer";
import { GabisaNgoding_VM } from "../src/gabisa-ngoding/gabisa-ngoding.vm";

function runGabisaNgodingFile(filePath: string) {
  const absPath = path.resolve(filePath);

  if (!fs.existsSync(absPath)) {
    console.error(`Contoh: gabisa-ngoding file.gabisa-ngoding`);
    process.exit(1);
  }

  const src = fs.readFileSync(absPath, "utf8");
  const tok = GabisaNgoding_lexer(src);
  const bytecode = GabisaNgoding_compile(tok.tokennya);
  const vm = new GabisaNgoding_VM(bytecode);

  try {
    vm.run();
  } catch (e: any) {
    console.error(`\n${e.message || e}`);
    process.exit(1);
  }
}

yargs(hideBin(process.argv))
  .scriptName("gabisa-ngoding")
  .usage("$0 <cmd> [args]")
  .command(
    "run <file>",
    "Jalankan file bahasa GabisaNgoding (.gbng)",
    (yargs) => {
      return yargs.positional("file", {
        describe: "Path ke file sumber .gbng",
        type: "string",
      });
    },
    (argv) => {
      runGabisaNgodingFile(argv.file as string);
    }
  )
  .command(
    "lex <file>",
    "Tampilkan token hasil lexer",
    (yargs) => {
      return yargs.positional("file", {
        describe: "Path ke file sumber .gbng",
        type: "string",
      });
    },
    (argv) => {
      const absPath = path.resolve(argv.file as string);
      const source = fs.readFileSync(absPath, "utf8");
      const tokens = GabisaNgoding_lexer(source);
      console.log(tokens.tokennya);
    }
  )
  .command(
    "compile <file>",
    "Tampilkan bytecode hasil kompilasi",
    (yargs) => {
      return yargs.positional("file", {
        describe: "Path ke file sumber .gabisa-ngoding",
        type: "string",
      });
    },
    (argv) => {
      const absPath = path.resolve(argv.file as string);
      const source = fs.readFileSync(absPath, "utf8");
      const tokens = GabisaNgoding_lexer(source);
      const bytecode = GabisaNgoding_compile(tokens.tokennya);
      console.log(bytecode);
    }
  )
  .demandCommand(1, "Pilih salah satu perintah.")
  .help()
  .strict()
  .parse();
