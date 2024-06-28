#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define CONFIG_FILE "/etc/pxml.xml"

void print_usage() {
    printf("Usage: pxml <file.xml> [<key>]\n");
}

void print_help() {
    printf("pxml - a simple XML Parser\n\n");
    printf("Usage:\n");
    printf("  pxml <file.xml> [<key>]\n");
    printf("  pxml -v | --version\n");
    printf("  pxml -h | --help\n\n");
    printf("Options:\n");
    printf("  -h, --help       Show this help message\n");
    printf("  -v               Show version number\n");
    printf("  --version        Show detailed version information\n");
}

void print_value(xmlNode *node, const char *key) {
    xmlNode *cur_node = NULL;
    char *token = strtok(strdup(key), ".");

    while (token) {
        for (cur_node = node->children; cur_node; cur_node = cur_node->next) {
            if (cur_node->type == XML_ELEMENT_NODE && xmlStrcmp(cur_node->name, (const xmlChar *)token) == 0) {
                node = cur_node;
                break;
            }
        }
        token = strtok(NULL, ".");
    }

    if (node && node->children && node->children->type == XML_TEXT_NODE) {
        printf("%s\n", (char *)node->children->content);
    } else {
        printf("Key not found or not a text node.\n");
    }
}

void print_file_contents(const char *filename, const char *scheme) {
    char command[256];
    snprintf(command, sizeof(command), "pygmentize -l xml -f terminal256 -O style=%s %s | less -R", scheme, filename);
    system(command);
}

void print_version() {
    xmlDoc *doc = xmlReadFile(CONFIG_FILE, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Error: Could not parse file %s\n", CONFIG_FILE);
        return;
    }
    xmlNode *root_element = xmlDocGetRootElement(doc);
    xmlNode *current_node = NULL;
    for (current_node = root_element->children; current_node; current_node = current_node->next) {
        if (current_node->type == XML_ELEMENT_NODE && strcmp((const char *)current_node->name, "Release") == 0) {
            xmlNode *release_node = current_node->children;
            for (release_node = release_node; release_node; release_node = release_node->next) {
                if (release_node->type == XML_ELEMENT_NODE && strcmp((const char *)release_node->name, "Version") == 0) {
                    xmlChar *version = xmlNodeGetContent(release_node);
                    printf("%s\n", version);
                    xmlFree(version);
                    xmlFreeDoc(doc);
                    xmlCleanupParser();
                    return;
                }
            }
        }
    }
    fprintf(stderr, "Error: 'Version' element not found in XML config.\n");
    xmlFreeDoc(doc);
    xmlCleanupParser();
}

void print_detailed_version() {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    doc = xmlReadFile(CONFIG_FILE, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Error: Failed to parse XML config file %s.\n", CONFIG_FILE);
        return;
    }

    root_element = xmlDocGetRootElement(doc);
    if (root_element == NULL) {
        fprintf(stderr, "Error: Empty XML config document %s.\n", CONFIG_FILE);
        xmlFreeDoc(doc);
        return;
    }

    xmlNode *program_node = NULL;
    for (program_node = root_element->children; program_node; program_node = program_node->next) {
        if (program_node->type == XML_ELEMENT_NODE && xmlStrcmp(program_node->name, (const xmlChar *)"Program") == 0) {
            break;
        }
    }

    if (program_node) {
        xmlNode *name_node = NULL;
        for (name_node = program_node->children; name_node; name_node = name_node->next) {
            if (name_node->type == XML_ELEMENT_NODE && xmlStrcmp(name_node->name, (const xmlChar *)"Name") == 0) {
                xmlNode *name_value = name_node->children;
                if (name_value && name_value->type == XML_TEXT_NODE) {
                    printf("%s ", (char *)name_value->content);
                }
                break;
            }
        }
    }

    xmlNode *release_node = NULL;
    for (release_node = root_element->children; release_node; release_node = release_node->next) {
        if (release_node->type == XML_ELEMENT_NODE && xmlStrcmp(release_node->name, (const xmlChar *)"Release") == 0) {
            break;
        }
    }

    if (release_node) {
        xmlNode *version_node = NULL;
        for (version_node = release_node->children; version_node; version_node = version_node->next) {
            if (version_node->type == XML_ELEMENT_NODE && xmlStrcmp(version_node->name, (const xmlChar *)"Version") == 0) {
                xmlNode *version_value = version_node->children;
                if (version_value && version_value->type == XML_TEXT_NODE) {
                    printf("version %s", (char *)version_value->content);
                }
                break;
            }
        }

        xmlNode *release_type_node = NULL;
        for (release_type_node = release_node->children; release_type_node; release_type_node = release_type_node->next) {
            if (release_type_node->type == XML_ELEMENT_NODE && xmlStrcmp(release_type_node->name, (const xmlChar *)"Release") == 0) {
                xmlNode *release_type_value = release_type_node->children;
                if (release_type_value && release_type_value->type == XML_TEXT_NODE) {
                    printf(" - %s\n", (char *)release_type_value->content);
                }
                break;
            }
        }
    }

    xmlFreeDoc(doc);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_help();
        return 0;
    }

    if (strcmp(argv[1], "-v") == 0) {
        print_version();
        return 0;
    }

    if (strcmp(argv[1], "--version") == 0) {
        print_detailed_version();
        return 0;
    }

    const char *filename = argv[1];
    const char *key = argc > 2 ? argv[2] : NULL;

    struct stat buffer;
    if (stat(filename, &buffer) != 0) {
        fprintf(stderr, "Error: File %s not found.\n", filename);
        return 1;
    }

    xmlDoc *config_doc = xmlReadFile(CONFIG_FILE, NULL, 0);
    if (config_doc == NULL) {
        fprintf(stderr, "Error parsing config XML file: %s\n", CONFIG_FILE);
        return 1;
    }

    xmlNode *root_element = xmlDocGetRootElement(config_doc);
    xmlNode *config_node = NULL;
    char scheme[256] = "monokai";
    for (config_node = root_element->children; config_node; config_node = config_node->next) {
        if (config_node->type == XML_ELEMENT_NODE && xmlStrcmp(config_node->name, (const xmlChar *)"Config") == 0) {
            xmlNode *scheme_node = NULL;
            for (scheme_node = config_node->children; scheme_node; scheme_node = scheme_node->next) {
                if (scheme_node->type == XML_ELEMENT_NODE && xmlStrcmp(scheme_node->name, (const xmlChar *)"Scheme") == 0) {
                    xmlNode *scheme_value = scheme_node->children;
                    if (scheme_value && scheme_value->type == XML_TEXT_NODE) {
                        strncpy(scheme, (char *)scheme_value->content, sizeof(scheme));
                        scheme[sizeof(scheme) - 1] = '\0';
                    }
                    break;
                }
            }
            break;
        }
    }

    if (key) {
        xmlDoc *doc = xmlReadFile(filename, NULL, 0);
        if (doc == NULL) {
            fprintf(stderr, "Error parsing XML file: %s\n", filename);
            xmlFreeDoc(config_doc);
            return 1;
        }

        xmlNode *root_element = xmlDocGetRootElement(doc);
        print_value(root_element, key);
        xmlFreeDoc(doc);
    } else {
        print_file_contents(filename, scheme);
    }

    xmlFreeDoc(config_doc);
    return 0;
}
