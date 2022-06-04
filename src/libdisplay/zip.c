#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#include "zip.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

 
// void menu(){
//     printf("%24c*********************\n",' ');
//     printf("%24c**BMPѹ��/��ѹ����**\n",' ');
//     printf("%24c*********************\n",' ');
//     printf("%17c*************************************\n",' ');
//     printf("%17c***       1.ѹ��(compress)        ***\n",' ');
//     printf("%17c***       2.��ѹ��(uncompress)    ***\n",' ');
//     printf("%17c***       3.�˳�(quit)            ***\n",' ');
//     printf("%17c*************************************\n",' ');
// }

static struct head header[512], tmp;  //�ڵ���
 
//������compress()
//���ã���ȡ�ļ����ݲ�����ѹ��
int compress(const char *filename,const char *outputfile)
{
    char buf[512];
    unsigned char c; //c =[0,256) 
    long i, j, m, n, f;
    long min1, pt1, flength;
    FILE *ifp, *ofp;
    ifp = fopen(filename, "rb");                  //��ԭʼ�ļ�
    if (ifp == NULL)
    {
        printf("���ļ�ʧ��:%s\n",filename);
        return 0;                             //�����ʧ�ܣ������������Ϣ
    }
    ofp = fopen(outputfile,"wb");                 //��ѹ����洢��Ϣ���ļ�
    if (ofp == NULL)
    {
        printf("���ļ�ʧ��:%s\n",outputfile);
        return 0;
    }
    flength = 0;
    while (!feof(ifp))
    {
        fread(&c, 1, 1, ifp);
        header[c].count ++;                       //���ļ���ͳ���ַ����ִ���
        flength ++;                               //��¼�ļ����ַ�����
    }
    flength --;
    header[c].count --;
    for (i = 0; i < 512; i ++)                    //HUFFMAN�㷨�г�ʼ�ڵ������
    {
        if (header[i].count != 0)
            header[i].b = (unsigned char) i;
        else
            header[i].b = -1;
        header[i].parent = -1;
        header[i].lch = header[i].rch = -1;
    }
 
    for (i = 0; i < 256; i ++)                    //���ڵ㰴���ִ�������
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
 
 
    for (i = 0; i < 256; i ++)                    //ͳ�Ʋ�ͬ�ַ�������
	{
		if (header[i].count == 0)
        break;
	}
//	printf("i��ֵ�ǣ�%d",i); 
 //����HUFFMAN��
    n = i; //i��ֵΪ���ִ������ٵ��ַ���λ�� 
    m = 2 * n - 1;  
    for (i = n; i < m; i ++)
    {
        min1 = 999999999;
        for (j = 0; j < i; j ++)
        {
            if (header[j].parent != -1) continue;
            if (min1 > header[j].count) //�ҵ���ǰ��С������ 
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
            if (min1 > header[j].count)//�ҵ���ǰ�ڶ�С�� ���� 
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
 
    for (i = 0; i < n; i ++)                        //�����ַ��ı���
    {
        f = i;
        header[i].bits[0] = 0;
        while (header[f].parent != -1)
        {
            j = f; // �����ʼҶ�ӽڵ�λ�� 
            f = header[f].parent;
            if (header[f].lch == j) //���j��һ����ڵ� 
            {
                j = strlen(header[i].bits); //��ǰ����ĳ��� 
                memmove(header[i].bits + 1, header[i].bits, j + 1); //���� �����j+1����Ϊ�ַ����������һ��������\0 
                header[i].bits[0] = '0'; //��ڵ�·��Ϊ0 
            }
            else
            {
                j = strlen(header[i].bits);
                memmove(header[i].bits + 1, header[i].bits, j + 1);
                header[i].bits[0] = '1'; // �ҽڵ�·��Ϊ1 
            }
        }
    }
 
    //����ľ��Ƕ�ԭ�ļ���ÿһ���ַ����������úõı����滻�ļ��е��ַ�
    fseek(ifp, 0, SEEK_SET);                                                //��ָ�붨���ļ���ʼλ��
    fseek(ofp, 8, SEEK_SET);                                //��8λ��������Ϊ��λ���ж�ȡ���ӵ�8���ֽڵ�λ�ÿ�ʼ������ ��Ԥ��ǰ8 ���ֽ������洢������Ϣ 
    buf[0] = 0;
    f = 0;
    pt1 = 8;
 
	printf("����ѹ��...");
//		int cou=0;
    while (!feof(ifp))
    {
    
    	
        c = fgetc(ifp); 									//��ȡһ���ֽڲ����� c=(0~256)
        f ++;
        for (i = 0; i < n; i ++)
        {
            if (c == header[i].b) break;					//���c��huffman�ڵ㣬�±�ֵ 
        }
        strcat(buf, header[i].bits); 
        
        j = strlen(buf);
        c = 0;
        while (j >= 8)                                             //��ʣ���ַ�������С��8��ʱ��ÿ�δչ�8λ�ʹ��ȥ  
        {
            for (i = 0; i < 8; i ++)                               //���հ�λ��������ת����ʮ����ASCII��д���ļ�һ�ν���ѹ��
            {
                if (buf[i] == '1') c = (c << 1) | 1; 				//ÿ������һλ���λ��������1 
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
    if (j > 0)  //j=2                                                     //��ʣ���ַ���������8��ʱ
    {
        strcat(buf, "00000000");
        for (i = 0; i < 8; i ++)
        {
            if (buf[i] == '1') c = (c << 1) | 1;
            else c = c << 1;                                        //�Բ����λ�����в���
        }
        fwrite(&c, 1, 1, ofp);
        pt1 ++;
           
    }
//    printf("pt1->%d\n",pt1);
    
    fseek(ofp, 0, SEEK_SET);  //��λ����ͷ                                      //��������Ϣд��洢�ļ�
	fwrite(&flength,1,sizeof(flength),ofp); //longռ4���ֽ� 
    fwrite(&pt1, sizeof(long), 1, ofp);
    fseek(ofp, pt1, SEEK_SET); 					//��λ���ļ�β�� 
    fwrite(&n, sizeof(long), 1, ofp); 			// n=256
    for (i = 0; i < n; i ++)					//�����������ձ�д��ȥ 
    {
		tmp = header[i];
 
        fwrite(&(header[i].b), 1, 1, ofp); 		//�ַ���1byte��|�����������·�����ȣ�1byte��|���������루���ܳ���1���ֽڣ� 
		pt1++;
        c = strlen(header[i].bits);
        fwrite(&c, 1, 1, ofp);
		pt1++;
        j = strlen(header[i].bits);
 
        if (j % 8 != 0)                         //��λ������8�ı���ʱ���Ը������в������
        {
            for (f = j % 8; f < 8; f ++)
                strcat(header[i].bits, "0");
        }
 
        while (header[i].bits[0] != 0) // '\0'���� 8λ��0 ��00000000 
        {
            c = 0;
            for (j = 0; j < 8; j ++)
            {
                if (header[i].bits[j] == '1') c = (c << 1) | 1;
                else c = c << 1;
            }
            strcpy(header[i].bits, header[i].bits + 8);
            fwrite(&c, 1, 1, ofp);                                            //�����õı�����Ϣд���ļ�
			pt1++;
        }
 
		header[i] = tmp;
    }
    fclose(ifp);
    fclose(ofp);                                                              //�ر��ļ�
 
 	printf("ѹ����ϣ�ѹ������ļ�Ϊ��%s\n",outputfile);
    return 1;                                       //����ѹ���ɹ���Ϣ
}
 
 
//������uncompress()
//���ã���ѹ���ļ���������ѹ�������д�����ļ�
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
    ifp = fopen(filename, "rb");                                              //���ļ�
    if (ifp == NULL)
    {
        return 0;     //����ʧ�ܣ������������Ϣ
    }
	  //��ȡԭ�ļ���
	if(outputfile)
		strcpy(out_filename,outputfile);
	else
		strcpy(out_filename,c_name);
 
    ofp = fopen(out_filename, "wb");                                            //���ļ�
    if (ofp == NULL)
    {
        return 0;
    }
 
	fseek(ifp,0,SEEK_END);
	len = ftell(ifp);//����ļ����ַ����� 
	fseek(ifp,0,SEEK_SET);//��λ����ͷ 
 
	printf("���ڽ�ѹ...");
 
    fread(&flength, sizeof(long), 1, ifp);                                  //��ȡԭ�ļ���
    fread(&f, sizeof(long), 1, ifp);										//��ȡѹ�����ļ��ĳ��� 
    fseek(ifp, f, SEEK_SET);												//��ָ�붨λ�����ձ��ǰ�� 
    fread(&n, sizeof(long), 1, ifp);                                         //��ȡԭ�ļ������� 256 
    for (i = 0; i < n; i ++)                                              //��ȡѹ���ļ����ݲ�ת���ɶ�������
    {
        fread(&header[i].b, 1, 1, ifp);
        fread(&c, 1, 1, ifp);
        p = (long) c; //��ȡ����������ĳ��� ��֮ǰ�������ʽ�� 1110->11100000 
        header[i].count = p; //����������ĳ�����ΪȨֵ 
        header[i].bits[0] = 0;
        if (p % 8 > 0) m = p / 8 + 1; //��������������ж���ֽڴ�С 
        else m = p / 8;
        for (j = 0; j < m; j ++)
        {
            fread(&c, 1 , 1 , ifp);
            f = c; //f=0~255
            /*
			�������һ�����⣺0~255���������ԭʼ�Ĺ���������������ֽ��еĵ�һλ��0����ô�ѹ��������븴ԭ��ʱ��Ҫ��ǰ�油0
			�磺��ǰ�ַ��Ĺ���������ԭ����10λ 1001 1001 01�������������16λ�ٱ��棬���籣�����1001 1001 01|000000 ������6λ������һ���ַ�����Ϣ ��
				�������뵱���Ƕ�ȡ�����ĵڶ����ֽ� ת����0~255��������ת������ƺ���� 110 1001��������Ҫ��ǰ�油һ��0 ��
				����ǰ�ַ��Ĺ���������ͻ���1001 1001 1000000 
			 
			*/ 
            itoa(f, buf, 2);  
            f = strlen(buf);
            for (l = 8; l > f; l --)
            {
                strcat(header[i].bits, "0");       //��0����ֹ�������������                          //λ�����㣬ִ�в������
                
            }
            strcat(header[i].bits, buf);
        }
		
        header[i].bits[p] = 0;
    }
 
    for (i = 0; i < n; i ++)//�������� 
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
    fseek(ifp, 8, SEEK_SET);//��λ�����ݿ�ʼ�洢��λ�� 
    m = 0;
    bx[0] = 0; 
 	//�������湹���Ĺ���������ʼ��ѹ�ļ� 
    while (1)
    {
        while (strlen(bx) < (unsigned int)p) //�ַ����ĳ���Ҫ���� '\0' 
        {
            fread(&c, 1, 1, ifp);//ȡһ���ֽڵ����� 
            f = c;//��1���ֽڱ�ɳ����� 
            itoa(f, buf, 2);
            f = strlen(buf);
            for (l = 8; l > f; l --)
            {
                strcat(bx, "0"); //����8λ���ڲ�0 ����0��ͷ���������루2���ƣ�->10����->2���� ���ܻᶪʧ����������ǰ���0 
            }
            strcat(bx, buf); //ƴ�� 
            
        }
        
        for (i = 0; i < n; i ++)
        {
            if (memcmp(header[i].bits, bx, header[i].count) == 0) break; //�Ƚ�ǰn��λ ��ȡ���±�i 
        }
        strcpy(bx, bx + header[i].count);//һ�����ֽ��п��ܺ�����һ���ַ��Ĺ��������룬���ｫbx����һ���ַ��Ĺ����������Ƶ���ͷ
		 
        c = header[i].b;
        fwrite(&c, 1, 1, ofp);
        m ++;

        if (m == flength) break;
    }
 
    fclose(ifp);
    fclose(ofp);
  	printf("��ѹ��ϣ�\n");
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
// 	        printf("����������ѡ��");
// 	        scanf("%d",&choice);
// 	        switch(choice){
// 	            case 1:printf("�����뱻ѹ�����ļ����ļ���:");
// 	                scanf("%s",filename);
// 	                strcat(compressfile, filename);
// 				   	strcat(compressfile,".huf");
// 				   	compress(filename,compressfile);
// 	                break;
// 	            case 2:printf("������ѹ���ļ����ļ���������չ����:");
// 	                scanf("%s",compressfile);
// 	                printf("�������ѹ���ļ����ļ���������չ����:");
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
