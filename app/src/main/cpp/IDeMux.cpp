//
// Created by Administrator on 2022/6/7.
//

#include "IDeMux.h"
void IDeMux::main() {
    while (!isExit) {
        XData data = read();
        if (data.size > 0) {
            notify(data);
        }
    }
}

void IDeMux::setName(char *name) {
    this->name = name;
}