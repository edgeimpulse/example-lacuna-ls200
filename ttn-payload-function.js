function Decoder(bytes, port) {
    var isSatellite = false;

    if (bytes[0] === 0) {
        isSatellite = false;
    } else {
        isSatellite = true;
    }

    // Based on https://stackoverflow.com/a/37471538 by Ilya Bursov
    function bytesToFloat(bytes) {
    // JavaScript bitwise operators yield a 32 bits integer, not a float.
    // Assume LSB (least significant byte first).
    var bits = bytes[3]<<24 | bytes[2]<<16 | bytes[1]<<8 | bytes[0];
    var sign = (bits>>>31 === 0) ? 1.0 : -1.0;
    var e = bits>>>23 & 0xff;
    var m = (e === 0) ? (bits & 0x7fffff)<<1 : (bits & 0x7fffff) | 0x800000;
    var f = sign * m * Math.pow(2, e - 150);
    return f;
    }

    var b=1;
    var housesparrow = bytesToFloat(bytes.slice(b, b+=4)).toFixed(5);
    var noise = bytesToFloat(bytes.slice(b, b+=4)).toFixed(5);
    var roseringedparakeet = bytesToFloat(bytes.slice(b, b+=4)).toFixed(5);

    return {
        satellite: isSatellite,
        housesparrow: housesparrow,
        roseringedparakeet: roseringedparakeet,
        noise: noise
    };
}}
