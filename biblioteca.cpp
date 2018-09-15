//Desenvolvedores: Bruno Domene e Thiago Polycarpo

//bibliotecas
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

//defines
#define TAM_STRUCT 100

//structs

//struct do arquivo biblioteca.bin
struct livro{
  char isbn[14];
  char titulo[50];
  char autor[50];
  char ano[5];
}arq_livros[TAM_STRUCT];

struct buscaPrimaria{
	char isbn[14];
}busca_p[TAM_STRUCT];

struct buscaSecundaria{
	char autor[50];
}busca_s[TAM_STRUCT];

//struct do arquivo da chave primaria
struct tipo_index{
  char isbn[14];
  int RRN;
}index_isbn[TAM_STRUCT];

//struct do arquivo da chave secundaria
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
int abrir_arquivo(FILE **p_arq, char nome_arq[], char tipo_abertura[]);
int criar_arquivo(char nome_arq[]);
void fechar_arquivo(FILE **p_arq);
int dump_arquivo(FILE **arq);
int carregar_arquivo();
int pegar_registro(FILE **p_arq, char *p_reg);
int inserir(int tam_vet_inserir);

int main(){
  int resp, sair = 0;
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
		    inserir(tam_vet_inserir);
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
int abrir_arquivo(FILE **p_arq, char nome_arq[], char tipo_abertura[]){
  if((*p_arq = fopen(nome_arq, tipo_abertura)) == NULL){
    printf("\nimpossivel abrir o arquivo");
    system("pause");
    return 0;
  }
  return 1;
} 

//cria arquivo livros.bin e adiciona o byte int do contador e o byte offset
int criar_arquivo(char nome_arq[]){
  int cont=0;
  FILE *arq;
  
  if((arq = fopen(nome_arq,"w+b")) == NULL){
    printf("impossivel criar o arquivo");
    system("pause");
    return 0;
  }
  
  //escrevendo o cabeçalho do arquivo passado pelo parametro
  /*if(!strcmp(nome_arq, "arq_livros")){
	  //reserva os 4 primeiros bytes para o contador
	  //fwrite(&cont,sizeof(int),1, arq);//cont do arquivo de insercao
	}*/

  fclose(arq);
  return 1;
}

//carrega arquivo biblioteca.bin e remove.bin
int carregar_arquivo(){
  char arq_cadastro[]="biblioteca.bin", arq_busca_p[]="busca_p.bin", arq_busca_s[]="busca_s.bin", abrir[] = "r+b";
  FILE *arq;
  int i, tam_vet_inserir = 0;
  
  system("cls");
  
  //abre arquivo biblioteca, carrega em vetor de struct 
  if(abrir_arquivo(&arq, arq_cadastro, abrir)){
  	i=0;
    while(fread(&arq_livros[i], sizeof(livro), 1, arq)){
      printf("\n%s | %s | %s | %s", arq_livros[i].isbn, arq_livros[i].titulo, arq_livros[i].autor, arq_livros[i].ano);	
      i++;  
    }
		printf("\ntamanho vetor insercao: %d",i);
    printf("\nDados de insercao carregados!!\n");
    fechar_arquivo(&arq);
  	tam_vet_inserir = i;  
  }
  
  //abre arquivo busca_p.bin, carrega em vetor de struct 
  if(abrir_arquivo(&arq, arq_busca_p, abrir)){
    i=0;
		while(fread(&busca_p[i], sizeof(struct buscaPrimaria), 1, arq)){
		  printf("\n%s ", busca_p[i].isbn);
		  i++;
		}
		printf("\nDados de busca primaria carregados!!\n ");
		fechar_arquivo(&arq);
  }
  
  //abre arquivo busca_s.bin, carrega em vetor de struct 
  if(abrir_arquivo(&arq, arq_busca_s, abrir)){
    i=0;
		while(fread(&busca_s[i], sizeof(struct buscaSecundaria), 1, arq)){
		  printf("\n%s ", busca_s[i].autor);
		  i++;
		}
		printf("\nDados de busca secundaria carregados!!\n\n ");
		fechar_arquivo(&arq);
  }
  
  system("pause");
  return tam_vet_inserir;
}

//faz o dump do arquivo passado por parametro.
int dump_arquivo(FILE **arq){
	char *pch, registro[119], tam_reg, arq_nome[16]="\0", abrir[] = "r+b";
	int aberto, cont_insercao, cont_remocao, offset, resp;
	system("cls");
	do{
		printf("\nDigite qual arquivo deseja carregar: ");
		printf("\n1 - Principal");
		printf("\n2 - Infice Primario");
		printf("\n3 - Indice Secundario");
		printf("\n4 - Lista Invertida: ");
		scanf("%d",&resp);
	}while(resp < 1 || resp > 4);

	if(resp == 1){
		strcpy(arq_nome,"livros.bin");
	}else if (resp == 2){
		strcpy(arq_nome,"busca_p.bin");
	}else if (resp == 3){
		strcpy(arq_nome,"busca_s.bin");
	}else{
		strcpy(arq_nome,"l_invertida.bin");
	}

	!abrir_arquivo(arq, arq_nome, abrir);
	
	fseek(*arq,0,0);
	if(resp == 1){
		fread(&cont_insercao, sizeof(int), 1, *arq);
 		fread(&cont_remocao, sizeof(int), 1, *arq);
 		fread(&offset, sizeof(int), 1, *arq);
 		printf("contador insercao: %d\n", cont_insercao);
 		printf("contador remocao: %d\n", cont_remocao);
		printf("offset: %d\n\n", offset); 	
		
		tam_reg = pegar_registro(arq,registro);
		while (tam_reg > 0){
			pch = strtok(registro,"|");
				while (pch != NULL){
					//exibindo o hexadecimal de cada caractere
					for(int i = 0; i < strlen(pch); i++)
						printf("%X ", pch[i]);
					printf("- %s\n",pch);
					pch = strtok(NULL,"|");
				}
			printf("\n");
		  tam_reg = pegar_registro(arq,registro);
		}
	}

 	
	printf("\n");
	system("pause");
	fclose(*arq);
	return 1;
}

//devolve o tamanho dos registros e obtem o registro
int pegar_registro(FILE **p_arq, char *p_reg){
  int bytes;
     
  if (!(fread(&bytes, sizeof(int), 1, *p_arq)))
    return 0;
  fread(p_reg, bytes, 1, *p_arq);
  return bytes;
}

int inserir(int qtd_livros){
	FILE *arq; 
	int cont_registro, tam_registro ;
	char registro[119], nome_arq[]="livros.bin", leitura[]= "r+b", atualizar[] = "ab";
	
	system("cls");
	
  //testa se o arquivo existe senão, cria o arquivo
	if((fopen(nome_arq, "r+b")) == NULL){
		criar_arquivo(nome_arq);
    printf("\narquivo criado\n");
	}
	
	abrir_arquivo(&arq, nome_arq, leitura); 			//abrindo arquivo principal
	//le o contador de inserção no arquivo
	
	fseek(arq,0,0);
  fread(&cont_registro,1,sizeof(int), arq);
  printf("contador: %d\n", cont_registro);
  fclose(arq);
  
  
	arq = fopen("livros.bin", atualizar);
  //formatando os registros para a estrategia de tamanho variavel a partir do i que voltou do arquivo
  sprintf(registro,"%s|%s|%s|%s|", arq_livros[cont_registro].isbn, arq_livros[cont_registro].titulo, 
														arq_livros[cont_registro].autor, arq_livros[cont_registro].ano);
  tam_registro = strlen(registro); //pega o tamanho de cada registro
	tam_registro++;
	printf("registro na funcao inserir:  %s\n", registro);
	printf("tamanho registro na funcao inserir: %d\n\n", tam_registro);
	
	//inserindo no arquivo principal
	fwrite(&tam_registro, sizeof(int), 1, arq);
	fwrite(registro,sizeof(char),tam_registro, arq);
	fclose(arq);
	
	
	arq = fopen("livros.bin", leitura);
	fseek(arq, 0, 0);														//aponta para o contador
	cont_registro++;					
  fwrite(&cont_registro,sizeof(int),1,arq);//reescreve o contador de inserção
	fclose(arq);														//fecha arquivo principal

  printf("\n\n");
  system("pause");
} 

