// config.js module

// gather environment variables
const dotenv = require('dotenv');

// read the .env file
// assign the variables to process.env
const result = dotenv.config();
// throws an error, if it failed
if (result.error) {
  throw result.error;
}
//
// collect the variables into the envs object
// return an object (named parsed), containing the content
const { parsed: envs } = result;

// export the variables
module.exports = envs;
