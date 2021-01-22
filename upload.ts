/* eslint-disable */
import {ObjectInfo} from "./types";

import bindings = require("bindings");
const uplink = bindings("uplink");
const errorhandle = require("./error.js");

export class UploadResultStruct {

    upload: any;

    constructor (upload: any) {

        this.upload = upload;

    }

    /*
     * Function uploads bytes data passed as parameter to the object's data stream.
     * Input : Buffer (Buf), Buffer length (Int)
     * Output : WriteResult (Int)
     */
    async write (buffer: Buffer, bytesread: number): Promise<Record<string, any>> {

        const writeResult = await uplink.upload_write(
            this.upload,
            buffer,
            bytesread
        ).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });


        return writeResult;

    }

    /*
     * Function to set custom meta information while uploading data
     * Input : customMetadata (Object)
     * Output : None
     */
    async setCustomMetadata (customMetadata: Record<string, any>): Promise<void> {

        await uplink.upload_set_custom_metadata(
            this.upload,
            customMetadata
        ).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });

    }

    /*
     * Function commits the uploaded data.
     * Input : None
     * Output : None
     */
    async commit (): Promise<void> {

        await uplink.upload_commit(this.upload).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });

    }

    /*
     * Function returns the last information about the uploaded object.
     * Input : None
     * Output : ObjectInfo
     */
    async info (): Promise<ObjectInfo> {

        const object = await uplink.upload_info(this.upload).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });


        return object;

    }

    /*
     * Function aborts an ongoing upload.
     * Input : None
     * Output : ObjectInfo
     */
    async abort (): Promise<ObjectInfo> {

        /* eslint-disable max-len */
        const object = await uplink.upload_abort(this.upload).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });

        return object;

    }

}
/* eslint-enable max-len,@typescript-eslint/no-explicit-any */
