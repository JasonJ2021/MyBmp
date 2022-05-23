#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#include "zip.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

 
// void menu(){
//     printf("%24c*********************\n",' ');
//     printf("%24c**BMP压缩/解压程序**\n",' ');
//     printf("%24c*********************\n",' ');
//     printf("%17c*************************************\n",' ');
//     printf("%17c***       1.压缩(compress)        ***\n",' ');
//     printf("%17c***       2.解压缩(uncompress)    ***\n",' ');
//     printf("%17c***       3.退出(quit)            ***\n",' ');
//     printf("%17c*************************************\n",' ');
// }

static struct head header[512], tmp;  //节点树
 
//函数：compress()
//作用：读取文件内容并加以压缩
int compress(const char *filename,const char *outputfile)
{
    char buf[512];
    unsigned char c; //c =[0,256) 
    long i, j, m, n, f;
    long min1, pt1, flength;
    FILE *ifp, *ofp;
    ifp = fopen(filename, "rb");                  //打开原始文件
    if (ifp == NULL)
    {
        printf("打开文件失败:%s\n",filename);
        return 0;                             //如果打开失败，则输出错误信息
    }
    ofp = fopen(outputfile,"wb");                 //打开压缩后存储信息的文件
    if (ofp == NULL)
    {
        printf("打开文件失败:%s\n",outputfile);
        return 0;
    }
    flength = 0;
    while (!feof(ifp))
    {
        fread(&c, 1, 1, ifp);
        header[c].count ++;                       //读文件，统计字符出现次数
        flength ++;                               //记录文件的字符总数
    }
    flength --;
    header[c].count --;
    for (i = 0; i < 512; i ++)                    //HUFFMAN算法中初始节点的设置
    {
        if (header[i].count != 0)
            header[i].b = (unsigned char) i;
        else
            header[i].b = -1;
        header[i].parent = -1;
        header[i].lch = header[i].rch = -1;
    }
 
    for (i = 0; i < 256; i ++)                    //将节点按出现次数排序
    {
        for (j = i + 1; j < 256; j ++)
        {
            if (header[i].count < header[j].count)
            {
                tmp = header[i];
                header[i] = header[j];
                header[j] = tmp;
            }
        }
    }
 
 
    for (i = 0; i < 256; i ++)                    //统计不同字符的数量
	{
		if (header[i].count == 0)
        break;
	}
//	printf("i的值是：%d",i); 
 //构造HUFFMAN树
    n = i; //i的值为出现次数最少的字符的位置 
    m = 2 * n - 1;  
    for (i = n; i < m; i ++)
    {
        min1 = 999999999;
        for (j = 0; j < i; j ++)
        {
            if (header[j].parent != -1) continue;
            if (min1 > header[j].count) //找到当前最小的子树 
            {
                pt1 = j;
                min1 = header[j].count;
                continue;
            }
        }
        
        header[i].count = header[pt1].count;
        header[pt1].parent = i;
        header[i].lch = pt1;
        min1 = 999999999;
        for (j = 0; j < i; j ++)
        {
            if (header[j].parent != -1) continue;
            if (min1 > header[j].count)//找到当前第二小的 子树 
            {
                pt1 = j;
                min1 = header[j].count;
                continue;
            }
        }
        header[i].count += header[pt1].count;
        header[i].rch = pt1;
        header[pt1].parent = i;
    }
//    printf("i=%d\n\n",i);
 
    for (i = 0; i < n; i ++)                        //设置字符的编码
    {
        f = i;
        header[i].bits[0] = 0;
        while (header[f].parent != -1)
        {
            j = f; // 保存初始叶子节点位置 
            f = header[f].parent;
            if (header[f].lch == j) //如果j是一个左节点 
            {
                j = strlen(header[i].bits); //当前编码的长度 
                memmove(header[i].bits + 1, header[i].bits, j + 1); //复制 这里的j+1是因为字符数组后面有一个结束符\0 
                header[i].bits[0] = '0'; //左节点路径为0 
            }
            else
            {
                j = strlen(header[i].bits);
                memmove(header[i].bits + 1, header[i].bits, j + 1);
                header[i].bits[0] = '1'; // 右节点路径为1 
            }
        }
    }
 
    //下面的就是读原文件的每一个字符，按照设置好的编码替换文件中的字符
    fseek(ifp, 0, SEEK_SET);                                                //将指针定在文件起始位置
    fseek(ofp, 8, SEEK_SET);                                //以8位二进制数为单位进行读取，从第8个字节的位置开始存数据 （预留前8 个字节用来存储编码信息 
    buf[0] = 0;
    f = 0;
    pt1 = 8;
 
	printf("正在压缩...");
//		int cou=0;
    while (!feof(ifp))
    {
    
    	
        c = fgetc(ifp); 									//读取一个字节并返回 c=(0~256)
        f ++;
        for (i = 0; i < n; i ++)
        {
            if (c == header[i].b) break;					//获得c的huffman节点，下标值 
        }
        strcat(buf, header[i].bits); 
        
        j = strlen(buf);
        c = 0;
        while (j >= 8)                                             //当剩余字符数量不小于8个时，每次凑够8位就存进去  
        {
            for (i = 0; i < 8; i ++)                               //按照八位二进制数转化成十进制ASCII码写入文件一次进行压缩
            {
                if (buf[i] == '1') c = (c << 1) | 1; 				//每次左移一位最低位或运算变成1 
                else c = c << 1;
            }
            fwrite(&c, 1, 1, ofp);
            pt1 ++;
            strcpy(buf, buf + 8);
            j = strlen(buf);
        }
        
        if (f == flength)
        	break;

    }
    if (j > 0)  //j=2                                                     //当剩余字符数量少于8个时
    {
        strcat(buf, "00000000");
        for (i = 0; i < 8; i ++)
        {
            if (buf[i] == '1') c = (c << 1) | 1;
            else c = c << 1;                                        //对不足的位数进行补零
        }
        fwrite(&c, 1, 1, ofp);
        pt1 ++;
           
    }
//    printf("pt1->%d\n",pt1);
    
    fseek(ofp, 0, SEEK_SET);  //定位到开头                                      //将编码信息写入存储文件
	fwrite(&flength,1,sizeof(flength),ofp); //long占4个字节 
    fwrite(&pt1, sizeof(long), 1, ofp);
    fseek(ofp, pt1, SEEK_SET); 					//定位到文件尾部 
    fwrite(&n, sizeof(long), 1, ofp); 			// n=256
    for (i = 0; i < n; i ++)					//将哈夫曼对照表写进去 
    {
		tmp = header[i];
 
        fwrite(&(header[i].b), 1, 1, ofp); 		//字符（1byte）|哈夫曼编码的路径长度（1byte）|哈夫曼编码（可能超过1个字节） 
		pt1++;
        c = strlen(header[i].bits);
        fwrite(&c, 1, 1, ofp);
		pt1++;
        j = strlen(header[i].bits);
 
        if (j % 8 != 0)                         //当位数不满8的倍数时，对该数进行补零操作
        {
            for (f = j % 8; f < 8; f ++)
                strcat(header[i].bits, "0");
        }
 
        while (header[i].bits[0] != 0) // '\0'就是 8位的0 即00000000 
        {
            c = 0;
            for (j = 0; j < 8; j ++)
            {
                if (header[i].bits[j] == '1') c = (c << 1) | 1;
                else c = c << 1;
            }
            strcpy(header[i].bits, header[i].bits + 8);
            fwrite(&c, 1, 1, ofp);                                            //将所得的编码信息写入文件
			pt1++;
        }
 
		header[i] = tmp;
    }
    fclose(ifp);
    fclose(ofp);                                                              //关闭文件
 
 	printf("压缩完毕！压缩后的文件为：%s\n",outputfile);
    return 1;                                       //返回压缩成功信息
}
 
 
//函数：uncompress()
//作用：解压缩文件，并将解压后的内容写入新文件
int uncompress(const char *filename,const char *outputfile)
{
    char buf[255], bx[255];
    unsigned char c;
	char out_filename[512];
    long i, j, m, n, f, p, l;
    long flength;
	int len = 0;
    FILE *ifp, *ofp;
	char c_name[512] = {0};
    ifp = fopen(filename, "rb");                                              //打开文件
    if (ifp == NULL)
    {
        return 0;     //若打开失败，则输出错误信息
    }
	  //读取原文件长
	if(outputfile)
		strcpy(out_filename,outputfile);
	else
		strcpy(out_filename,c_name);
 
    ofp = fopen(out_filename, "wb");                                            //打开文件
    if (ofp == NULL)
    {
        return 0;
    }
 
	fseek(ifp,0,SEEK_END);
	len = ftell(ifp);//获得文件的字符长度 
	fseek(ifp,0,SEEK_SET);//定位到开头 
 
	printf("正在解压...");
 
    fread(&flength, sizeof(long), 1, ifp);                                  //读取原文件长
    fread(&f, sizeof(long), 1, ifp);										//读取压缩后文件的长度 
    fseek(ifp, f, SEEK_SET);												//把指针定位到对照表的前面 
    fread(&n, sizeof(long), 1, ifp);                                         //读取原文件各参数 256 
    for (i = 0; i < n; i ++)                                              //读取压缩文件内容并转换成二进制码
    {
        fread(&header[i].b, 1, 1, ifp);
        fread(&c, 1, 1, ifp);
        p = (long) c; //读取哈夫曼编码的长度 ，之前保存的形式是 1110->11100000 
        header[i].count = p; //哈夫曼编码的长度作为权值 
        header[i].bits[0] = 0;
        if (p % 8 > 0) m = p / 8 + 1; //如果哈夫曼编码有多个字节大小 
        else m = p / 8;
        for (j = 0; j < m; j ++)
        {
            fread(&c, 1 , 1 , ifp);
            f = c; //f=0~255
            /*
			这里会有一个问题：0~255的数字如果原始的哈夫曼编码在这个字节中的第一位是0，那么把哈夫曼编码复原的时候要在前面补0
			如：当前字符的哈夫曼编码原本有10位 1001 1001 01，但是他会凑齐16位再保存，假如保存后是1001 1001 01|000000 （后面6位属于另一个字符的信息 ）
				上述编码当我们读取出来的第二个字节 转化成0~255的数字在转会二进制后会变成 110 1001，所以需要在前面补一个0 ，
				否则当前字符的哈夫曼编码就会变成1001 1001 1000000 
			 
			*/ 
            itoa(f, buf, 2);  
            f = strlen(buf);
            for (l = 8; l > f; l --)
            {
                strcat(header[i].bits, "0");       //补0，防止哈夫曼编码出错                          //位数不足，执行补零操作
                
            }
            strcat(header[i].bits, buf);
        }
		
        header[i].bits[p] = 0;
    }
 
    for (i = 0; i < n; i ++)//升序排序 
    {
        for (j = i + 1; j < n; j ++)
        {
            if (strlen(header[i].bits) > strlen(header[j].bits))
            {
                tmp = header[i];
                header[i] = header[j];
                header[j] = tmp;
            }
        }
    }
 
    p = strlen(header[n-1].bits); //p=9 
    fseek(ifp, 8, SEEK_SET);//定位到数据开始存储的位置 
    m = 0;
    bx[0] = 0; 
 	//根据上面构建的哈夫曼树开始解压文件 
    while (1)
    {
        while (strlen(bx) < (unsigned int)p) //字符串的长度要算上 '\0' 
        {
            fread(&c, 1, 1, ifp);//取一个字节的数据 
            f = c;//将1个字节变成长整型 
            itoa(f, buf, 2);
            f = strlen(buf);
            for (l = 8; l > f; l --)
            {
                strcat(bx, "0"); //不够8位，在补0 还是0开头哈夫曼编码（2进制）->10进制->2进制 可能会丢失哈夫曼编码前面的0 
            }
            strcat(bx, buf); //拼接 
            
        }
        
        for (i = 0; i < n; i ++)
        {
            if (memcmp(header[i].bits, bx, header[i].count) == 0) break; //比较前n个位 （取得下标i 
        }
        strcpy(bx, bx + header[i].count);//一个字字节中可能含有另一个字符的哈夫曼编码，这里将bx中另一个字符的哈夫曼编码移到开头
		 
        c = header[i].b;
        fwrite(&c, 1, 1, ofp);
        m ++;

        if (m == flength) break;
    }
 
    fclose(ifp);
    fclose(ofp);
  	printf("解压完毕！\n");
    return 1;                  
}
 
