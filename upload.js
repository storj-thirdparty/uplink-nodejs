var uplink = require("bindings")("uplink");
const errorhandle = require('./error.js');

//
class UploadResultStruct{
    constructor(upload){
        this.upload = upload;
    }

    // function uploads bytes data passed as parameter to the object's data stream.
    // Input : Buffer (Buf), Buffer length (Int)
    // Output : WriteResult (Int)
    async write(buffer,bytesread){
        var writeResult = await uplink.upload_write(this.upload,buffer,bytesread).catch((error) => {
            errorhandle.storjException(error.error.code,error.error.message);
        });
        return writeResult;
    }

    // function to set custom meta information while uploading data
    // Input : customMetadata (Object)
    // Output : None
    async setCustomMetadata(customMetadata){
        await uplink.upload_set_custom_metadata(this.upload,customMetadata).catch((error) => {
            errorhandle.storjException(error.error.code,error.error.message);
        });
    }

    // function commits the uploaded data.
    // Input : None
    // Output : None
    async commit(){
        await uplink.upload_commit(this.upload).catch((error) => {
            errorhandle.storjException(error.error.code,error.error.message);
        });
    }

    // function returns the last information about the uploaded object.
    // Input : None
    // Output : ObjectInfo
    async info(){
        var object = await uplink.upload_info(this.upload).catch((error) => {
            errorhandle.storjException(error.error.code,error.error.message);
        });
        return object;
    }

    // function aborts an ongoing upload.
    // Input : None
    // Output : ObjectInfo
    async abort(){
        var object = await uplink.upload_abort(this.upload).catch((error) => {
            errorhandle.storjException(error.error.code,error.error.message);
        });
        return object;
    }
}

module.exports = UploadResultStruct;