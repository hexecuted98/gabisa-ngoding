let reset="\x1b[00m"
let merah="\x1b[91m"
let hijau="\x1b[92m"
let kuning="\x1b[93m"
let biru="\x1b[94m"

//logger info,debug,warn dan error
export default class log{
    static info(input:string){console.log(hijau+input+reset)}
    static debug(input:string){console.log(biru+input+reset)}
    static warn(input:string){console.log(kuning+input+reset)}
    static error(input:string){console.log(merah+input+reset)}
}

//ekstra
export{reset,merah,hijau,kuning,biru}