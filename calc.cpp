#include <iostream>
#include <stack>
#include<cmath>
#include <algorithm>
#include<string>

const double Pi = acos(-1);

double Sin(double k) {
    return (round(sin(k) * 10000000) / 10000000);
}

double Cos(double k) {
    return (round(cos(k) * 10000000) / 10000000);
}

double ctg(double k) {
    double a = Cos(k);
    double b = Sin(k);
    return (a / b);
}

double tan(double k) {
    double a = Cos(k);
    double b = Sin(k);
    return (b / a);
}

struct Del {
    char zn;
    double val;
};

bool Math(std::stack<Del> &St_num, std::stack<Del> &St_sym) {
    Del item{};
    double a, b, c;
    a = St_num.top().val;
    St_num.pop();
    if (St_sym.top().zn == '+') {
        b = St_num.top().val;
        St_num.pop();
        c = a + b;
        item.zn = '0';
        item.val = c;
        St_num.push(item);
        St_sym.pop();
    } else if (St_sym.top().zn == '-') {
        b = St_num.top().val;
        St_num.pop();
        c = b - a;
        item.zn = '0';
        item.val = c;
        St_num.push(item);
        St_sym.pop();
    } else if (St_sym.top().zn == '*') {
        b = St_num.top().val;
        St_num.pop();
        c = a * b;
        item.zn = '0';
        item.val = c;
        St_num.push(item);
        St_sym.pop();
    } else if (St_sym.top().zn == '/') {
        b = St_num.top().val;
        if (a == 0) {
            std::cerr << "\ndo not divide by zero\n";
            return false;
        } else {
            St_num.pop();
            c = (b / a);
            item.zn = '0';
            item.val = c;
            St_num.push(item);
            St_sym.pop();
        }
    } else if (St_sym.top().zn == 's') {
        c = Sin(a);
        item.zn = '0';
        item.val = c;
        St_num.push(item);
        St_sym.pop();
    } else if (St_sym.top().zn == 'c') {
        c = Cos(a);
        item.zn = '0';
        item.val = c;
        St_num.push(item);
        St_sym.pop();
    } else if (St_sym.top().zn == 't') {
        if (Cos(a) == 0) {
            std::cerr << "\ntan doesnt exist!\n";
            return false;
        } else {
            c = tan(a);
            item.zn = '0';
            item.val = c;
            St_num.push(item);
            St_sym.pop();
        }
    } else if (St_sym.top().zn == 'g') {
        if (Sin(a) == 0) {
            std::cerr << "\nctg doesnt exist!\n";
            return false;
        } else {
            c = ctg(a);
            item.zn = '0';
            item.val = c;
            St_num.push(item);
            St_sym.pop();
        }
    } else if (St_sym.top().zn == 'e') {
        c = exp(a);
        item.zn = '0';
        item.val = c;
        St_num.push(item);
        St_sym.pop();
    } else {
        std::cerr << "\nError!\n";
        return false;
    }
    return true;
}

int Rang(char Ch) {
    if (Ch == 's' || Ch == 'c' || Ch == 't' || Ch == 'g' || Ch == 'e') return 3;
    if (Ch == '+' || Ch == '-')return 1;
    if (Ch == '*' || Ch == '/')return 2;
    else return 0;
}

