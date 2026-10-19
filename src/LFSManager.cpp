#include "LFSManager.h"

LFSManager::LFSManager(Stream *serialPort)
{
    _serial = serialPort;
    _cmdLen = 0;
    _state = IDLE;
    _chunkReceived = 0;
    _lastReceiveTime = 0;
}

void LFSManager::begin()
{
    if (!LittleFS.begin())
    {
        DEBUG_PRINTLN("LFS: Mount Failed");
    }
}

void LFSManager::tick()
{
    switch (_state)
    {
    case IDLE:
    case SENDING_FILE:
        handleIdleState();
        break;
    case RECEIVING_FILE:
        handleReceivingFile();
        break;
    }
}

void LFSManager::handleIdleState()
{
    while (_serial->available())
    {
        char c = _serial->read();
        if (c == '\n')
        {
            _cmdBuffer[_cmdLen] = '\0';
            processCommand(_cmdBuffer);
            _cmdLen = 0;
        }
        else if (c != '\r' && _cmdLen < sizeof(_cmdBuffer) - 1)
        {
            _cmdBuffer[_cmdLen++] = c;
        }
    }
}

void LFSManager::processCommand(char *cmdStr)
{
    char *cmd = strtok(cmdStr, ":");
    if (!cmd)
        return;

    if (strcmp(cmd, "PING") == 0)
    {
        _serial->println("PONG:LFS_FM");
    }
    else if (strcmp(cmd, "INFO") == 0)
    {
        sendInfo();
    }
    else if (strcmp(cmd, "LIST") == 0)
    {
        listFiles();
    }
    else if (strcmp(cmd, "DEL") == 0)
    {
        char *path = strtok(NULL, ":");
        if (path)
            deleteFile(path);
    }
    else if (strcmp(cmd, "REN") == 0)
    {
        char *oldPath = strtok(NULL, ":");
        char *newPath = strtok(NULL, ":");
        if (oldPath && newPath)
            renameFile(oldPath, newPath);
    }
    else if (strcmp(cmd, "FMT") == 0)
    {
        formatFS();
    }
    else if (strcmp(cmd, "UPL_START") == 0)
    {
        char *path = strtok(NULL, ":");
        char *sizeStr = strtok(NULL, ":");
        char *crcStr = strtok(NULL, ":");

        if (path && sizeStr && crcStr)
        {
            strncpy(_targetFile, path, sizeof(_targetFile) - 1);
            _expectedSize = strtoul(sizeStr, NULL, 10);
            _expectedCRC = strtoul(crcStr, NULL, 16);

            _activeFile = LittleFS.open("/temp_upload", "w");
            if (_activeFile)
            {
                _state = RECEIVING_FILE;
                _bytesProcessed = 0;
                _currentCRC = 0xFFFFFFFF;
                _chunkReceived = 0;
                _lastReceiveTime = millis();
                _serial->println("READY");
            }
            else
            {
                _serial->println("ERR:FS_OPEN");
            }
        }
    }
    else if (strcmp(cmd, "DWN_START") == 0)
    {
        char *path = strtok(NULL, ":");
        if (path)
        {
            _activeFile = LittleFS.open(path, "r");
            if (_activeFile)
            {
                _expectedSize = _activeFile.size();
                _bytesProcessed = 0;
                _state = SENDING_FILE;
                _serial->print("DWN_HDR:");
                _serial->println(_expectedSize);
            }
            else
            {
                _serial->println("ERR:FS_OPEN");
            }
        }
    }
    else if (strcmp(cmd, "DWN_NEXT") == 0)
    {
        if (_state == SENDING_FILE)
            handleSendingFile();
    }
}

