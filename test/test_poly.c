#include "c_oop.h"
#include "debug.h"

#include <stddef.h>
#include <stdio.h>

Class(Text, MEMBER(int m_length; char *m_text;), METHOD(void (*get_length)(void)), print);
Class(Tree, MEMBER(double m_circle), , change);

int song_print(Text *_in, void *param)
{
    printf(LOG_FMT("&&&&&&&&&&&&&&&&&&&& SONG: Test ###############\n", LOG_PURPLE));
    printf("%s\n", _in->m_text);
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    return 0;
};

int html_print(Text *_in, void *param)
{
    printf(LOG_FMT("-------- WebPage Test ------- \n", LOG_YELLOW));
    printf("%s\n", _in->m_text);
    printf("-------------------------");
    return 0;
}

int a_change(Tree *_in, void *param) { return 0; }

typedef struct
{
    int m_id;
} Test;

int main()
{
    // test type
    Test  test;

    // vtable test
    Text *song = new (Text, 33, "This is a simple love song, not that conflict song kind.", NULL, NULL, song_print);
    Text *html = new (
        Text, 9121,
        "`-----------------`\n\tWelCome To HTML Text\t\n\tThis is body Text: Hello World.\t\n`----------------`", NULL,
        NULL, html_print
    );

    Tree *a = new (Tree, 33.1313,NULL , a_change);

    print(song, NULL);
    print(html, NULL);

    delete (song, html);
    return 0;
}