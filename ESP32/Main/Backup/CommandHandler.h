#ifndef TRINETRA_COMMAND_HANDLER_H
#define TRINETRA_COMMAND_HANDLER_H

#include <Arduino.h>

// Initialize command system
void commandHandlerBegin();

// Process incoming Android/app commands
void commandHandlerUpdate();

// Process a single command
void processCommand(String command);

#endif