//Desenvolvedores: Bruno Domene e Thiago Polycarpo

//bibliotecas
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

//defines
#define TAM_STRUCT 1000

//structs

//struct do arquivo biblioteca.bin
struct livro{
  char isbn[14];
  char titulo[50];
  char autor[50];
  char ano[5];
}index_livro[TAM_STRUCT];

//struct do arquivo busca_p.bin da chave primaria
struct tipo_index{
  char isbn[14];
  int RRN;
}index_isbn[TAM_STRUCT];

//struct do arquivo busca_s.bin da chave secundaria
struct tipo_index_autor{
  char autor[50];
  int  p_list;
}index_autor[TAM_STRUCT];

//struct da lista invertida
struct lista_l_invertida{
  char isbn[14];
  int  prox;
}l_invertida[TAM_STRUCT];


//funcoes
int abrir_arquivo(FILE **p_arq, char nome_arq[]);
//void criar_arquivo(FILE *p_arq, char nome_arq[]);
void fechar_arquivo(FILE **p_arq);
int carregar_arquivo();
/*int pegar_registro(FILE **p_arq, char *p_reg);
int inserir(FILE **arq, char nome_arq[], int tam_vet_inserir);
int dump_arquivo(FILE **arq);
int remover(FILE **arq);
int compactar(FILE **arq);
int deletar_arquivo(char arq[]);
*/
int main(){
  int resp, sair = 0;
	FILE *arq;
	char arq_livros[]="livros.bin";
	int tam_vet_inserir;
	do{ 
	  system("cls");
	    
	  printf("\n        Menu");
	  printf("\n1 - Insercao");
	  printf("\n2 - Remocao");
	  printf("\n3 - Compactacao");
	  printf("\n4 - Carregar Arquivos");
	  printf("\n5 - Dump Arquivo ");
	  printf("\n6 - Sair");
	  printf("\nopcao:");
	  scanf("%d",&resp);
	
		switch(resp){
			case 1:{
		    //inserir(&arq, arq_livros, tam_vet_inserir);
				break;
			}
		  	case 2:{
		  	//remover(&arq);
				break;
			}
		  	case 3: {
		  	//compactar(&arq);
		  		break;
		  	}
		  	case 4:{
		    tam_vet_inserir = carregar_arquivo(); 
				break;
			}
		  	case 5:{
		    //dump_arquivo(&arq);
				break;
			}
		  	case 6:{
		    sair = 1;
				break;
			}
		  	default:{
		    printf("\nOpcao invalida!");
				break;
			} 
		}
	}while(sair != 1);
}

//fecha arquivos
void fechar_arquivo(FILE **p_arq){
  fclose(*p_arq);
}

//abre arquivos
int abrir_arquivo(FILE **p_arq, char nome_arq[]){
  if((*p_arq = fopen(nome_arq,"r+b")) == NULL){
    printf("\nimpossivel abrir o arquivo");
    return 0;
  }
  return 1;
} 

//cria arquivo livros.bin e adiciona o byte int do contador e o byte offset
void criar_arquivo(FILE *p_arq, char nome_arq[]){
  int cont=0;
  if((p_arq = fopen(nome_arq,"w+b")) == NULL){
    printf("impossivel criar o arquivo");
    //system("pause");
    exit(0);
  }
  //reserva os 4 primeiros bytes para o contador e os outros 4 para o offset
  fwrite(&cont,sizeof(int),1,p_arq);//cont do arquivo de insercao
  fclose(p_arq);
}

//carrega arquivo biblioteca.bin e remove.bin
int carregar_arquivo(){
  char arq_cadastro[]="biblioteca.bin", arq_busca_p[]="busca_p.bin", arq_busca_s[]="busca_s.bin", arq_l_invertida[]="l_invertida.bin";
  FILE *arq;
  int i, aberto,resp,tam_vet_inserir;
  
  //abre arquivo biblioteca, carrega em vetor de struct 
  aberto = abrir_arquivo(&arq, arq_cadastro);
  if(aberto){
  	i=0;
    while(fread(&index_livro[i], sizeof(livro), 1, arq)){
      printf("\n%s | %s | %s | %s", index_livro[i].isbn, index_livro[i].titulo, index_livro[i].autor, index_livro[i].ano);	
      i++;
      printf("\ntamanho vetor insercao--> %d",i);
    }
    printf("\nDados de insercao carregados!!\n");
    fechar_arquivo(&arq);
  }
  tam_vet_inserir = i;  
  //abre arquivo busca_p.bin, carrega em vetor de struct 
 aberto = abrir_arquivo(&arq, arq_busca_p);
  if(aberto){
    i=0;
	while(fread(&index_isbn[i], sizeof(struct tipo_index), 1, arq)){
	  printf("\n%s ", index_isbn[i].isbn);
	  printf("\n%d ", index_isbn[i].RRN);
	  i++;
	}
	printf("\nDados de busca primaria carregados!!\n ");
	fechar_arquivo(&arq);
  }
  //abre arquivo busca_s.bin, carrega em vetor de struct 
  aberto = abrir_arquivo(&arq, arq_busca_s);
  if(aberto){
    i=0;
	while(fread(&index_autor[i], sizeof(struct tipo_index_autor), 1, arq)){
	  printf("\n%s ", index_autor[i].autor);
	  printf("\n%d ", index_autor[i].p_list);
	  i++;
	}
	printf("\nDados de busca secundaria carregados!!\n ");
	fechar_arquivo(&arq);
  }
  //abre arquivo arq_l_invertida.bin, carrega em vetor de struct 
	aberto = abrir_arquivo(&arq, arq_l_invertida);
  if(aberto){
    i=0;
	while(fread(&l_invertida[i], sizeof(struct lista_l_invertida), 1, arq)){
	  printf("\n%s ", l_invertida[i].isbn);
	  printf("\n%d ", l_invertida[i].prox);
	  i++;
	}
	printf("\nDados de lista invertida carregados!!\n ");
	fechar_arquivo(&arq);
  }
  system("pause");
  return tam_vet_inserir;
}

