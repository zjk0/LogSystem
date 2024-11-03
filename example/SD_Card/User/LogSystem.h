#pragma once

// Include
#include "ffconf.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "diskio.h"
#include "integer.h"
#include "stdint.h"

// Marco
#define SUCCESS 1
#define FAIL 0

// Typedef
typedef struct {
    FATFS FatFS;              // File system object for log logical drive
    char Path[4];             // Log logical drive path
    FIL File;                 // File object for log
    uint8_t Buffer[_MAX_SS];  // Working buffer for log
    uint32_t WriteByte;       // Number of bytes written
    uint32_t ReadByte;        // Number of bytes read
} LogInformation;

// Variables
extern LogInformation Log;  // A LogInformation object

// Function
uint8_t LogSystem_Init (void);
uint8_t LogSystem_DeInit (void);
uint8_t LogSystem_CreateFile (char* FileName);
uint8_t LogSystem_OpenFile (char* FileName);
uint8_t LogSystem_CloseFile (void);
uint8_t LogSystem_WriteString (char* Str);
uint8_t LogSystem_WriteNumber (float Num);
uint8_t LogSystem_Write (char* Str, float Num);
