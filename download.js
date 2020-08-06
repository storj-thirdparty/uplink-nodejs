var uplink = require("bindings")("uplink");
const errorhandle = require('./error.js');

class downloadResultStruct{
    constructor(download){
        this.download = download;
    }

    // function downloads up to len size_to_read bytes from the object's data stream.
    // It returns the data_read in bytes and number of bytes read
    // Input : Buffer (Buf), Buffer length (Int)
    // Output : ReadResult (Int)
    async read(buffer,length){
        var bytesread = await uplink.download_read(this.download,buffer,length).catch((error)=>{
            errorhandle.storj_exception(error.error.code,error.error.message)
        });
        return bytesread;
    }

    // function returns information about the downloaded object.
    // Input : None
    // Output : ObjectInfo (Object)
    async info(){
        var objectInfo = await uplink.download_info(this.download).catch((error)=>{
            errorhandle.storj_exception(error.error.code,error.error.message)
        });
        return objectInfo;
    }

    // function closes the download.
    // Input : None
    // Output : None
    async close(){
        await uplink.close_download(this.download).catch((error)=>{
            errorhandle.storj_exception(error.error.code,error.error.message)
        });
    }
}

module.exports = downloadResultStruct;