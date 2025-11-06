/**
 * @file gabisa-ngoding.vm.ts
 */

import { GabisaNgoding_OPCode } from "./gabisa-ngoding.opcode";
import atasiError, { GabisaNgoding_err } from "../helper/errors";

export class GabisaNgoding_VM {
  private memo = new Uint8Array(30000);
  private pointer = 0;
  private ip = 0;

  constructor(private bytecode: Uint8Array) {}

  run() {
    const stack: number[] = [];

    while (this.ip < this.bytecode.length) {
      const instr = this.bytecode[this.ip];

      switch (instr) {
        case GabisaNgoding_OPCode.GBS_NGODING_PTR_INC:
          this.pointer++;
          if (this.pointer >= this.memo.length)
            atasiError(GabisaNgoding_err.GBS_NGODING_OUT_OF_BOUNDS)
          break;

        case GabisaNgoding_OPCode.GBS_NGODING_PTR_DEC:
          if (this.pointer === 0)
            atasiError(GabisaNgoding_err.GBS_NGODING_OUT_OF_BOUNDS)
          this.pointer--;
          break;

        case GabisaNgoding_OPCode.GBS_NGODING_VAL_INC:
          this.memo[this.pointer]++;
          break;

        case GabisaNgoding_OPCode.GBS_NGODING_VAL_DEC:
          this.memo[this.pointer]--;
          break;

        case GabisaNgoding_OPCode.GBS_NGODING_OUTPUT:
          process.stdout.write(String.fromCharCode(this.memo[this.pointer]));
          break;

        case GabisaNgoding_OPCode.GBS_NGODING_INPUT:
          break;

        case GabisaNgoding_OPCode.GBS_NGODING_LOOP_START:
          if (this.memo[this.pointer] === 0) {
            let depth = 1;
            while (depth > 0) {
              this.ip++;
              if (this.ip >= this.bytecode.length)
                atasiError(GabisaNgoding_err.GBS_NGODING_ERROR_UNCLOSED)
              if (this.bytecode[this.ip] === GabisaNgoding_OPCode.GBS_NGODING_LOOP_START) depth++;
              if (this.bytecode[this.ip] === GabisaNgoding_OPCode.GBS_NGODING_LOOP_END) depth--;
            }
          } else {
            stack.push(this.ip);
          }
          break;

        case GabisaNgoding_OPCode.GBS_NGODING_LOOP_END:
          if (stack.length === 0)
            atasiError(GabisaNgoding_err.GBS_NGODING_ERROR_UNCLOSED)
          
          if (this.memo[this.pointer] !== 0) {
            this.ip = stack[stack.length - 1];
          } else {
            stack.pop();
          }
          break;
      }

      this.ip++;
    }
  }
}