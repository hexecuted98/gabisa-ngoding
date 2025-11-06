/**
 * @file gabisa-ngoding.lexer
 */

import { GabisaNgoding_err } from '../helper/errors'
import KataKunci, { type Token } from './gabisa-ngoding.token'

export function GabisaNgoding_lexer(src:string): { tokennya: Token[], error?: GabisaNgoding_err } {
    const kata = src.trim().split(/\s+/);
    const tokennya: Token[] = [];

    for (let i = 0; i < kata.length; i++) {
        const katanya = kata[i];
        const jenisToken = KataKunci[katanya];
        if (jenisToken === undefined) {
            return { tokennya, error: GabisaNgoding_err.GBS_NGODING_ERROR_INVALID_TARGET };
        }

        tokennya.push({ type: jenisToken, value: katanya, position: i });
    }

    return { tokennya, error: GabisaNgoding_err.GBS_NGODING_ERROR_SUCCESS };
}