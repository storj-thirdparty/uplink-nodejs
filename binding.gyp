{
  'targets': [
    {
      'target_name': 'storjnodejs',
      'sources': [ './libUplink.cc'],
      "include_dirs":["./"],
      "libraries":["<(module_root_dir)/libuplinkc.so"],
      "ldflags": ["-Wl,-rpath,'$$ORIGIN'"],
      "cflags_cc": ["-fexceptions","-fPIC","-Wno-unknown-pragmas"]
    }
  ]
}
