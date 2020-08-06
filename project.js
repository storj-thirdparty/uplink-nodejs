var uplink = require("bindings")("uplink");
//
const uploadResultStruct = require('./upload.js');
const downloadResultStruct = require('./download.js');
//
const errorhandle = require('./error.js');

//
class projectResultStruct {
    //Project handle
    constructor(project){
        this.project = project;
    }

    // function closes the project and all associated resources.
    // Input : None
    // Output : None
    async close(){
        await uplink.close_project(this.project).catch((error)=>{
            errorhandle.storj_exception(error.error.code,error.error.message)
        });
        
    }

    // function starts download to the specified key.
    // Iutput : Bucket Name (String) , ObjectPath (String) and Download Options (Object)
    // Onput : Download (Object)
    async download_object(bucketName,uploadPath,downloadOptions){
        var download = await uplink.download_object(this.project,bucketName,uploadPath,downloadOptions).catch((error)=>{
            errorhandle.storj_exception(error.error.code,error.error.message)
        });
        var downloadResultReturn = new downloadResultStruct(download.download);
        return(downloadResultReturn);
    }

    // function starts an upload to the specified key.
    // Iutput : Bucket Name (String) , ObjectPath (String) and Download Options (Object)
    // Onput : Upload (Object)
    async upload_object(bucketName,uploadPath,uploadOptions){
        var upload = await uplink.upload_object(this.project,bucketName,uploadPath,uploadOptions).catch((error)=>{
            errorhandle.storj_exception(error.error.code,error.error.message)
        });
        var uploadResultReturn = new uploadResultStruct(upload.upload);
        return(uploadResultReturn);
    }

    // function returns a list of objects with all its information.
    //Input : BucketName (String) , ListObjectOptions (Object)
    //Output : ObjectList (Object)
    async list_objects(bucketName,listObjectsOptions){
       var objectlist = await uplink.list_objects(this.project,bucketName,listObjectsOptions).catch((error)=>{
        errorhandle.storj_exception(error.error.code,error.error.message)    
        });
        return objectlist;
    }

    // function deletes the object at the specific key.
    //Input : BucketName (String) , ObjectName (String)
    //Output : ObjectInfo (Object)
    async delete_object(bucketName,uploadPath){
        var objectinfo = await uplink.delete_object(this.project,bucketName,uploadPath).catch((error)=>{
            errorhandle.storj_exception(error.error.code,error.error.message)
        });
        return objectinfo;
    }

    // function returns information about an object at the specific key.
    //Input : BucketName (String) , ObjectName (String)
    //Output : ObjectInfo (Object)
    async stat_object(bucketName,uploadPath){
        var objectinfo = await uplink.stat_object(this.project,bucketName,uploadPath).catch((error)=>{
            errorhandle.storj_exception(error.error.code,error.error.message)
        });
        return objectinfo;
    }

    // function returns information about a bucket.
    // Input : BucketName (String)
    // Output : BucketInfo (Object)
    async stat_bucket(bucketName){
        var bucketInfo = await uplink.stat_bucket(this.project,bucketName).catch((error)=>{
            errorhandle.storj_exception(error.error.code,error.error.message)
        });
        return bucketInfo;
    }

    // function creates a new bucket.
    // Input : BucketName (String)
    // Output : BucketInfo (Object)
    async create_bucket(bucketName){
        var bucketInfo = await uplink.create_bucket(this.project,bucketName).catch((error)=>{
            errorhandle.storj_exception(error.error.code,error.error.message)
        });
        return bucketInfo;
    }

    // function ensures that a bucket exists or creates a new one.
    // When bucket already exists it returns a valid Bucket and no error
    // Input : BucketName (String)
    // Output : BucketInfo (Object)
    async ensure_bucket(bucketName){
        var bucketInfo = await uplink.ensure_bucket(this.project,bucketName).catch((error)=>{
            errorhandle.storj_exception(error.error.code,error.error.message)
        });
        return bucketInfo;
    }

    // function returns a list of buckets with all its information.
    // Input : ListBucketOptions (Object)
    // Output : List of Bucket Info (Object)
    async listbuckets(listBucketsOptions){
        var bucketListResult =  await uplink.list_buckets(this.project,listBucketsOptions).catch((error)=>{
            errorhandle.storj_exception(error.error.code,error.error.message)
        });
        return bucketListResult;
    }

    // function deletes a bucket.
    // When bucket is not empty it throws BucketNotEmptyError exception.
    // Input : BucketName (String)
    // Output : BucketInfo (Object)
    async delete_bucket(bucketName){
        var bucketInfo = await uplink.delete_bucket(this.project,bucketName).catch((error)=>{
            errorhandle.storj_exception(error.error.code,error.error.message)
        });
        return bucketInfo;
    }
}
module.exports = projectResultStruct;