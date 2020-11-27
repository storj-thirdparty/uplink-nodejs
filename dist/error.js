var ERROR_INTERNAL = 0x02,
    ERROR_CANCELED = 0x03,
    ERROR_INVALID_HANDLE = 0x04,
    ERROR_TOO_MANY_REQUESTS = 0x05,
    ERROR_BANDWIDTH_LIMIT_EXCEEDED = 0x06,

    ERROR_BUCKET_NAME_INVALID = 0x10,
    ERROR_BUCKET_ALREADY_EXISTS = 0x11,
    ERROR_BUCKET_NOT_EMPTY = 0x12,
    ERROR_BUCKET_NOT_FOUND = 0x13,

    ERROR_OBJECT_KEY_INVALID = 0x20,
    ERROR_OBJECT_NOT_FOUND = 0x21,
    ERROR_UPLOAD_DONE = 0x22;

class StorjException extends Error {
  constructor(message, code, details) {
    super(StorjException);
    this.message = message;
    this.code = code;
    this.details = details;
  }
}

class InternalError extends StorjException {
  constructor(details) {
    super("internal error", ERROR_INTERNAL, details);
  }
}


class CancelledError extends StorjException {
    constructor(details) {
      super("operation cancelled", ERROR_CANCELED, details);
    }
  }


class InvalidHandleError extends StorjException {
  constructor(details) {
    super("invalid handle", ERROR_INVALID_HANDLE, details);
  }
}


class TooManyRequestsError extends StorjException {
    constructor(details) {
      super("too many requests", ERROR_TOO_MANY_REQUESTS, details);
    }
  }

  
class BandwidthLimitExceededError extends StorjException {
  constructor(details) {
    super("bandwidth limit exceeded", ERROR_BANDWIDTH_LIMIT_EXCEEDED, details);
  }
}


class BucketNameInvalidError extends StorjException {
    constructor(details) {
      super("invalid bucket name", ERROR_BUCKET_NAME_INVALID, details);
    }
  }

  
class BucketAlreadyExistError extends StorjException {
    constructor(details) {
      super("bucket already exists", ERROR_BUCKET_ALREADY_EXISTS, details);
    }
  }

  
class BucketNotEmptyError extends StorjException {
  constructor(details) {
    super("bucket is not empty", ERROR_BUCKET_NOT_EMPTY, details);
  }
}


class BucketNotFoundError extends StorjException {
    constructor(details) {
      super("bucket not found", ERROR_BUCKET_NOT_FOUND, details);
    }
  }

  
class ObjectKeyInvalidError extends StorjException {
  constructor(details) {
    super("invalid object key", ERROR_OBJECT_KEY_INVALID, details);
  }
}

class ObjectNotFoundError extends StorjException {
    constructor(details) {
      super("object not found", ERROR_OBJECT_NOT_FOUND, details);
    }
  }

class UploadDoneError extends StorjException {
  constructor(details) {
    super("upload completed", ERROR_UPLOAD_DONE, details);
  }
}

module.exports={
  storjException : function storjException(code, details) {
    switch(code) {
      case 0x02:throw new InternalError(details);
      case 0x03:throw new CancelledError(details);
      case 0x04:throw new InvalidHandleError(details);
      case 0x05:throw new TooManyRequestsError(details);
      case 0x06:throw new BandwidthLimitExceededError(details);
      case 0x10:throw new BucketNameInvalidError(details);
      case 0x11:throw new BucketAlreadyExistError(details);
      case 0x12:throw new BucketNotEmptyError(details);
      case 0x13:throw new BucketNotFoundError(details);
      case 0x20:throw new ObjectKeyInvalidError(details);
      case 0x21:throw new ObjectNotFoundError(details);
      case 0x22:throw new UploadDoneError(details);
      default:throw new InternalError(details);
    }
  },
  StorjException,
  InternalError,
  CancelledError,
  InvalidHandleError,
  TooManyRequestsError,
  BandwidthLimitExceededError,
  BucketNameInvalidError,
  BucketAlreadyExistError,
  BucketNotEmptyError,
  BucketNotFoundError,
  ObjectKeyInvalidError,
  ObjectNotFoundError,
  UploadDoneError
};
