// Create a brainfuck interpreter
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_SIZE 100000
#define MAX_BUFFER_SIZE 100000
#define MAX_OUTPUT_SIZE 100000
#define MAX_INPUT_SIZE 100000
#define MAX_OUTPUT_SIZE 100000

typedef struct {
    char *program;
    int size;
} program_t;
typedef struct {
    char *buffer;
    int size;
} buffer_t;
typedef struct {
    char *buffer;
    int size;
} output_t;

// Interpret function
void interpret(program_t *program, buffer_t *input, output_t *output)
{
    char *ptr = program->program;
    char *input_ptr = input->buffer;
    char *output_ptr = output->buffer;

    while (*ptr != '\0') {
        if (*ptr == '+') {
            *input_ptr = *input_ptr + 1;
        }
        else if (*ptr == '-') {
            *input_ptr = *input_ptr - 1;
        }
        else if (*ptr == '>') {
            input_ptr++;
        }
        else if (*ptr == '<') {
            input_ptr--;
        }
        else if (*ptr == '.') {
            putchar(*input_ptr);
        }
        else if (*ptr == ',') {
            *input_ptr = getchar();
        }
        else if (*ptr == '[') {
            if (*input_ptr == 0) {
                while (*ptr != ']') {
                    ptr++;
                }
            }
        }
        else if (*ptr == ']') {
            if (*input_ptr != 0) {
                while (*ptr != '[') {
                    ptr--;
                }
            }
        }
        ptr++;
    }
    printf("Output: %s", output_ptr);
    free(program->program);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }
    program_t program;
    program.program = NULL;
    program.size = 0;
    buffer_t buffer;
    buffer.buffer = NULL;
    buffer.size = 0;
    output_t output;
    output.buffer = NULL;
    output.size = 0;

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    program.size = MAX_SIZE;
    program.program = malloc(program.size);
    if (program.program == NULL) {
        printf("Error: Could not allocate memory for program\n");
        return 1;
    }

    int i = 0;
    while (i < program.size) {
        int c = fgetc(file);
        if (c == EOF) {
            break;
        }
        program.program[i] = c;
        i++;
    }
    program.size = i;

    fclose(file);

    buffer.size = MAX_BUFFER_SIZE;
    buffer.buffer = malloc(buffer.size);
    if (buffer.buffer == NULL) {
        printf("Error: Could not allocate memory for buffer\n");
        return 1;
    }

    output.size = MAX_OUTPUT_SIZE;
    output.buffer = malloc(output.size);
    if (output.buffer == NULL) {
        printf("Error: Could not allocate memory for output\n");
        return 1;
    }

    char input[MAX_INPUT_SIZE];
    int input_size = 0;

    interpret(&program, &buffer, &output);
    
    return 0;
}