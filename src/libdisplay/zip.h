#pragma once

struct head
{
    int b;						  //字符
    long count;                   //文件中该字符出现的次数
    long parent, lch, rch;        //make a tree
    char bits[256];               //the huffuman code
};

//函数：compress()
//作用：读取文件内容并加以压缩
int compress(const char *filename,const char *outputfile);


//函数：uncompress()
//作用：解压缩文件，并将解压后的内容写入新文件
int uncompress(const char *filename,const char *outputfile);