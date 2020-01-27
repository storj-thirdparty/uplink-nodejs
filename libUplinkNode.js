//
var storjnodejs = require('bindings')('storjnodejs');
//
var UplinkConfig = {
    Volatile : {
        tls : {
            skip_peer_ca_whitelist : false,
            peer_ca_whitelist_path : ""
        },
        peer_id_version : "",
        max_inline_size : 0,
        max_memory : 0,
        dial_timeout : 0
    }
};
//
var ListOptions = {
    prefix: "",
    cursor: "",
    delimiter:"",
    recursive:false,
    direction:0,
    limit:0
};
//
var Caveat = {
    disallow_reads:false,
    disallow_writes:false,
    disallow_lists:false,
    disallow_deletes:false
}
//
var EncryptionRestriction = {
    bucket:"",
    path_prefix:""
}
//
var UploadOptions = {
    content_type : "",
    expires : 0
}
//
var CipherSuite = {
 STORJ_ENC_UNSPECIFIED : 0,
 STORJ_ENC_NULL        : 1,
 STORJ_ENC_AESGCM      : 2,
 STORJ_ENC_SECRET_BOX  : 3   
}
//
var BucketConfig = {
    path_cipher : 0,
    encryption_parameters : {cipher_suite:0,block_size:0},
    redundancy_scheme : {algorithm:0,share_size:0,required_shares:0,repair_shares:0,optimal_shares:0,total_shares:0}
}
//
var EncryptionParameters = {
    cipher_suite : CipherSuite,
    block_size : 0
};

//
var RedundancyAlgorithm = {
    STORJ_INVALID_REDUNDANCY_ALGORITHM : 0,
    STORJ_REED_SOLOMON                 : 1
};
//
var RedundancyScheme = {
    algorithm:RedundancyAlgorithm,
    share_size:0,
    required_shares:0,
    repair_shares:0,
    optimal_shares:0,
    total_shares:0
};
var BucketListOptions = {
    cursor:"",
    direction:0,
    limit:0
}
//exporting function and object
module.exports = {
    storj_nodejs : storjnodejs,
    UplinkConfig : UplinkConfig,
    ListOptions : ListOptions,
    Caveat : Caveat,
    EncryptionRestriction : EncryptionRestriction,
    UploadOptions : UploadOptions,
    RedundancyAlgorithm : RedundancyAlgorithm,
    CipherSuite : CipherSuite,
    EncryptionParameters : EncryptionParameters,
    BucketConfig : BucketConfig,
    RedundancyScheme : RedundancyScheme,
    BucketListOptions : BucketListOptions
}