#include <iostream>
#include <stdio.h>
#include <string.h>

typedef const char* (*StrFunc)();
typedef void (*Action)();
typedef bool (*BoolFunc)();

struct Node {
    const char* (*GetTitle)();
    bool m_is_menu;
    struct Node** m_entries;
};

#define _UNIQUE_NAME_1_(a, b) a##_##b
#define _UNIQUE_NAME_0_(a, b) _UNIQUE_NAME_1_(a, b)
#define UNIQUE_NAME(name) _UNIQUE_NAME_0_(name, __LINE__)
#define ENTRIES_NAME(name) UNIQUE_NAME(__Entries##name)
#define FUNC_NAME(name) UNIQUE_NAME(__Func##name)

// Add GetIcon function pointer and so on
#define DECLARE_LEAF(name, title_funciton_pointer) static struct Node name = {title_funciton_pointer, false, nullptr};

// Add GetIcon function pointer and so on
#define DECLARE_MENU(name, title_funciton_pointer, ...)                  \
    static struct Node* ENTRIES_NAME(name)[] = {##__VA_ARGS__, nullptr}; \
    static struct Node name = {title_funciton_pointer, true, ENTRIES_NAME(name)};

//------------------------- test macro begin -----------------------
// For convenience of test: generate anonymous "GetTitle" function
#define UNIQUE_FUNC(name)                                      \
    const char* FUNC_NAME(name)() {                            \
        return "Title of " #name " in func: " __FUNCTION__ ""; \
    }

// For convenience of test: generate anonymous "GetTitle" function
#define DECLARE_LEAF_TEST(name) \
    UNIQUE_FUNC(name)           \
    static struct Node name = {FUNC_NAME(name), false, nullptr};

// For convenience of test: generate anonymous "GetTitle" function and anonymous sub menu array
#define DECLARE_MENU_TEST(name, ...)                                     \
    UNIQUE_FUNC(name)                                                    \
    static struct Node* ENTRIES_NAME(name)[] = {##__VA_ARGS__, nullptr}; \
    static struct Node name = {FUNC_NAME(name), true, ENTRIES_NAME(name)};

DECLARE_LEAF_TEST(Leaf1)
DECLARE_MENU_TEST(Menu1, &Leaf1)

DECLARE_LEAF_TEST(Leaf2)
DECLARE_MENU_TEST(Menu2, &Leaf2, &Menu1, &Leaf2, &Leaf1)

DECLARE_LEAF_TEST(Leaf3)
DECLARE_LEAF_TEST(Leaf4)
DECLARE_MENU_TEST(Root, &Leaf3, &Leaf4, &Menu2, &Leaf3, &Menu2, &Leaf2, &Menu1, &Leaf1)

/*
The above code expands to the following code:
const char* __FuncLeaf1_47() { return "Title of " "Leaf1" " in func: "  __FUNCTION__   ""; } static struct Node Leaf1 = {__FuncLeaf1_47, false, nullptr};
const char* __FuncMenu1_48() { return "Title of " "Menu1" " in func: "  __FUNCTION__   ""; } static struct Node* __EntriesMenu1_48[] = {&Leaf1, nullptr}; static struct Node Menu1 = {__FuncMenu1_48, true, __EntriesMenu1_48};

const char* __FuncLeaf2_50() { return "Title of " "Leaf2" " in func: "  __FUNCTION__   ""; } static struct Node Leaf2 = {__FuncLeaf2_50, false, nullptr};
const char* __FuncMenu2_51() { return "Title of " "Menu2" " in func: "  __FUNCTION__   ""; } static struct Node* __EntriesMenu2_51[] = {&Leaf2, &Menu1, &Leaf2, &Leaf1, nullptr}; static struct Node Menu2 = {__FuncMenu2_51, true, __EntriesMenu2_51};

const char* __FuncLeaf3_53() { return "Title of " "Leaf3" " in func: "  __FUNCTION__   ""; } static struct Node Leaf3 = {__FuncLeaf3_53, false, nullptr};
const char* __FuncLeaf4_54() { return "Title of " "Leaf4" " in func: "  __FUNCTION__   ""; } static struct Node Leaf4 = {__FuncLeaf4_54, false, nullptr};
const char* __FuncRoot_55() { return "Title of " "Root" " in func: "  __FUNCTION__   ""; } static struct Node* __EntriesRoot_55[] = {&Leaf3, &Leaf4, &Menu2, &Leaf3, &Menu2, &Leaf2, &Menu1, &Leaf1, nullptr}; static struct Node Root = {__FuncRoot_55, true, __EntriesRoot_55};
*/
//------------------------- test macro end -----------------------

void Travel(struct Node* p_node, const char* prefix) {
    printf("%s>%s Node[%p] with title: \"%s\"\n", prefix, (p_node->m_is_menu ? "Menu" : "Application"), p_node,
           p_node->GetTitle());
    char buf[64] = {0};
    strncpy_s(buf, prefix, sizeof(buf) - 1);
    for (char* p = buf; *p != '\0'; ++p) {
        if (*p == '-') {
            *p = ' ';
        }
    }
    strncat_s(buf, "|---", sizeof(buf) - 1);
    struct Node** pp_node = p_node->m_entries;
    if (pp_node != nullptr) {
        struct Node* p_child_node = *pp_node;
        while (p_child_node != nullptr) {
            Travel(p_child_node, buf);
            p_child_node = *++pp_node;
        }
    }
}

int main(int argc, char** argv) {
    Travel(&Root, "---");
    getchar();
    return 0;
}