const uplink = require("bindings")("uplink");
import { ProjectResultStruct } from './project.js';
const errorhandle = require('./error.js');

//
export class AccessResultStruct {
    access: any;
    constructor(access: any){
        this.access = access;
    }

    //function opens Storj(V3) project using access grant.
    //Input : None
    //Output : Project(Object)
    async openProject(): Promise<ProjectResultStruct>{
        const project = await uplink.open_project(this.access).catch((error: any) => {
            errorhandle.storjException(error.error.code,error.error.message);
        });
        const projectResultReturn = new ProjectResultStruct(project.project);
        return(projectResultReturn);
    }

    //function opens Storj(V3) project using access grant and custom configuration.
    //Input : None
    //Output : Project(Object)
    async configOpenProject(): Promise<ProjectResultStruct> {
        const project = await uplink.config_open_project(this.access).catch((error: any) => {
            errorhandle.storjException(error.error.code,error.error.message);
        });
        const projectResultReturn = new ProjectResultStruct(project.project);
        return(projectResultReturn);
    }
    
    //
    //function Share creates a new access grant with specific permissions.
    //Access grants can only have their existing permissions restricted, and the resulting
    //access grant will only allow for the intersection of all previous Share calls in the
    //access grant construction chain.
    //Prefixes, if provided, restrict the access grant (and internal encryption information)
    //to only contain enough information to allow access to just those prefixes.
    //Input : Permission (Object) , sharePrefixListArray (Array) , sharePrefixListArraylength (Int)
    //Output : Project(Object)
    async share(permission: Record<string, any>, sharePrefixListArray: Array<any>, sharePrefixListArraylength: number): Promise<AccessResultStruct> {
        const sharedAccess = await uplink.access_share(this.access,permission,sharePrefixListArray,sharePrefixListArraylength).catch((error: any) => {
            errorhandle.storjException(error.error.code,error.error.message);
        });
        const sharedAccessResultReturn = new AccessResultStruct(sharedAccess.access);
        return(sharedAccessResultReturn);
    }

    //function serializes an access grant such that it can be used later with ParseAccess or other tools.
    //Input : None
    //Output : SharedString (String)
    async serialize(): Promise<string> {
        const stringResult = await uplink.access_serialize(this.access).catch((error: any) => {
            errorhandle.storjException(error.error.code,error.error.message);
        });
        return stringResult;
    }
}
