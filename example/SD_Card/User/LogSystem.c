#include "LogSystem.h"
#include "string.h"

// Variables
LogInformation Log;  // A LogInformation object

/**
 * @name LogSystem_Init
 * 
 * @brief Initialize log system
 * 
 * @param none
 * 
 * @return SUCCESS: Successfully initialize log system
 *         FAIL: Fail to initialize log system
 */
uint8_t LogSystem_Init (void) {
    if (f_mount(&Log.FatFS, (const TCHAR*)Log.Path, 0) == FR_OK && f_mkfs((const TCHAR*)Log.Path, FM_ANY, 0, Log.Buffer, sizeof(Log.Buffer)) == FR_OK) {
        return SUCCESS;
    }
    else {
        return FAIL;
    }
}

/**
 * @name LogSystem_DeInit
 * 
 * @brief Deinitialize log system
 * 
 * @param none
 * 
 * @return SUCCESS: Successfully deinitialize log system
 *         FAIL: Fail to deinitialize log system
 */
uint8_t LogSystem_DeInit (void) {
    if (f_mount(NULL, (const TCHAR*)Log.Path, 0) == FR_OK) {
        return SUCCESS;
    }
    else {
        return FAIL;
    }
}

/**
 * @name LogSystem_CreateFile
 * 
 * @brief Create a file in log system
 * 
 * @param FileName: Pointer to the name of the file which will be created
 * 
 * @return SUCCESS: Successfully create a file
 *         FAIL: Fail to create a file
 */
uint8_t LogSystem_CreateFile (char* FileName) {
    if (f_open(&Log.File, (const TCHAR*)FileName, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK) {
        return SUCCESS;
    }
    else {
        return FAIL;
    }
}

/**
 * @name LogSystem_CloseFile
 * 
 * @brief Close file
 * 
 * @param none
 * 
 * @return SUCCESS: Successfully close the file
 *         FAIL: Fail to close the file
 */
uint8_t LogSystem_CloseFile (void) {
    if (f_close(&Log.File) == FR_OK) {
        return SUCCESS;
    }
    else {
        return FAIL;
    }
}

/**
 * @name LogSystem_WriteString
 * 
 * @brief Write string to file
 * 
 * @param Str: Pointer to the string which will be written to file
 * 
 * @return SUCCESS: Successfully write string to file
 *         FAIL: Fail to write string to file
 */
uint8_t LogSystem_WriteString (char* Str) {
    if (f_write(&Log.File, (const TCHAR*)Str, strlen(Str), (void*)&Log.WriteByte) == FR_OK) {
        return SUCCESS;
    }
    else {
        return FAIL;
    }
}

/**
 * @name LogSystem_WriteNumber
 * 
 * @brief Write number to file
 * 
 * @param Num: The number which will be written to file
 * 
 * @return SUCCESS: Successfully write number to file
 *         FAIL: Fail to write number to file
 */
uint8_t LogSystem_WriteNumber (float Num) {
    int TempNum = (int)(Num * 1000000);
    int ToolNum = 1;
    int DecimalNum = 6;
    int WriteLength = 0;
    int count = 0;
    char WriteChar;

    while (TempNum != 0) {
        WriteLength++;
        TempNum /= 10;
    }

    if (WriteLength != 0) {
        count = WriteLength - 1;
    }

    while (count > 0) {
        ToolNum *= 10;
        count--;
    }

    TempNum = (int)(Num * 1000000);

    for (int i = 0; i < WriteLength; i++) {
        WriteChar = '0' + (TempNum / ToolNum);
        if (f_write(&Log.File, (const TCHAR*)&WriteChar, sizeof(WriteChar), (void*)&Log.WriteByte) != FR_OK) {
            return FAIL;
        }

        if (i == WriteLength - DecimalNum - 1) {
            WriteChar = '.';
            if (f_write(&Log.File, (const TCHAR*)&WriteChar, sizeof(WriteChar), (void*)&Log.WriteByte) != FR_OK) {
                return FAIL;
            }
        }

        TempNum = TempNum % ToolNum;
        ToolNum /= 10;
    }

    return SUCCESS;
}

/**
 * @name LogSystem_Write
 * 
 * @brief Write string and number to file
 * 
 * @param Str: Pointer to the string which will be written to file
 * @param Num: The number which will be written to file
 * 
 * @return SUCCESS: Successfully write string and number to file
 *         FAIL: Fail to write string and number to file
 */
uint8_t LogSystem_Write (char* Str, float Num) {
    if (LogSystem_WriteString(Str) == FAIL) {
        return FAIL;
    }
    if (LogSystem_WriteNumber(Num) == FAIL) {
        return FAIL;
    }
    if (LogSystem_WriteString("\r\n") == FAIL) {
        return FAIL;
    }
    return SUCCESS;
}