void LFSManager::handleReceivingFile()
{
    if (_bytesProcessed >= _expectedSize)
        return;

    uint32_t now = millis();
    size_t currentChunkSize = _expectedSize - _bytesProcessed;
    if (currentChunkSize > 256)
        currentChunkSize = 256;

    while (_serial->available() > 0 && _chunkReceived < currentChunkSize)
    {
        _receiveBuffer[_chunkReceived++] = _serial->read();
        _lastReceiveTime = now;
    }

    if (_chunkReceived >= currentChunkSize)
    {
        _activeFile.write(_receiveBuffer, currentChunkSize);
        _currentCRC = calculateCRC32(_receiveBuffer, currentChunkSize, _currentCRC);
        _bytesProcessed += currentChunkSize;

        _serial->println("ACK");
        _serial->flush();

        _chunkReceived = 0;
        _lastReceiveTime = millis();

        if (_bytesProcessed >= _expectedSize)
        {
            _activeFile.close();
            _currentCRC ^= 0xFFFFFFFF;
            _state = IDLE;

            if (_currentCRC == _expectedCRC)
            {
                LittleFS.remove(_targetFile);
                LittleFS.rename("/temp_upload", _targetFile);
                _serial->println("SUCCESS:CRC_OK");
            }
            else
            {
                LittleFS.remove("/temp_upload");
                _serial->println("ERR:CRC_MISMATCH");
            }
        }
    }
    else if (millis() - _lastReceiveTime > 2000)
    {
        _activeFile.close();
        LittleFS.remove("/temp_upload");
        _state = IDLE;
        _chunkReceived = 0;
        _serial->println("ERR:TIMEOUT");
    }
}

void LFSManager::handleSendingFile()
{
    uint8_t buffer[256];
    size_t toRead = _expectedSize - _bytesProcessed;
    if (toRead > sizeof(buffer))
        toRead = sizeof(buffer);

    if (toRead > 0)
    {
        size_t readNow = _activeFile.read(buffer, toRead);
        _serial->write(buffer, readNow);
        _serial->flush();
        _bytesProcessed += readNow;
    }

    if (_bytesProcessed >= _expectedSize)
    {
        _activeFile.close();
        _state = IDLE;
        _serial->println("\nSUCCESS:DWN_OK");
    }
}

void LFSManager::sendInfo()
{
    uint32_t total = 0, used = 0;
#ifdef ESP8266
    FSInfo fs_info;
    LittleFS.info(fs_info);
    total = fs_info.totalBytes;
    used = fs_info.usedBytes;
#elif defined(ESP32)
    total = LittleFS.totalBytes();
    used = LittleFS.usedBytes();
#endif
    _serial->print("INFO:");
    _serial->print(total);
    _serial->print(":");
    _serial->println(used);
}

void LFSManager::listFiles()
{
#ifdef ESP8266
    Dir dir = LittleFS.openDir("/");
    while (dir.next())
    {
        _serial->print("FILE:");
        if (dir.fileName().charAt(0) != '/')
            _serial->print("/");
        _serial->print(dir.fileName().c_str());
        _serial->print(":");
        _serial->println(dir.fileSize());
    }
#elif defined(ESP32)
    File root = LittleFS.open("/");
    File file = root.openNextFile();
    while (file)
    {
        _serial->print("FILE:");
        if (file.name()[0] != '/')
            _serial->print("/");
        _serial->print(file.name());
        _serial->print(":");
        _serial->println(file.size());
        file = root.openNextFile();
    }
#endif
    _serial->println("LIST_END");
}

void LFSManager::deleteFile(const char *path)
{
    if (LittleFS.remove(path))
        _serial->println("OK");
    else
        _serial->println("ERR:REMOVE");
}

void LFSManager::renameFile(const char *oldPath, const char *newPath)
{
    if (LittleFS.rename(oldPath, newPath))
        _serial->println("OK");
    else
        _serial->println("ERR:RENAME");
}

void LFSManager::formatFS()
{
    if (LittleFS.format())
        _serial->println("OK");
    else
        _serial->println("ERR:FORMAT");
}

uint32_t LFSManager::calculateCRC32(const uint8_t *data, size_t len, uint32_t crc)
{
    for (size_t i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++)
            crc = (crc >> 1) ^ (0xEDB88320 & (-(crc & 1)));
    }
    return crc;
}