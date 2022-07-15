#include "../headers/main.h"

#include <stdio.h>
#include <stdlib.h>

#include "../headers/LabelTable.h"
#include "../headers/Translation.h"
#include "../headers/asm_descriptor.h"
#include "../headers/constants.h"
#include "../headers/label.h"
#include "../headers/macro_regiester.h"
#include "../headers/string_parsers.h"

AsmDescriptor* ds;

void for_each_cmd_param(char* cmd_param[], const int parm_count,
                        void callback(char*)) {
    int i;
    for (i = 1; i < parm_count; i++) {
        callback(cmd_param[i]);
    }
}

void run_pre_proc(char* asm_file_name) {
    char* spread_file_name = cp_string(asm_file_name);
    char* full_file_name = cp_string(asm_file_name);
    spread_file_name = cat_string(spread_file_name, SPREAD_FILE_SUFFIX);
    full_file_name = cat_string(full_file_name, ASM_FILE_SUFFIX);
    ds = new_asm_descriptor(full_file_name);
    generate_spread_file(spread_file_name);
    if (ds->err_log->has_errors) print_all_logger_errors(ds->err_log);
    free(spread_file_name);
    free(full_file_name);
    free_asm_descriptor(ds);
}

int main(int argc, char* argv[]) {
    char* twelve;
    Translation* trans = newTranslation();
    addTranslation(aToBin('a'), trans);
    addTranslation(NULL, trans);
    twelve = intToBinary(12);
    addTranslation(twelve, trans);
    shiftLeft(twelve, 6);
    addTranslation(twelve, trans);
    twelve = intToBinary(12);
    shiftLeft(twelve, 7);
    addTranslation(twelve, trans);
    twelve = intToBinary(-12);
    addTranslation(twelve, trans);
    printf(
        "expected:"
        "\n[0]:0001100001\n[1]:NULL\n[2]:0000001100\n[3]:1100000000\n[4]:"
        "1000000000\n[5]:1111110100\n");
    printf("actual value:\n");
    printTranslation(trans);
    printf("---------------\n");
    updateTranslationAtIndex(trans, 1, aToBin('b'));
    updateTranslationAtIndex(trans, 2, NULL);
    setARE(trans->binary[3], A);
    twelve = cp_string(trans->binary[3]);
    updateTranslationAtIndex(trans, 3, twelve);
    twelve = intToBinary(3);
    shiftLeft(twelve, 2);
    setARE(twelve, R);
    addTranslation(twelve, trans);
    printTranslation(trans);
    printf("binary:%s\ndecimal:%d\n", intToBinary(12),
           (int)binToDec(intToBinary(12)));
    return 0;
}
