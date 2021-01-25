/* eslint-disable */
import bindings = require("bindings");
const uplink = bindings("uplink");

import {ProjectResultStruct} from "./project.js";
const errorhandle = require("./error.js");

//
export class AccessResultStruct {

    access: any;

    constructor (access: any) {

        this.access = access;

    }

    /*
     * Function opens Storj(V3) project using access grant.
     * Input : None
     * Output : Project(Object)
     */
    async openProject (): Promise<ProjectResultStruct> {

        const project = await uplink.open_project(this.access).catch((error: any) => {

                errorhandle.storjException(
                    error.error.code,
                    error.error.message
                );

            }),
            projectResultReturn = new ProjectResultStruct(project.project);


        return projectResultReturn;

    }

    /*
     * Function opens Storj(V3) project using access grant and custom configuration.
     * Input : None
     * Output : Project(Object)
     */
    async configOpenProject (): Promise<ProjectResultStruct> {

        const project = await uplink.config_open_project(this.access).catch((error: any) => {

                errorhandle.storjException(
                    error.error.code,
                    error.error.message
                );

            }),
            projectResultReturn = new ProjectResultStruct(project.project);


        return projectResultReturn;

    }

    /*
     *
     * Function Share creates a new access grant with specific permissions.
     * Access grants can only have their existing permissions restricted, and the resulting
     * access grant will only allow for the intersection of all previous Share calls in the
     * access grant construction chain.
     * Prefixes, if provided, restrict the access grant (and internal encryption information)
     * to only contain enough information to allow access to just those prefixes.
     * Input : Permission (Object) , sharePrefixListArray (Array) , sharePrefixListArraylength (Int)
     * Output : Project(Object)
     */
    async share (permission: Record<string, any>, sharePrefixListArray: Array<any>, sharePrefixListArraylength: number): Promise<AccessResultStruct> {

        const sharedAccess = await uplink.access_share(
                this.access,
                permission,
                sharePrefixListArray,
                sharePrefixListArraylength
            ).catch((error: any) => {

                errorhandle.storjException(
                    error.error.code,
                    error.error.message
                );

            }),
            sharedAccessResultReturn = new AccessResultStruct(sharedAccess.access);


        return sharedAccessResultReturn;

    }

    /*
     * Function serializes an access grant such that it can be used later with ParseAccess or other tools.
     * Input : None
     * Output : SharedString (String)
     */
    async serialize (): Promise<string> {

        const stringResult = await uplink.access_serialize(this.access).catch((error: any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });


        return stringResult;

    }

    /*
     * Function serializes an access grant such that it can be used later with ParseAccess or other tools.
     * Input : bucket (String) , prefix (String) and Encrption key
     * Output : None
     */
    async overrideEncryptionKey (bucket: string, prefix:string, encryption_key:any): Promise<string> {

        const stringResult = await uplink.access_override_encryption_key(
            this.access,
            bucket,
            prefix,
            encryption_key
        ).catch((error:any) => {

            errorhandle.storjException(
                error.error.code,
                error.error.message
            );

        });


        return stringResult;

    }

}
/* eslint-enable */
