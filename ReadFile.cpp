#include "Akinator_header.h"

int FileLength(FILE* fp) {
    assert(fp != nullptr);

    fseek(fp, 0, SEEK_END);
    int pos = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return pos;
}


int LinesCount(char* buffer, int text_size) {
    assert(buffer != nullptr);

    int lines_count = 1;
    for (int i = 0; i < text_size; ++i) {
        if (buffer[i] == '\n') {
       		++lines_count;
        }
    }
    return lines_count;
}

int ReadFile(FILE* fp, int text_size, char* buffer) {
    assert(fp != nullptr);

    int real_text_size = fread(buffer, sizeof(char), text_size, fp);

    return real_text_size;
}

void MakeLines(char* buffer, Str* lines, int text_size) {
    assert(buffer != nullptr);
    assert(lines != nullptr);

    int lastI = 0;
    int line_no = 0;

    lines[line_no++].Str = buffer;

    for (int i = 0; i < text_size; ++i) {
        if (buffer[i] == '\n') {
            buffer[i] = '\0';
            lines[line_no - 1].length = i - lastI;
            lastI = i + 1;
            lines[line_no++].Str = buffer + i + 1;
        }
    }
    lines[line_no - 1].length = text_size - lastI;
}


void ReadTextAndMakeLines(Text_info* text, FILE* fp) {
    text->text_size = FileLength(fp);
    text->buffer = (char*) calloc(text->text_size + 1, sizeof(char));
    text->text_size = ReadFile(fp, text->text_size, text->buffer);
    text->lines_count = LinesCount(text->buffer, text->text_size);
    text->lines = (Str*) calloc(text->lines_count + 1, sizeof(Str));
    MakeLines(text->buffer, text->lines, text->text_size);
}

void WriteTextUpd(struct Text_info* text_info) {
    assert(text_info != nullptr);

    for (int i = 0; i < text_info->lines_count; ++i) {
        fputs(text_info->lines[i].Str, stdout);
        fputc('\n', stdout);
    }
}