std::string prepare(const std::string &in_str) {
    std::string str = in_str;
    bool f_um = false;
    std::string sybs = "(nsp+-*/";
    std::string tmp_fsymbs = "scte";
    std::string str_copy = str;
    unsigned int delta = 0;
    int bracket_balance = 0;
    int ignore_symbs_count = 0;
    int after_minus = 0; //0 - не опр. 1 - число 2 - откр. скобка 3 - функция
    unsigned int minus_count = std::count_if(str.begin(), str.end(), [](char c) { return c == '-'; });
    for (int _ = 0; _ < minus_count; _++) {
        for (int i = 0; i < str.size(); i++) {
            //change
            if (!f_um) {
                if (str[i] == '-' && ((i == 0) || (sybs.find(str[i - 1]) != std::string::npos))) {
                    f_um = true;
                    str_copy.insert(i + delta, "(0");
                    delta += 2;
                }
            } else {
                switch (after_minus) {
                    case 0:
                        if (str[i] >= '0' and str[i] <= '9' or str[i] == 'x' or str[i] == 'P') after_minus = 1;
                        else if (str[i] == '(') {
                            after_minus = 2;
                            bracket_balance++;
                        } else if (tmp_fsymbs.find(str[i]) != std::string::npos) {
                            after_minus = 3;
                            ignore_symbs_count++;
                        } else exit(2);
                        break;
                    case 1:
                        if ((str[i] < '0' or str[i] > '9') and (str[i] != '.')) {
                            after_minus = 0;
                            f_um = false;
                            str_copy.insert(i + delta, ")");
                            delta++;
                        }
                        break;
                    case 2:
                        if (str[i] == '(') {
                            bracket_balance++;
                            if (bracket_balance == 0) {
                                after_minus = 0;
                                f_um = false;
                                str_copy.insert(i + delta, ")");
                                delta++;
                                bracket_balance = 0;
                            }
                        } else if (str[i] == ')') {
                            bracket_balance--;
                            if (bracket_balance == 0) {
                                after_minus = 0;
                                f_um = false;
                                str_copy.insert(i + delta, ")");
                                delta++;
                                bracket_balance = 0;
                            }
                        }
                        break;
                    case 3:
                        if (ignore_symbs_count != 3) ignore_symbs_count++;
                        else if (str.substr(i - 3, 3) == "sin" or str.substr(i - 3, 3) == "cos" or
                                 str.substr(i - 3, 3) == "tan" or str.substr(i - 3, 3) == "ctg" or
                                 str.substr(i - 3, 3) == "exp") {
                            if (str[i] == '(') {
                                after_minus = 2;
                                ignore_symbs_count = 0;
                                bracket_balance++;
                            } else exit(2);
                        } else exit(2);
                        break;
                }
            }
        }
        str = str_copy;
        delta = 0;
    }
    return str_copy;
}

