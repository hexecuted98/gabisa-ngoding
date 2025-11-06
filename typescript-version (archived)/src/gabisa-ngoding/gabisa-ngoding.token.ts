/**
 * @file gabisa-ngoding.token.ts
 */

export enum TokenType {
    TOKEN_LOOP_START, //[
    TOKEN_LOOP_END, //]
    //-----------------------------------------//
    TOKEN_PTR_INCREMENT, //>
    TOKEN_PTR_DECREMENT, //<
    //-----------------------------------------//
    TOKEN_VAL_INCREMENT, //+
    TOKEN_VAL_DECREMENT, //-
    //-----------------------------------------//
    TOKEN_OUTPUT, //.
    TOKEN_INPUT //,
}

const KataKunci: Record<string, TokenType> = {
    "puh": TokenType.TOKEN_LOOP_START,
    "sepuh": TokenType.TOKEN_LOOP_END,
    //-----------------------------------------//
    "gw": TokenType.TOKEN_PTR_INCREMENT,
    "woy": TokenType.TOKEN_PTR_DECREMENT,
    //-----------------------------------------//
    "gabisa": TokenType.TOKEN_VAL_INCREMENT,
    "ngoding": TokenType.TOKEN_VAL_DECREMENT,
    //-----------------------------------------//
    "ajarin": TokenType.TOKEN_OUTPUT,
    "dong": TokenType.TOKEN_INPUT
}

export default KataKunci
export interface Token {
  type: TokenType;
  value: string;
  position: number; 
}