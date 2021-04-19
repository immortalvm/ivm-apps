#ifndef IVM_APP_SETTINGS
#define IVM_APP_SETTINGS

// The iVM app should load in three stages:
//   Stage 1: Loader - Uncompresses rest of app
//   Stage 2: Reader - Reads app from film
//   Stage 3: App - Application responsible for decoding rest of film

#ifndef IVM_INCLUDE_APP
#define IVM_INCLUDE_APP     1
#endif

#define LOGGING_ENABLED 1

#endif