int main() {
    bool x_is_known = false;
    double x;
    bool f_num = false;
    std::string tmp_num;
    std::cout << "Calculator\n   Enter: ";
    bool flag = true;
    std::stack<Del> St_num;
    std::stack<Del> St_sym;
    Del item{};
    std::string Ch;
    bool first = true;
    std::cin >> Ch;
    Ch = prepare(Ch);
    std::cout << ">" << Ch << "<" << std::endl;
    std::string tmp;
    for (size_t i = 0; i < Ch.size(); i++) {
        if (i == 1) {
            first = false;
        }
        std::string fsymbs = "nsgp";
        std::string symbs = "-+*/nsgp";
        std::string alls = "nsgPx";
        if (!first)
            if ((Ch[i - 1] == '.' && (Ch[i] > '9' || Ch[i] < '0')) ||
                (symbs.substr(1, 3).find(*Ch.begin()) != std::string::npos) ||
                (symbs.find(*Ch.end()) != std::string::npos)) {
                std::cerr << "Error: incorrect input" << std::endl;
                exit(1);
            }
        switch (Ch[i]) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (tmp[0] == ')' || tmp[0] == 'x') {
                    std::cerr << "Error: symbol before number" << std::endl;
                    exit(1);
                }
                break;
            case '+':
            case '/':
            case '*':
            case '-':
                if (tmp[0] == '-' || tmp[0] == '+' || tmp[0] == '*' || tmp[0] == '/' || tmp[0] == 'n' ||
                    tmp[0] == 's' || tmp[0] == 'g' || tmp[0] == 'p') {
                    std::cerr << "Error: wrong place for operator" << std::endl;
                    exit(1);
                }
                break;
            case '(':
                if ((tmp[0] >= '0' && tmp[0] <= '9') || tmp[0] == 'x' || tmp[0] == 'P') {
                    std::cerr << "Error: operator miss" << std::endl;
                    exit(1);
                }
                break;
            case 'x':
            case 's':
            case 'c':
            case 't':
            case 'e':
            case 'P':
                if ((tmp[0] >= '0' && tmp[0] <= '9')  || (alls.find(tmp[0])!=std::string::npos)) {
                    std::cerr << "Error: Invalid expression [double function]" << std::endl;
                    exit(1);
                }
                break;
            case ')':
                if ((tmp[0] < '0' || tmp[0] > '9') && tmp[0] != 'x' && Ch[i-1] != ')' && Ch[i-1] != '(' && Ch[i-1] != 'P') {
                    std::cerr << "Error: bracket" << std::endl;
                    exit(1);
                }
                break;
            default:
                continue;
        }
        tmp = Ch[i];
        first = false;
        if (Ch[i] == 's' || Ch[i] == 'c' || Ch[i] == 't' || Ch[i] == 'e') {
            if (f_num) {
                item.zn = '0';
                item.val = std::stod(tmp_num);
                St_num.push(item);
                flag = false;
                f_num = false;
                tmp_num = "";
            }
            std::string vec = Ch.substr(i, 3);
            i += 2;
            if (vec[0] == 's' && vec[1] == 'i' && vec[2] == 'n') {
                item.zn = 's';
                item.val = 0;
                St_sym.push(item);
                continue;
            }
            if (vec[0] == 'c' && vec[1] == 'o' && vec[2] == 's') {
                item.zn = 'c';
                item.val = 0;
                St_sym.push(item);
                continue;
            }
            if (vec[0] == 't' && vec[1] == 'a' && vec[2] == 'n') {
                item.zn = 't';
                item.val = 0;
                St_sym.push(item);
                continue;
            }
            if (vec[0] == 'c' && vec[1] == 't' && vec[2] == 'g') {
                item.zn = 'g';
                item.val = 0;
                St_sym.push(item);
                continue;
            }
            if (vec[0] == 'e' && vec[1] == 'x' && vec[2] == 'p') {
                item.zn = 'e';
                item.val = 0;
                St_sym.push(item);
                continue;
            }
        }
        if (Ch[i] == 'P') {
            if (f_num) {
                item.zn = '0';
                item.val = std::stod(tmp_num);
                St_num.push(item);
                flag = false;
                f_num = false;
                tmp_num = "";
            }
            item.zn = '0';
            item.val = Pi;
            St_num.push(item);
            flag = false;
            continue;
        }
        if ((Ch[i] >= '0' && Ch[i] <= '9') || (Ch[i] == '-' && flag == 1) || Ch[i] == '.') {
            if (!f_num)
                f_num = true;
            tmp_num += Ch[i];
            flag = false;
            continue;
        }
        if (Ch[i] == 'x') {
            if (!x_is_known) {
                std::cout << "Enter x:";
                std::cin >> x;
                x_is_known = true;
            }
            item.zn = '0';
            item.val = x;
            St_num.push(item);
            flag = false;
            f_num = false;
            tmp_num = "";
            continue;
        }
        if (Ch[i] == '+' || Ch[i] == '-' && flag == 0 || Ch[i] == '*' || Ch[i] == '/') {
            if (f_num) {
                item.zn = '0';
                item.val = std::stod(tmp_num);
                St_num.push(item);
                flag = false;
                f_num = false;
                tmp_num = "";
            }
            if (St_sym.size() == 0) {
                item.zn = Ch[i];
                item.val = 0;
                St_sym.push(item);
                continue;
            }
            if (St_sym.size() != 0 && Rang(Ch[i]) > Rang(St_sym.top().zn)) {
                item.zn = Ch[i];
                item.val = 0;
                St_sym.push(item);
                continue;
            }
            if (St_sym.size() != 0 && Rang(Ch[i]) <= Rang(St_sym.top().zn)) {
                if (!Math(St_num, St_sym)) {
                    std::cerr << "Math broken" << std::endl;
                    return 0;
                }
                item.zn = Ch[i];
                item.val = 0;
                St_sym.push(item);
                continue;
            }
        }
        if (Ch[i] == '(') {
            if (f_num) {
                item.zn = '0';
                item.val = std::stod(tmp_num);
                St_num.push(item);
                flag = false;
                f_num = false;
                tmp_num = "";
            }
            item.zn = Ch[i];
            item.val = 0;
            St_sym.push(item);
            continue;
        }
        if (Ch[i] == ')') {
            if (f_num) {
                item.zn = '0';
                item.val = std::stod(tmp_num);
                St_num.push(item);
                flag = false;
                f_num = false;
                tmp_num = "";
            }
            while (St_sym.top().zn != '(') {
                if (!Math(St_num, St_sym)) {
                    return 0;
                } else continue;
            }
            St_sym.pop();
            continue;
        } else {
            std::cout << "\nIncorrectly entered!\n";
            system("pause");
            return 0;
        }
    }
    if (!tmp_num.empty()) {
        item.zn = '0';
        item.val = std::stod(tmp_num);
        St_num.push(item);
        flag = false;
        f_num = false;
        tmp_num = "";
    }
    while (!St_sym.empty()) {
        if (!Math(St_num, St_sym)) {
            return 0;
        } else continue;
    }
    std::cout << " Result: " << St_num.top().val << std::endl;
    return 0;
}
