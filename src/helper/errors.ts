/**
 * @file errors.ts
*/

import { biru, reset } from "./logger";

export enum GabisaNgoding_err{
    GBS_NGODING_ERROR_SUCCESS,          
    GBS_NGODING_ERROR_MALLOC,           
    GBS_NGODING_ERROR_IO,               
    GBS_NGODING_ERROR_INVALID_TARGET,  
    GBS_NGODING_ERROR_UNEXPECTED_LOOP_END, 
    GBS_NGODING_ERROR_UNCLOSED,        
    GBS_NGODING_OUT_OF_BOUNDS,          
    GBS_NGODING_END_OF_FILE             
}

export default function atasiError(e: GabisaNgoding_err): never | void {
    switch (e) {
        case GabisaNgoding_err.GBS_NGODING_ERROR_SUCCESS:
            return; 
        case GabisaNgoding_err.GBS_NGODING_ERROR_MALLOC:
            console.log("{biru}GabisaNgoding{reset}: gagal alokasi memori (malloc error).".replace("{reset}", reset).replace("{biru}", biru));
        case GabisaNgoding_err.GBS_NGODING_ERROR_IO:
            console.log("{biru}GabisaNgoding{reset}: kesalahan input/output (IO error).".replace("{reset}", reset).replace("{biru}", biru));
        case GabisaNgoding_err.GBS_NGODING_ERROR_INVALID_TARGET:
            console.log("{biru}GabisaNgoding{reset}: pointer target tidak valid.".replace("{reset}", reset).replace("{biru}", biru));
        case GabisaNgoding_err.GBS_NGODING_ERROR_UNEXPECTED_LOOP_END:
            console.log("{biru}GabisaNgoding{reset}: ditemukan 'sepuh' tanpa 'puh' (loop end tak terduga).".replace("{reset}", reset).replace("{biru}", biru));
        case GabisaNgoding_err.GBS_NGODING_ERROR_UNCLOSED:
            console.log("{biru}GabisaNgoding{reset}: loop 'puh' tidak pernah ditutup (unclosed loop).".replace("{reset}", reset).replace("{biru}", biru));
        case GabisaNgoding_err.GBS_NGODING_OUT_OF_BOUNDS:
            console.log("{biru}GabisaNgoding{reset}: pointer keluar dari batas memori.".replace("{reset}", reset).replace("{biru}", biru));
        case GabisaNgoding_err.GBS_NGODING_END_OF_FILE:
            console.log("{biru}GabisaNgoding{reset}: mencapai akhir file (EOF).".replace("{reset}", reset).replace("{biru}", biru));
        default:
            console.log("{biru}GabisaNgoding{reset}: error code 0 [UNKNOWN].".replace("{reset}", reset).replace("{biru}", biru));
    }
}
