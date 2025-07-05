#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

int total_max = -1;//定义一个全局变量，记录第三个任务中所有最大字母组合的数量
int total_min = -1;//定义一个全局变量，记录第三个任务中所有最小字母组合的数量

struct maxdata {
    int length_s_max;
    char function3_result_1_max ;
    char function3_result_2_max ;
    char function3_result_3_max ;
}maxdatas[999];//定义一个结构体和对应的数组存储最大字母组合的信息

struct mindata {
    int length_s_min;
    char function3_result_1_min;
    char function3_result_2_min;
    char function3_result_3_min;
}mindatas[999];//定义一个结构体和对应的数组存储最小字母组合的信息

// 函数声明
const char* read_txt();//读取文本文档，返回读取字符串指针。
void function1_searchforspecialtext(const char* pattern, const char* text);
//任务一实现函数：实现匹配字符串的完整功能，其中有调用到computeLPSArray()函数
void computeLPSArray(const char* pattern, size_t M, unsigned long long int* lps);
//任务一过程函数：计算部分匹配表（LPS数组），供实现函数所调用
void function2_searchforRS(const std::string& text);
//任务二实现函数：实现寻找最长重复字母序列的功能
void function3_searchforTL(const char* text);
//任务三实现函数：实现求出三字母搭配的最多和最少情形及它们出现数量
void function1_searchforspecialtext_s(const char* pattern, const char* text);
//任务三过程函数：是功能一实现函数的改装，实现匹配字符串的完整功能，供实现函数所调用
void computeLPSArray_s(const char* pattern, size_t M, unsigned long long int* lps);
//任务三过程函数：是功能一过程函数的改装，实现计算部分匹配表（LPS数组），供实现函数所调用

int main() {
    const char* textAddress = read_txt();// 调用函数读取文本文件
    if (textAddress != nullptr) {// 检查指针是否为空
        //输出读取成功的检查信息
        std::cout << "读取文本文档内容成功！" << std::endl;
    }
    else {
        //输出读取失败的检查信息
        std::cerr << "读取文本文档内容失败！" << std::endl;
    }
    const char* text = textAddress;//textAddress和text一样，成为两个独立存储文本内容的变量
    std::string thestring;
    thestring = textAddress;//定义了一个string变量，并使其也存储文本内容
    std::cout<< "读取到了" << thestring.length() << "个字符" << std::endl;
    // 汇报读取内容的简要信息，即字数数量，让读者对于运行时间心里有数。
    std::cout<<"匹配字符串为zhongguo的时候, " << std::endl;
    function1_searchforspecialtext("zhongguo", text);
    //匹配字符串为zhongguo的时候，运行任务一的实现函数
    std::cout << "匹配字符串为deff的时候," << std::endl;
    function1_searchforspecialtext("deff",text);
    //匹配字符串为deff的时候，运行任务一的实现函数
    function2_searchforRS(thestring);
    //运行任务二的实现函数
    function3_searchforTL(textAddress);
    //运行任务三的实现函数
    delete[] textAddress;
    return 0;
}

const char* read_txt() {
    // 文件路径，你可以根据需要修改
    std::string filePath = "test.txt";
    // 打开文件
    std::ifstream fileStream(filePath);
    if (!fileStream.is_open()) {
        std::cerr << "打开文件出错： " << filePath << std::endl;
        return nullptr;  // 返回空指针表示出错
    }
    // 从文件读取内容到字符串
    std::string text((std::istreambuf_iterator<char>(fileStream)),
        std::istreambuf_iterator<char>());
    // 关闭文件
    fileStream.close();
    // 检查文件内容是否为空
    if (text.empty()) {
        std::cerr << "错误：文件为空" << std::endl;
        return nullptr;  // 返回空指针表示出错
    }
    // 将字符串拷贝到动态分配的内存中
    char* textCopy = new char[text.size() + 1];
    // 使用 strcpy_s 替代 strcpy
    strcpy_s(textCopy, text.size() + 1, text.c_str());
    // 返回字符串的地址
    return textCopy;
}

// 计算部分匹配表（LPS数组）
void computeLPSArray(const char* pattern, size_t M, unsigned long long int* lps) {
    unsigned long long int len = 0;// 初始化匹配长度为0
    lps[0] = 0;// 首个元素的LPS值为0

    unsigned long long int i = 1;
    while (i < M) {
        if (pattern[i] == pattern[len]) {// 当前字符匹配
            len++;// 增加匹配长度
            lps[i] = len;// 更新当前位置的LPS值
            i++;// 移动到下一个字符
        }
        else {
            if (len != 0) {
                len = lps[len - 1];// 回溯到前一个字符的LPS值
            }
            else {
                lps[i] = 0;// 当前字符无匹配，设置LPS值为0
                i++;// 移动到下一个字符
            }
        }
    }
}

