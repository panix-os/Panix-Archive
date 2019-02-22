/**
 * File: screen.cpp
 * Author: Keeton Feavel and James Osborne
 */

#include "screen.h"

/* Declaration of private functions */
int getCursorOffset();
void setCursorOffset(int offset);
int printChar(char c, int column, int row, char color);
int getOffset(int column, int row);
int getOffsetRow(int offset);
int getOffsetColumn(int offset);

/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

/**
 * Print a message on the specified location
 * If col, row, are negative, we will use the current offset
 */
void kprintAtPosition(char* message, int column, int row) {
    /* Set cursor if column/row are negative */
    int offset;
    if (column >= 0 && row >= 0) {
        offset = getOffset(column, row);
    } else {
        offset = getCursorOffset();
        row = getOffsetRow(offset);
        column = getOffsetColumn(offset);
    }

    /* Loop through message and print it */
    int i = 0;
    while (message[i] != 0) {
        offset = printChar(message[i++], column, row, YELLOW_ON_BLACK);   // TODO: Make color dynamic
        /* Compute row/column for next iteration */
        row = getOffsetRow(offset);
        column = getOffsetColumn(offset);
    }
}

void kprintBackspace() {
    int offset = getCursorOffset() - 2;
    int row = getOffsetRow(offset);
    int column = getOffsetColumn(offset);
    printChar(0x08, column, row, WHITE_ON_BLACK);
}

void kprint(char *message) {
    kprintAtPosition(message, -1, -1);
}

/**********************************************************
 * Private kernel functions                               *
 **********************************************************/

/**
 * Innermost print function for our kernel, directly accesses the video memory 
 *
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * Returns the offset of the next character
 * Sets the video cursor to the returned offset
 */
int printChar(char c, int column, int row, char color) {
    uint8_t* videoMemory = (uint8_t*) VIDEO_ADDRESS;
    if (!color) {
        color = WHITE_ON_BLACK;
    }

    /* Error control: print a red 'E' if the coords aren't right */
    if (column >= MAX_COLUMNS || row >= MAX_ROWS) {
        videoMemory[2 * (MAX_COLUMNS) * (MAX_ROWS) - 2] = 'E';
        videoMemory[2 * (MAX_COLUMNS) * (MAX_ROWS) - 1] = RED_ON_WHITE;
        return getOffset(column, row);
    }

    int offset;
    if (column >= 0 && row >= 0) {
        offset = getOffset(column, row);
    }
    else {
        offset = getCursorOffset();
    }

    switch(c) {
        case '\n':
            row = getOffsetRow(offset);
            offset = getOffset(0, row + 1);
            break;
        case 0x08:
            videoMemory[offset] = ' ';
            videoMemory[offset + 1] = color;
            break;
        default:
            videoMemory[offset] = c;
            videoMemory[offset + 1] = color;
            offset += 2;
            break;
    }

    /* Check if the offset is over screen size and scroll */
    if (offset >= MAX_ROWS * MAX_COLUMNS * 2) {
        for (int i = 1; i < MAX_ROWS; i++) {
            memoryCopy(((char*) getOffset(0, i)) + VIDEO_ADDRESS,
                       ((char*) getOffset(0, i - 1)) + VIDEO_ADDRESS,
                       MAX_COLUMNS * 2);
        }

        /* Blank last line */
        char* lastLine = ((char*) getOffset(0, MAX_ROWS - 1)) + VIDEO_ADDRESS;
        for (int i = 0; i < MAX_COLUMNS * 2; i++) {
            lastLine[i] = 0;
        }

        offset -= 2 * MAX_COLUMNS;
    }

    setCursorOffset(offset);
    return offset;
}

int getCursorOffset() {
    /* Use the VGA ports to get the current cursor position
     * 1. Ask for high byte of the cursor offset (data 14)
     * 2. Ask for low byte (data 15)
     */
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; 	/* High byte: << 8 */
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);

    return offset * 2; 									/* Position * size of character cell */
}

void setCursorOffset(int offset) {
    /* Similar to getCursorOffset, but instead of reading we write data */
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (uint8_t) (offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (uint8_t) (offset & 0xff));
}

void clearScreen() {
    int screen_size = MAX_COLUMNS * MAX_ROWS;
    char *screen = (char *)VIDEO_ADDRESS;

    for (int i = 0; i < screen_size; i++) {
        screen[i * 2] = ' ';
        screen[i * 2 + 1] = WHITE_ON_BLACK;
    }
    setCursorOffset(getOffset(0, 0));
}

int getOffset(int column, int row) { 
	return 2 * (row * MAX_COLUMNS + column);
}

int getOffsetRow(int offset) {
	return offset / (2 * MAX_COLUMNS);
}

int getOffsetColumn(int offset) {
	return (offset - (getOffsetRow(offset) * 2 * MAX_COLUMNS)) / 2;
}
