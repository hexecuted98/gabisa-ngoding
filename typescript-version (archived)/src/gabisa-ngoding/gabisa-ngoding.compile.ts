/**
 * @file gabisa-ngoding.compile.ts
 */

import { GabisaNgoding_OPCode } from "./gabisa-ngoding.opcode";
import { TokenType } from "./gabisa-ngoding.token";
import type { Token } from "./gabisa-ngoding.token";

export default function GabisaNgoding_compile(t:Token[]): Uint8Array {
  const bytecode: number[] = [];

  for (const token of t) {
    switch (token.type) {
      case TokenType.TOKEN_PTR_INCREMENT:
        bytecode.push(GabisaNgoding_OPCode.GBS_NGODING_PTR_INC);
        break;
      case TokenType.TOKEN_PTR_DECREMENT:
        bytecode.push(GabisaNgoding_OPCode.GBS_NGODING_PTR_DEC);
        break;
      case TokenType.TOKEN_VAL_INCREMENT:
        bytecode.push(GabisaNgoding_OPCode.GBS_NGODING_VAL_INC);
        break;
      case TokenType.TOKEN_VAL_DECREMENT:
        bytecode.push(GabisaNgoding_OPCode.GBS_NGODING_VAL_DEC);
        break;
      case TokenType.TOKEN_LOOP_START:
        bytecode.push(GabisaNgoding_OPCode.GBS_NGODING_LOOP_START);
        break;
      case TokenType.TOKEN_LOOP_END:
        bytecode.push(GabisaNgoding_OPCode.GBS_NGODING_LOOP_END);
        break;
      case TokenType.TOKEN_INPUT:
        bytecode.push(GabisaNgoding_OPCode.GBS_NGODING_INPUT);
        break;
      case TokenType.TOKEN_OUTPUT:
        bytecode.push(GabisaNgoding_OPCode.GBS_NGODING_OUTPUT);
        break;
    }
  }

  return new Uint8Array(bytecode);
}