// 使用KMP算法在文本中查找匹配的模式串
void function1_searchforspecialtext(const char* pattern, const char* text) {
    std::cout << "开始进行第一个任务程序，即找出模式串在文本中的匹配次数和对应的位置，结果如下：" << std::endl;
    // 获取模式串和文本串的长度
    size_t M = strlen(pattern);
    size_t N = strlen(text);
    // 存储匹配位置的向量
    std::vector<unsigned long long int> positions;
    // 记录匹配的数量
    unsigned long long int count = 0;
    // 分配存储部分匹配表（LPS数组）的内存
    unsigned long long int* lps = new unsigned long long int[M];
    // 计算部分匹配表
    computeLPSArray(pattern, M, lps);
    // 初始化文本和模式串的索引
    unsigned long long int i = 0; // 用于遍历text[]
    unsigned long long int j = 0; // 用于遍历pattern[]
    // KMP算法主循环
    while (i < N) {
        // 如果字符匹配，增加索引
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }
        // 如果整个模式串匹配，记录位置并调整索引
        if (j == M) {
            // 模式串在文本中的位置为 i - j
            positions.push_back(i - j);
            j = lps[j - 1];
            count++;
        }
        else if (i < N && pattern[j] != text[i]) {
            // 如果字符不匹配，根据部分匹配表调整索引
            if (j != 0) {
                j = lps[j - 1];
            }
            else {
                i++;
            }
        }
    }
    // 释放部分匹配表的内存
    delete[] lps;
    // 输出结果
    if(count!=0)
    { 
        std::cout << "模式串在文本中出现 " << count << " 次，位置分别为：\n";
        for (unsigned long long int pos : positions) {
            std::cout << pos + 1 << " ";
        }//输出模式串结果
        std::cout << std::endl;
    }
    else
    {
        std::cout << "模式串在文本中未出现" << std::endl;
    }//在未找到模式串的情况下也同样输出模式串结果
}

void function2_searchforRS(const std::string& text) {
    std::cout << "开始进行第二个任务，即找出最长重复字母、重复长度及长度，结果如下：" << std::endl;
    int maxLen = 0;// 记录最长重复长度的变量
    int maxPos = -1;// 记录最长重复字母的起始位置的变量
    char maxChar = '\0';// 记录最长重复字母的变量
    // 遍历文本字符串
    for (size_t i = 1; i < text.length(); i++) {
        char currentChar = text[i];// 当前字符

        size_t j = i - 1;// 寻找与当前字符相同的最远位置
        while (j >= 0 && text[j] == currentChar) {
            j--;
        }

        int len = i - j - 1;// 计算重复长度
        if (len > maxLen) {// 如果当前重复长度超过记录的最大长度
            maxLen = len;// 更新最大重复长度
            maxPos = j + 1;// 更新最长重复字母的起始位置
            maxChar = currentChar; // 更新最长重复字母
        }
    }
    std::cout << "最长的重复字母为: " << maxChar << ", 重复长度: " << maxLen + 1 << ", 位置: " << maxPos + 1 << std::endl;
}



void function3_searchforTL(const char* text) {
    std::cout<<"开始进行最后一个任务，将会已经计算出的字母组合及其数量，这虽然不是结果，但可以看到运行进度" << std::endl;
    std::cout<<"在打印完所有的内容后，会输出结果，即最多和最少的字母组合及其对应的数量" << std::endl;
    std::cout << "现在，进程将开始进行，如果文本量较大，那么程序运行时间将会非常长，请耐心等待" << std::endl;
    char pattern_s[]="aaa";
    for (char i = 'a'; i <= 'z'; ++i) {
        for (char j = 'a'; j <= 'z'; ++j) {
            for (char k = 'a'; k <= 'z'; ++k) {//遍历所有的三个字母组合
                pattern_s[0] = i;
                pattern_s[1] = j;
                pattern_s[2] = k;//把相应的字母进行赋值，形成完整的匹配字符串
                std::cout << pattern_s;//输出这个字符串，显示进度
                function1_searchforspecialtext_s(pattern_s,text);//调用函数进行KMP匹配
            }
        }
    }
    std::cout << "\n统计数量最大的字母组合是";
    for (int w=0;w<=total_max;w++)
    {
        std::cout << maxdatas[w].function3_result_1_max;
        std::cout << maxdatas[w].function3_result_2_max;
        std::cout << maxdatas[w].function3_result_3_max;//打出所有同时满足最大值的情况
        if (w!=total_max)
        {
            std::cout << "\t";
        }
        else
        {
            std::cout << "\n";
        }//这里需要分情况输出保证格式整洁
    }
    std::cout << "出现的次数是" <<maxdatas[0].length_s_max << std::endl;
    std::cout << "统计数量最小的字母组合是";
    for (int w = 0; w <= total_min; w++)
    {
        std::cout << mindatas[w].function3_result_1_min;
        std::cout << mindatas[w].function3_result_2_min;
        std::cout << mindatas[w].function3_result_3_min;//打出所有同时满足最小值的情况
        if (w != total_min)
        {
            std::cout << "\t";
        }
        else
        {
            std::cout << "\n";
        }//这里需要分情况输出保证格式整洁
    }
    std::cout << "出现的次数是" << mindatas[0].length_s_min << std::endl;
    
}

