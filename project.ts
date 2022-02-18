/* eslint-disable */
import {BucketInfo, ObjectInfo} from "./types.js";

import bindings = require("bindings");
const uplink = bindings("uplink");
//

import {DownloadResultStruct} from "./download.js";
const errorhandle = require("./error.js");

import {UploadResultStruct} from "./upload.js";
import {AccessResultStruct} from "./access.js";
//
//
export class ProjectResultStruct {

    project: any;

    // Project handle
    constructor (project: any) {

        this.project = project;

    }

    /*
     * Function closes the project and all associated resources.
     * Input : None
     * Output : None
     */
    async close (): Promise<void> {

        await uplink.close_project(this.project).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });

    }

    /*
     * Function starts download to the specified key.
     * Iutput : Bucket Name (String) , ObjectPath (String) and Download Options (Object)
     * Onput : Download (Object)
     */
    async downloadObject (bucketName: string, uploadPath: string, downloadOptions: Record<string, any>): Promise<DownloadResultStruct> {

        const download = await uplink.download_object(
                this.project,
                bucketName,
                uploadPath,
                downloadOptions
            ).catch((error: any) => {

                errorhandle.storjException(
                    error.error.code,
                    error.error.message
                );

            }),
            downloadResultReturn = new DownloadResultStruct(download.download);


        return downloadResultReturn;

    }

    /*
     * Function starts an upload to the specified key.
     * Iutput : Bucket Name (String) , ObjectPath (String) and Download Options (Object)
     * Onput : Upload (Object)
     */
    async uploadObject (bucketName: string, uploadPath: string, uploadOptions: Record<string, any>): Promise<UploadResultStruct> {

        const upload = await uplink.upload_object(
                this.project,
                bucketName,
                uploadPath,
                uploadOptions
            ).catch((error: any) => {

                errorhandle.storjException(
                    error.error.code,
                    error.error.message
                );

            }),
            uploadResultReturn = new UploadResultStruct(upload.upload);


        return uploadResultReturn;

    }

    /*
     *  Function returns a list of objects with all its information.
     * Input : BucketName (String) , ListObjectOptions (Object)
     * Output : ObjectList (Object)
     */
    async listObjects (bucketName: string, listObjectsOptions: Record<string, any>) {

        const objectlist = await uplink.list_objects(
            this.project,
            bucketName,
            listObjectsOptions
        ).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });


        return objectlist;

    }

    /*
     *  Function deletes the object at the specific key.
     * Input : BucketName (String) , ObjectName (String)
     * Output : ObjectInfo (Object)
     */
    async deleteObject (bucketName: string, uploadPath: string): Promise<ObjectInfo> {

        const objectinfo = await uplink.delete_object(
            this.project,
            bucketName,
            uploadPath
        ).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });


        return objectinfo;

    }

    /*
     *  Function returns information about an object at the specific key.
     * Input : BucketName (String) , ObjectName (String)
     * Output : ObjectInfo (Object)
     */
    async statObject (bucketName: string, uploadPath: string): Promise<ObjectInfo> {

        const objectinfo = await uplink.stat_object(
            this.project,
            bucketName,
            uploadPath
        ).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });


        return objectinfo;

    }

    /*
     * Function returns information about a bucket.
     * Input : BucketName (String)
     * Output : BucketInfo (Object)
     */
    async statBucket (bucketName: string): Promise<BucketInfo> {

        const bucketInfo = await uplink.stat_bucket(
            this.project,
            bucketName
        ).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });


        return bucketInfo;

    }

    /*
     * Function creates a new bucket.
     * Input : BucketName (String)
     * Output : BucketInfo (Object)
     */
    async createBucket (bucketName: string): Promise<BucketInfo> {

        const bucketInfo = await uplink.create_bucket(
            this.project,
            bucketName
        ).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });


        return bucketInfo;

    }

    /*
     * Function ensures that a bucket exists or creates a new one.
     * When bucket already exists it returns a valid Bucket and no error
     * Input : BucketName (String)
     * Output : BucketInfo (Object)
     */
    async ensureBucket (bucketName: string): Promise<BucketInfo> {

        const bucketInfo = await uplink.ensure_bucket(
            this.project,
            bucketName
        ).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });


        return bucketInfo;

    }

    /*
     * Function returns a list of buckets with all its information.
     * Input : ListBucketOptions (Object)
     * Output : List of Bucket Info (Object)
     */
    async listBuckets (listBucketsOptions: Record<string, any>) {

        const bucketListResult = await uplink.list_buckets(
            this.project,
            listBucketsOptions
        ).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });


        return bucketListResult;

    }

    /*
     * Function deletes a bucket.
     * When bucket is not empty it throws BucketNotEmptyError exception.
     * Input : BucketName (String)
     * Output : BucketInfo (Object)
     */
    async deleteBucket (bucketName: string): Promise<BucketInfo> {

        const bucketInfo = await uplink.delete_bucket(
            this.project,
            bucketName
        ).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });


        return bucketInfo;

    }

    /*
     * Function revokes the API key embedded in the provided access grant.
     * Input : Access (Object)
     * Output : None
     */
    async revokeAccess (access: AccessResultStruct): Promise<void> {

        await uplink.revoke_access(
            this.project,
            access.access,
        ).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });

    }

}
/* eslint-enable */
