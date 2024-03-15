#include <stdio.h>
#include <stdlib.h>

#define MAX_SAMPLE_LEN 16
#define MAX_STRING_LEN 256

typedef struct t_string
{
    int current_index;
    int len;
    char line[MAX_STRING_LEN];
} t_string;

t_string create_string()
{
    t_string tmp = {
        .current_index = 0,
        .len = 0,
        .line = { 0 }
    };

    return tmp;
}

int input_sample(t_string* str)
{
    for (int i = 0; i < MAX_SAMPLE_LEN + 1; ++i)
    {
        char c = 0;
        if (fread(&c, sizeof(char), 1, stdin) != 1)
        {
            return EXIT_FAILURE;
        }

        if (c == '\n')
        {
            str->len = i;
            return EXIT_SUCCESS;
        }

        str->line[i] = c;
    }

    return EXIT_FAILURE;
}

void input_str(t_string* str)
{
    str->len = (int)fread(str->line, sizeof(char), MAX_STRING_LEN, stdin);
    str->current_index = 0;
}

void prefix_function(int* shift_table, t_string* sample) 
{
    for (int i = 1; i < sample->len; ++i)
    {
		int j = shift_table[i - 1];
		while (j > 0 && sample->line[i] != sample->line[j])
        {
			j = shift_table[j-1];
        }

        if (sample->line[i] == sample->line[j])
        {
            ++j;
        }

		shift_table[i] = j;
	}
}

void print_shift_table(int sample_len, int* shift_table)
{
    for (int i = 0; i < sample_len; ++i)
    {
        printf("%d ", shift_table[i]);
    }
}

void print_protokol(int position, int len)
{
    printf("%d %d ", position, len);
}

int shift_str(int shift, int sample_len, t_string* string)
{
    if (string->current_index + sample_len + shift > string->len)
    {
        int shiftedSymCount = string->len - string->current_index - shift;
        for (int i = 0; i < shiftedSymCount; ++i)
        {
            string->line[i] = string->line[string->current_index + shift + i];
        }

        string->len = shiftedSymCount + (int)fread(string->line + shiftedSymCount, sizeof(char), MAX_STRING_LEN - shiftedSymCount, stdin);
        string->current_index = 0;
        return (string->len > sample_len);
    }

    string->current_index += shift;
    return 1;
}

void kmp(t_string* sample) 
{
    int shift_table[MAX_SAMPLE_LEN] = { 0 };
    prefix_function(shift_table, sample);
    print_shift_table(sample->len, shift_table);
    
    t_string str = create_string();
    input_str(&str);

    if (sample->len > str.len)
    {   
        return;
    }

    int position = 1;

    while (1)
    {
        int matched_len = 0;
        while (matched_len < sample->len && sample->line[sample->current_index + matched_len] == str.line[str.current_index + matched_len])
        {
            ++matched_len;
        }

        int shift = 1;
        if (matched_len != 0)
        {
            shift = matched_len - shift_table[matched_len - 1];
            print_protokol(position, matched_len);
        }

        position += shift;
        if (!shift_str(shift, sample->len, &str))
        {
            return;
        }
    }
}

int main()
{
    //FILE* in = freopen("in.txt", "r", stdin);
    t_string sample = create_string();
    if (input_sample(&sample)) 
    {
        return EXIT_FAILURE;
    }

    if (sample.len == 0) 
    {
        return EXIT_SUCCESS;
    }

    kmp(&sample);

    return EXIT_SUCCESS;
}