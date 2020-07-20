## Flow Diagram

![](https://github.com/storj-thirdparty/uplink-nodejs/blob/master/README.assets/arch.drawio.png)



## Binding Functions

#### request_access_with_passphrase(String, String, String)
	* function request_access_with_passphrase requests satellite for a new access grant using a passhprase
	* pre-requisites: None
	* inputs: Satellite Address(String), API key(String), Passphrase(String)
	* output: Access(Object) or Error(Object)

#### config_request_access_with_passphrase(Object, String, String, String)
	* function config_request_access_with_passphrase requests satellite for a new access grant using a passhprase and config
	* pre-requisites: None
	* inputs: Config(Object), Satellite Address(String), API key(String), Passphrase(String)
	* output: Access(Object) or Error(Object)

####  parse_access(String)
	* function parse_access to parse serialized access grant string
	* pre-requisites: None
	* inputs: Serialized Access (String)
	* output: Access(Object) or Error(Object)

#### Access Related Functions

These functions require Access(Object) for calling.

	* *access_share(Object, Object, Object, Int)*
		* function access_share creates new access grant with specific permission. Permission will be applied to prefixes when defined
		* pre-requisites: parse_access
		* inputs: Access(Object), Permission(Object), Share Prefix(Object), Prefix Count(Int)
		* output: Access(Object) or Error(Object)

	* *access_serialize(Object)*
		* function access_serialize serializes access grant into a string.
		* pre-requisites: parse_access
		* inputs: Access(Object)
		* output: StringResult(Object)

	* *open_project(Object)*
		* function open_project opens project using access grant
		* pre-requisites: request_access_with_passphrase or config_request_access_with_passphrase
		* inputs: Access(Object)
		* output: Project(Object) or Error(Object)

	* *config_open_project(Object, Object)*
		* function config_open_project opens project using access grant and config
		* pre-requisites: request_access_with_passphrase or config_open_project
		* inputs: Config(Object), Access(Object)
		* output: Project(Object) or Error(Object)

#### Project Related Functions

These functions require Project(Object) for calling.

	* *close_project(Object)*
		* function close_project closes the project
		* pre-requisites: open_project
		* inputs: Project(Object)
		* output: Error(Object)

	* *stat_bucket(Object, String)*
		* function stat_bucket returns information about a bucket
		* pre-requisites: open_project
		* inputs: Project(Object), Bucket Name(String)
		* output: Bucket(Object) or Error(Object)

	* *ensure_bucket(Object, String)*
		* function ensure_bucket creates a new bucket and ignores the error when it already exists
		* pre-requisites: open_project
		* inputs: Project(Object), Bucket Name(String)
		* output: Bucket(Object) or Error(Object)

	* *create_bucket(Object, String)*
		* function create_bucket creates a new bucket
		* pre-requisites: open_project
		* inputs: Project(Object), Bucket Name(String)
		* output: Bucket(Object) or Error(Object)

	* *delete_bucket(Object, String)*
		* function delete_bucket deletes a bucket
		* pre-requisites: open_project
		* inputs: Project(Object), Bucket Name(String)
		* output: Bucket(Object) or Error(Object)

	* *list_buckets(Object, Object)*
		* function list_buckets lists buckets
		* pre-requisites: open_project
		* inputs: Project(Object), ListBucketsOptions(Object)
		* output: BucketList(Object) or Error(Object)

	* *stat_object(Object, String, String)*
		* function stat_object information about an object at the specific key
		* pre-requisites: open_project
		* inputs: Project(Object), Bucket Name(String),  Object Key(String)
		* output: ObjectResult(Object) or Error(Object)

	* *list_objects(Object, String, Object)*
		* function list_objects lists objects
		* pre-requisites: open_project
		* inputs: Project(Object), Bucket Name(String), ListObjectsOptions(Object)
		* output: ObjectList(Object)

	* *upload_object(Object, String, String, Object)*
		* function upload_object starts an upload to the specified key
		* pre-requisites: open_project
		* inputs: Project(Object), Bucket Name(String),  Object Key(String),  Upload Options(Object)
		* output: Upload(Object) or Error(Object)

	* *download_object(Object, String, String, Object)*
		* function download_object starts  download to the specified key
		* pre-requisites: open_project
		* inputs: Project(Object), Buxcket Name(String), Object Key(String), Download Options(Object)
		* output: Download(Object) or Error(Object)

	* *delete_object(Object, String, String)*
		* function delete_object deletes an object
		* pre-requisites: open_project
		* inputs: Project(Object), Bucket Name(String), Object Key(String)
		* output: Object(Object) or Error(Object)

#### Upload Related Funcitons

These functions require Upload(Object) for calling.

	* *upload_set_custom_metadata(Object, Object)*
		* function upload_set_custom_metadata set custom meta information
		* pre-requisites: upload_object
		* inputs: Upload(Object), CustomMetadata(Object)
		* output: Error(Object)

	* *upload_write(Object, Buffer, Int)*
		* function upload_write uploads len(p) bytes from p to the object's data stream
		* pre-requisites: upload_object
		* inputs: Upload(Object), Data(Buffer),  Length(Int)
		* output: WriteResult(Object) or Error(Object)

	* *upload_info(Object)*
		* function upload_info returns the last information about the uploaded object
		* pre-requisites: upload_object
		* inputs: Upload(Object)
		* output: Object(Object) or Error(Object)

	* *upload_commit(Object)*
		* function upload_commit commits the uploaded data
		* pre-requisites: upload_object
		* inputs: Upload(Object)
		* output: Error(Object)

	* *upload_abort(Object)*
		* function upload_abort aborts an upload
		* pre-requisites: upload_object
		* inputs: Upload(Object)
		* output: Error(Object)

#### Download Related Functions

These functions require Download(Object) for calling.

	* *close_download(Object)*
		* function close_download closes the download
		* pre-requisites: download_object
		* inputs: Download(Object)
		* output: Error(Object)

	* *download_read(Object, Buffer, Int)*
		* function download_readc downloads from object's data stream into bytes up to length amount
		* pre-requisites: download_object
		* inputs: Download(Object), Data(Buffer), Length(Int)
		* output: ReadResult(Object) or Error(Object)

	* *download_info(Object)*
		* function download_info returns information about the downloaded object
		* pre-requisites: download_object
		* inputs: Download(Object)
		* output: Object(Object) or Error(Object)

>NOTE: All the binding functions are asynchronous functions and return promises.


## Testing

* The project has been tested on the following operating systems:
```
* Windows
	* Version: 10 Pro
	* Processor: Intel(R) Core(TM) i3-5005U CPU @ 2.00GHz 2.00GHz
	* Node version : v10.18.1

* macOS Catalina
	* Version: 10.15.4
	* Processor: 2.5 GHz Dual-Core Intel Core i5
	* Node version : 13.6.0

* ubuntu
	* Version: 16.04 LTS
	* Processor: AMD A6-7310 APU with AMD Radeon R4 Graphics × 4
	* Node version : 10.12.0
```
