/*
 * FormatTime.h
 *
 *  Created on: Oct 19, 2015
 *      Author: u99319
 */

std::string fmt(const std::string& fmt, ...) {
    int size = 200;
    std::string str;
    va_list ap;
    while (1) {
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char*)str.c_str(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {
            str.resize(n);
            return str;
        }
        if (n > -1)
            size = n + 1;
        else
            size *= 2;
    }
    return str;
}