// int main()
// {
// 	char filename[100];
//  	char outputfile[100];
//  	char compressfile[100]; 
// 	memset(&header,0,sizeof(header));
//     memset(&tmp,0,sizeof(tmp));
//     int flag;
//     int choice=2;
// 	    while(1){
// 	    	memset(filename,0,sizeof(filename));
// 			memset(outputfile,0,sizeof(outputfile));
// 			memset(compressfile,0,sizeof(compressfile));
// 	        menu();
// 	        printf("请输入您的选择：");
// 	        scanf("%d",&choice);
// 	        switch(choice){
// 	            case 1:printf("请输入被压缩的文件的文件名:");
// 	                scanf("%s",filename);
// 	                strcat(compressfile, filename);
// 				   	strcat(compressfile,".huf");
// 				   	compress(filename,compressfile);
// 	                break;
// 	            case 2:printf("请输入压缩文件的文件名（含扩展名）:");
// 	                scanf("%s",compressfile);
// 	                printf("请输入解压后文件的文件名（含扩展名）:");
// 	                scanf("%s",outputfile);
// 					uncompress(compressfile,outputfile);
// 	                break;
// 	            case 3:exit(0);
// 	        }
// 	        system("cls");
// 	    }

// }
int main(){
    compress("1.bmp" , "1.bmp.huf");
}
