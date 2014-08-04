#include <stdio.h>
   
int main(void){
    FILE **fp=NULL;
    int i,height,width;
    char path[30],name[30];
    printf("height=");
    scanf_s("%i",&height);
    printf("width=");
    scanf_s("%i",&width);
    printf("Image path=");
    scanf_s("%s",path);
    printf("name=");
    scanf_s("%s",name);
    
       fopen_s(fp, "sprite_sheet2.xml", "w");
      
      fprintf(*fp,"<TextureAtlas imagePath=\"%s\">\n",path);
      for(i=0;i<height-3744;i++){
          if(i<10){
              fprintf(*fp,"<SubTexture name=\"%s000%i.png\" x=\"0\" y=\"%i\" width=\"3744\" height=\"3744\"/>\n",name,i,i);
          }else if(i<100){
              fprintf(*fp,"<SubTexture name=\"%s00%i.png\" x=\"0\" y=\"%i\" width=\"3744\" height=\"3744\"/>\n",name,i,i);  
          }else if(i<1000){
              fprintf(*fp,"<SubTexture name=\"%s0%i.png\" x=\"0\" y=\"%i\" width=\"3744\" height=\"3744\"/>\n",name,i,i);  
          }else if(i<10000){
              fprintf(*fp,"<SubTexture name=\"%s%i.png\" x=\"0\" y=\"%i\" width=\"3744\" height=\"3744\"/>\n",name,i,i);      
          }
      }
      fprintf(*fp,"</TextureAtlas>");
      
      fclose(*fp);
      return 0;
}