void function1_searchforspecialtext_s(const char* pattern, const char* text) {
    size_t M = strlen(pattern);
    size_t N = strlen(text);

    // 存储匹配位置的向量
    std::vector<unsigned long long int> positions;
    // 记录匹配的数量
    unsigned long long int count = 0;

    // 分配存储部分匹配表（LPS数组）的内存
    unsigned long long int* lps = new unsigned long long int[M];
    // 计算部分匹配表
    computeLPSArray_s(pattern, M, lps);

    // 初始化文本和模式串的索引
    unsigned long long int i = 0; // 用于遍历text[]
    unsigned long long int j = 0; // 用于遍历pattern[]

    // KMP算法主循环
    while (i < N) {
        // 如果字符匹配，增加索引
        if (pattern[j] == text[i]) {
            j++;
            i++;
        }

        // 如果整个模式串匹配，记录位置并调整索引
        if (j == M) {
            // 模式串在文本中的位置为 i - j
            positions.push_back(i - j);
            j = lps[j - 1];
            count++;
        }
        else if (i < N && pattern[j] != text[i]) {
            // 如果字符不匹配，根据部分匹配表调整索引
            if (j != 0) {
                j = lps[j - 1];
            }
            else {
                i++;
            }
        }
    }

    // 释放部分匹配表的内存
    delete[] lps;

    // 输出结果
    if (count != 0)
    {
        std::cout << count << "次";
        std::cout << "\t";//输出对应字符串的次数
        if ((maxdatas[total_max].length_s_max < count)&&(total_max!=-1))
        {
            total_max = 0;
            maxdatas[total_max].length_s_max = count;
            maxdatas[total_max].function3_result_1_max = pattern[0];
            maxdatas[total_max].function3_result_2_max = pattern[1];
            maxdatas[total_max].function3_result_3_max = pattern[2];//“打擂台”情况讨论，出现胜利者
        }
        else if (total_max == -1)
        {
            maxdatas[0].length_s_max = count;
            maxdatas[0].function3_result_1_max = pattern[0];
            maxdatas[0].function3_result_2_max = pattern[1];
            maxdatas[0].function3_result_3_max = pattern[2];//“打擂台”情况讨论，第一次特别处理
            total_max=0;
        }
        else if ((maxdatas[total_max].length_s_max == count) && (total_max != -1))
        {
            total_max++;
            maxdatas[total_max].length_s_max = count;
            maxdatas[total_max].function3_result_1_max = pattern[0];
            maxdatas[total_max].function3_result_2_max = pattern[1];
            maxdatas[total_max].function3_result_3_max = pattern[2];//“打擂台”情况讨论，出现平局
        }
        if ((mindatas[total_min].length_s_min > count) && (total_min != -1))
        {
            total_min = 0;
            mindatas[total_min].length_s_min = count;
            mindatas[total_min].function3_result_1_min = pattern[0];
            mindatas[total_min].function3_result_2_min = pattern[1];
            mindatas[total_min].function3_result_3_min = pattern[2];//“打擂台”情况讨论，出现胜利者
        }
        else if (total_min == -1)
        {
            mindatas[0].length_s_min = count;
            mindatas[0].function3_result_1_min = pattern[0];
            mindatas[0].function3_result_2_min = pattern[1];
            mindatas[0].function3_result_3_min = pattern[2];//“打擂台”情况讨论，第一次特别处理
            total_min=0;
        }
        else if ((mindatas[total_min].length_s_min == count) && (total_min != -1))
        {
            total_min++;
            mindatas[total_min].length_s_min = count;
            mindatas[total_min].function3_result_1_min = pattern[0];
            mindatas[total_min].function3_result_2_min = pattern[1];
            mindatas[total_min].function3_result_3_min = pattern[2];//“打擂台”情况讨论，出现平局
        }
    }
    else
    {
        std::cout << "字母组合在文本中未出现" << std::endl;//根本没有出现出现的情况
    }
}


void computeLPSArray_s(const char* pattern, size_t M, unsigned long long int* lps) {
    unsigned long long int len = 0;//初始化匹配长度为0
    lps[0] = 0;// 首个元素的LPS值为0

    unsigned long long int i = 1;
    while (i < M) {
        if (pattern[i] == pattern[len]) {// 当前字符匹配
            len++;//增加匹配长度
            lps[i] = len;// 更新当前位置的LPS值
            i++;// 移动到下一个字符
        }
        else {
            if (len != 0) {
                len = lps[len - 1];// 回溯到前一个字符的LPS值
            }
            else {
                lps[i] = 0;// 当前字符无匹配，设置LPS值为0
                i++;// 移动到下一个字符
            }
        }
    }
}