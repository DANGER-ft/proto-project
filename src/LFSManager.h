#ifndef LFS_MANAGER_H
#define LFS_MANAGER_H

#include <Arduino.h>
#include <LittleFS.h>
#include <FS.h>
using fs::File;
// #define LFS_DEBUG

#ifdef LFS_DEBUG
#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#define DEBUG_PRINTLN(...)
#endif

class LFSManager
{
public:
  LFSManager(Stream *serialPort = &Serial);
  void begin();
  void tick();

private:
  Stream *_serial;
  char _cmdBuffer[256];
  size_t _cmdLen;

  enum State
  {
    IDLE,
    RECEIVING_FILE,
    SENDING_FILE
  };
  State _state;

  char _targetFile[64];
  uint32_t _expectedSize;
  uint32_t _expectedCRC;
  uint32_t _currentCRC;
  uint32_t _bytesProcessed;
  File _activeFile;

  uint8_t _receiveBuffer[256];
  size_t _chunkReceived;
  uint32_t _lastReceiveTime;

  void processCommand(char *cmd);
  void handleIdleState();
  void handleReceivingFile();
  void handleSendingFile();

  void sendInfo();
  void listFiles();
  void deleteFile(const char *path);
  void renameFile(const char *oldPath, const char *newPath);
  void formatFS();

  uint32_t calculateCRC32(const uint8_t *data, size_t len, uint32_t crc = 0xFFFFFFFF);
};

#endif
