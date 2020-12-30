#include "Akinator_header.h"

char* GetText(char* file_name, size_t* text_size) {
    assert(file_name != NULL);

    struct stat info = {};
    stat(file_name, &info);
    size_t file_length = info.st_size;

    FILE* fp = fopen(file_name, "r");
    assert(fp != nullptr);

    char* data = (char*) calloc(file_length, sizeof(char));
    assert(data != nullptr);

    int obj_read = fread(data, sizeof(char), file_length, fp);

    //data[obj_read] = '\0';
    *text_size = obj_read;

    //printf("%s", data);

    fclose(fp);

    return data;
  }

Str* GetPreparedText(const char* file_in, char* text, size_t text_size, size_t lines_count) {
    assert(file_in != NULL);

    //lines_count = Get_Lines_Count(text, text_size);

    Str* line_pointers = (Str*) calloc(lines_count, sizeof(Str));
    MakeLinePointers(line_pointers, text, text_size);

    return line_pointers;
};

void UpdateInfo(struct TextInfo* text_info, char* file_name) {
    assert(file_name != NULL);

    size_t text_size = 0;
    size_t lines_count = 0;

    text_info->text = GetText(file_name, &text_size);
    text_info->text_size = text_size;

    ChangeChar(text_info->text, text_size, '\n', '\0');

    text_info->lines_count = GetLinesCount(text_info->text, text_size);

    text_info->line_pointers = (Str*) calloc(lines_count, sizeof(Str));
    MakeLinePointers(text_info->line_pointers, text_info->text, text_size);
}

void ChangeChar(char* text, size_t text_size, char old_c, char new_c) {
    assert(text != NULL);
    assert(text_size >= 0);

    for (int i = 0; i < text_size; i++) {
        if (text[i] == old_c) {
            text[i] = new_c;
        }
    }
}

size_t GetLinesCount(char* text, size_t text_size) {
    assert(text != NULL);
    assert(text_size >= 0);

    int i = 0;
    size_t lines_count = 0;
    for (i; i < text_size; i++) {
        if (text[i] == '\0') {
            lines_count++;
        }
    }

    return lines_count;
}

void MakeLinePointers(Str* line_pointers, char* text, size_t text_size) {

    assert(line_pointers != NULL);
    assert(text != NULL);

    size_t lastI    = 0;
    size_t num_line = 0;

    line_pointers[num_line++].value = text;

    for (int i = 0; i < text_size; ++i) {
        if (text[i] == '\0') {
            line_pointers[num_line - 1].length = i - lastI;
            lastI = i + 1;
            line_pointers[num_line++].value = &text[i+1];
        }
    }
    line_pointers[num_line - 1].length = text_size - lastI;

    /*
    int line_no = 0;
    int str_len = 0;
    line_pointers[0].value = text;

    for(int i = 0; i < text_size - 1; i++) {
        str_len++;
        if (text[i] == '\0') {
            line_pointers[line_no].length = str_len - 1;
            str_len = 0;
            line_no++;
            line_pointers[line_no].value = text + i + 1;
        }
    }*/
}

void WriteText(struct TextInfo* text_info) {
    assert(text_info != nullptr);

    for (int i = 0; i < text_info->lines_count; ++i) {
        fputs(text_info->line_pointers[i].value, stdout);
        fputc('\n', stdout);
    }
}
