/*
 * Copyright (c) 2020, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _COMMAND_PARSER_H
#define _COMMAND_PARSER_H

#if ARDUINO >= 100
# include <Arduino.h>
#else
# include <WProgram.h>
#endif

#include <stdlib.h>

class CLIServer;

#define COMMAND(X) int X(int argc, char **argv)

#define CLI_IS_PREFIX 0x01

typedef struct _CLICommand {
    char *command;
    uint8_t flags;
    int (*function)(int, char **);
    struct _CLICommand *next;
} CLICommand;

class CommandParser {
    private:
        CLICommand *commands;
        char *prompt;
        boolean _caseSensitive;

    public:
        CommandParser();
        void setCaseInsensitive();
        void setCaseSensitive();
        boolean isCaseSensitive();
        void addCommand(const char *command, int (*function)(int, char **));
        void addPrefix(const char *command, int (*function)(int, char **));
        int process(char *txt);
};

#endif
