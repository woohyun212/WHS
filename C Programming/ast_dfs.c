#include <stdio.h>
#include <stdlib.h>
#include "json_c.c"
#define PRINT_INDENT 4

char* read_file_as_string(const char* file_path)
{
    // 파일 크기만 큼 파일 읽기
    //https://dojang.io/mod/page/view.php?id=617
    int size;
    int count;

    FILE* fp = fopen(file_path, "r"); // path를 읽기 모드(r)로 열기.
    // 파일 포인터를 반환

    fseek(fp, 0, SEEK_END); // 파일 포인터를 파일의 끝으로 이동시킴
    size = ftell(fp); // 파일 포인터의 현재 위치를 얻음
    printf("File Read Success, File size: %d\n", size);

    char* buffer = malloc(size + 1); // 파일 크기 + 1바이트(문자열 마지막의 NULL)만큼 동적 메모리 할당
    memset(buffer, 0, size + 1); // 파일 크기 + 1바이트만큼 메모리를 0으로 초기화

    fseek(fp, 0, SEEK_SET); // 파일 포인터를 파일의 처음으로 이동시킴
    fread(buffer, size, 1, fp); // file path 에서 파일 크기만큼 값을 읽음

    fclose(fp); // 파일 포인터 닫기
    return buffer;
}

int step = 0;

void print_indent()
//indent 출력 함수
{
    for (int i = 0; i < step * PRINT_INDENT; i++) { printf(" "); }
}

bool json_has_key(json_value node, const char* key)
// 키 갖고 있는지 확인하는 함수
{
    if (node.type != JSON_OBJECT || node.value == NULL || key == NULL || *key == '\0')
    {
        return false;
    }

    json_object* j_obj = (json_object*)node.value;

    for (int i = 0; i <= j_obj->last_index; i++)
    {
        if (j_obj->keys[i] && strcmp(j_obj->keys[i], key) == 0)
        {
            return true;
        }
    }

    return false;
}

char* get_type_string(json_value node)
{
    char* node_type = json_get_string(node, "_nodetype");
    if (strcmp(node_type, "TypeDecl") == 0)
    {
        json_value inner = json_get(node, "type");
        return get_type_string(inner);
    }
    if (strcmp(node_type, "IdentifierType") == 0)
    {
        json_value names = json_get(node, "names");
        int n = json_len(names);
        int total_len = 0;
        for (int i = 0; i < n; i++)
        {
            total_len += strlen(json_get_string(names, i)) + 1; // 전체길이
        }

        char* result = malloc(total_len + 1);
        result[0] = '\0';
        for (int i = 0; i < n; i++)
        {
            strcat(result, json_get_string(names, i));
            if (i < n - 1)
                strcat(result, " ");
        }
        return result;
    }
    if (strcmp(node_type, "PtrDecl") == 0)
    {
        json_value inner = json_get(node, "type");
        char* inner_str = get_type_string(inner);
        int len = strlen(inner_str) + 2;
        char* result = malloc(len);
        snprintf(result, len, "*%s", inner_str);
        free(inner_str);
        return result;
    }
    if (strcmp(node_type, "ArrayDecl") == 0)
    {
        json_value inner = json_get(node, "type");
        char* base_type = get_type_string(inner);
        char* result = malloc(strlen(base_type) + 3);
        sprintf(result, "%s[]", base_type);
        free(base_type);
        return result;
    }

    return strdup(node_type);
}

char* extract_expr(json_value node)
{
    if (node.type != JSON_OBJECT) return strdup("None");
    char* nodetype = json_get_string(node, "_nodetype");

    if (strcmp(nodetype, "ID") == 0)
    {
        char* name = json_get_string(node, "name");
        return strdup(name);
    }
    else if (strcmp(nodetype, "Constant") == 0)
    {
        char* value = json_get_string(node, "value");
        return strdup(value);
    }
    else if (strcmp(nodetype, "BinaryOp") == 0)
    {
        char* op = json_get_string(node, "op");
        char* left = extract_expr(json_get(node, "left"));
        char* right = extract_expr(json_get(node, "right"));
        int len = strlen(left) + strlen(op) + strlen(right) + 5;
        char* result = malloc(len);
        snprintf(result, len, "(%s %s %s)", left, op, right);
        free(left);
        free(right);
        return result;
    }
    else if (strcmp(nodetype, "FuncCall") == 0)
    {
        json_value name_val = json_get(node, "name");
        char* name = extract_expr(name_val);

        // args 존재 확인
        json_value args_val;
        if (json_has_key(node, "args")) {
            json_value args_obj = json_get(node, "args");
                // args 속이 빈경우가 존재
            if (json_has_key(args_obj, "exprs")) {
                args_val = json_get(args_obj, "exprs");
            } else {
                args_val.type = JSON_UNDEFINED;
            }
        } else {
            args_val.type = JSON_UNDEFINED;
        }

        if (args_val.type != JSON_OBJECT && args_val.type != JSON_ARRAY) {
            int total_len = strlen(name) + 3; // name + "()" + null terminator
            char* result = malloc(total_len);
            snprintf(result, total_len, "%s()", name);
            free(name);
            return result;
        }

        int arg_count = json_len(args_val);
        int total_len = 0;
        char** args = malloc(sizeof(char*) * arg_count);
        for (int i = 0; i < arg_count; i++)
        {
            args[i] = extract_expr(json_get(args_val, i));
            total_len += strlen(args[i]) + 2;
        }
        total_len += strlen(name) + 3;
        char* result = malloc(total_len);
        strcpy(result, name);
        strcat(result, "(");
        for (int i = 0; i < arg_count; i++)
        {
            strcat(result, args[i]);
            if (i < arg_count - 1)
                strcat(result, ", ");
            free(args[i]);
        }
        strcat(result, ")");
        free(name);
        free(args);
        return result;
    }
    else if (strcmp(nodetype, "ArrayRef") == 0)
    {
        char* name = extract_expr(json_get(node, "name"));
        char* subscript = extract_expr(json_get(node, "subscript"));
        int len = strlen(name) + strlen(subscript) + 3;
        char* result = malloc(len);
        snprintf(result, len, "%s[%s]", name, subscript);
        free(name);
        free(subscript);
        return result;
    }

    return strdup(nodetype);
}

