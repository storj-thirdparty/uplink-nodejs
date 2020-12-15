/* eslint-disable */
import {ObjectInfo} from "./types";

import bindings = require("bindings");
const uplink = bindings("uplink");

const errorhandle = require("./error.js");

export class DownloadResultStruct {

    download: any;

    constructor (download: any) {

        this.download = download;

    }

    /*
     * Function downloads up to len size_to_read bytes from the object's data stream.
     * It returns the data_read in bytes and number of bytes read
     * Input : Buffer (Buf), Buffer length (Int)
     * Output : ReadResult (Int)
     */
    async read (buffer: Buffer, length: number): Promise<number> {

        const bytesread = await uplink.download_read(
            this.download,
            buffer,
            length
        ).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });


        return bytesread;

    }

    /*
     * Function returns information about the downloaded object.
     * Input : None
     * Output : ObjectInfo (Object)
     */
    async info (): Promise<ObjectInfo> {

        const objectInfo = await uplink.download_info(this.download).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });


        return objectInfo;

    }

    /*
     * Function closes the download.
     * Input : None
     * Output : None
     */
    async close (): Promise<void> {

        await uplink.close_download(this.download).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });

    }

}
/* eslint-enable */
