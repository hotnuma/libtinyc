
#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

// gcc `xml2-config --cflags --libs` -o xmlexample libxml2-example.c

static int indent;

static void print_element_names(xmlNode *a_node)
{
    printf("%*s", indent, "");
    printf("%d print_element_names\n", indent);

    for (xmlNode *cur_node = a_node; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            printf("%*s", indent, "");
            printf("%d: %s\n", indent, cur_node->name);
        }

        if (cur_node->children)
        {
            indent+=4;
            print_element_names(cur_node->children);
            indent-=4;
        }
    }
}

int main()
{
    LIBXML_TEST_VERSION;

    const char *filepath = "/home/hotnuma/config/Thunar/uca.xml";
    xmlDoc *doc = xmlReadFile(filepath, NULL, 0);


    if (doc == NULL)
    {
        printf("error: could not parse file %s\n", filepath);
        exit(1);
    }

    // Get the root element node
    xmlNode *root_element = xmlDocGetRootElement(doc);

    print_element_names(root_element);

    xmlFreeDoc(doc);

    xmlCleanupParser();

    return 0;
}


