#include <stdio.h>

int main()
{
    int sizeX[] = {5, 7, 8, 9};
    int sizeY[] = {5, 7, 8, 9};

    FILE *fp = fopen("levelManager.bin", "w");
    for(int i = 0; i < sizeof(sizeX)/sizeof(int); i++)
    {
        fwrite(&sizeX[i], sizeof(int), 1, fp);
        fwrite(&sizeY[i], sizeof(int), 1, fp);
    }
    fclose(fp);
}