void ast_dfs(json_value node)
{
    if (node.type == JSON_ARRAY)
    {
        // 배열이면 그대로 다시 넣으
        int len = json_len(node);
        for (int i = 0; i < len; i++)
        {
            ast_dfs(json_get(node, i));
        }
        return;
    }

    if (node.type != JSON_OBJECT || !json_has_key(node, "_nodetype")) return;
    //  없으면 반환

    char* nodetype = json_get_string(node, "_nodetype");

    if (strcmp(nodetype, "FuncDef") == 0)
    {
        // 함수 선언
        char* name = json_get_string(json_get(node, "decl", "name"));
        // 가변 인자라 확인해보니깐, 타고타고 들어갈 수 있음.
        char* return_type = get_type_string(json_get(node, "decl", "type", "type"));
        json_value args_node = json_get(node, "decl", "type", "args");
        // 함수의 이름/반환타입/내부정보(매개변수 등) 확인
        print_indent();
        printf("Function: %s(", name);
        if (args_node.type == JSON_OBJECT && json_has_key(args_node, "params"))
        {
            json_value params = json_get(args_node, "params");
            // prarams(매개변수)
            int p_len = json_len(params);
            for (int i = 0; i < p_len; i++)
            {
                json_value param = json_get(params, i); // array임
                char* pname = json_get_string(param, "name");
                char* ptype = get_type_string(json_get(param, "type"));
                printf("%s %s", ptype, pname);
                if (i < p_len - 1) printf(", ");
                free(ptype); // 포인터 해제 for 메모리 관리 |내부적으로 malloc이라
            }
        }
        printf(") -> %s\n", return_type);
        free(return_type);
        step++;
        ast_dfs(json_get(node, "body")); // body는 다시 재귀로 뜯기
        step--;
    }
    else if (strcmp(nodetype, "Decl") == 0)
    {
        char* vname = json_get_string(node, "name");
        char* vtype = get_type_string(json_get(node, "type"));
        print_indent();
        printf("Variable: %s %s\n", vtype, vname);
        free(vtype);
    }
    else if (strcmp(nodetype, "Return") == 0)
    {
        char* expr = extract_expr(json_get(node, "expr"));
        print_indent();
        printf("Return: %s\n", expr);
        free(expr);
    }
    else if (strcmp(nodetype, "If") == 0)
    {
        char* cond = extract_expr(json_get(node, "cond"));
        print_indent();
        printf("If condition: %s\n", cond);
        free(cond);
        step++;
        ast_dfs(json_get(node, "iftrue"));
        step--;
        if (json_has_key(node, "iffalse"))
        {
            print_indent();
            printf("Else:\n");
            step++;
            ast_dfs(json_get(node, "iffalse"));
            step--;
        }
    }
    else if (strcmp(nodetype, "While") == 0)
    {
        char* cond = extract_expr(json_get(node, "cond"));
        print_indent();
        printf("While condition: %s\n", cond);
        free(cond);
        step++;
        ast_dfs(json_get(node, "stmt"));
        step--;
    }
    else if (strcmp(nodetype, "Compound") == 0)
    {
        ast_dfs(json_get(node, "block_items"));
    }
}

int main(int argc, char* argv[])
{
    char* file_path = argv[1];
    char* file_contents = read_file_as_string(file_path);
    json_value root = json_create(file_contents);

    json_value ext = json_get(root, "ext");
    ast_dfs(ext);

    free(file_contents);
}
