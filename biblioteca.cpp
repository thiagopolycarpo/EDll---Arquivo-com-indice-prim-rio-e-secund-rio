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
typedef struct tipo_index{
  char isbn[14];
  int posicao;
}index;
index est_isbn[TAM_STRUCT];

//struct do arquivo da chave secundaria
typedef struct tipo_index_autor{
  char autor[50];
  int  p_list;
}index_autor;
index_autor est_autor[TAM_STRUCT];

//struct da lista invertida
typedef struct lista_l_invertida{
  char isbn[14];
  int  prox;
}invertida;

invertida l_invertida[TAM_STRUCT];

char atualizar[] = "ab", leitura[] = "r+b", escrever[] = "wb";
int qtd_chave_s = 0;



//funcoes
int abrir_arquivo(FILE **p_arq, char nome_arq[], char tipo_abertura[]);
int criar_arquivo(char nome_arq[]);
void fechar_arquivo(FILE **p_arq);
int dump_arquivo(FILE **arq);
int carregar_arquivo();
int pegar_registro(FILE **p_arq, char *p_reg);
int inserir(int tam_vet_inserir);
void InserindoNoArquivoDeChavePrimaria(int contador_insercao, int posicao_registro);
void inserirNoArquivoChaveSecundaria(int contador_insercao, char autor[]);
int inserir_lista_invertida(int contador_insercao, char isbn[], int posicao_do_anterior);
void mergeSortParaArquivoDeChavePrimaria(index *est, int posicaoInicio, int posicaoFim);
void mergeSortParaArquivoDeChaveSecundaria(index_autor *est, int posicaoInicio, int posicaoFim);


int main(){
  int resp, sair = 0;
	char arq_livros[]="livros.bin";
	int tam_vet_inserir = 0;
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
  int cont=0, atualizado = 1;
  FILE *arq;
  
  if((arq = fopen(nome_arq,"w+b")) == NULL){
    printf("impossivel criar o arquivo");
    system("pause");
    return 0;
  }
  
  //escrevendo o cabeçalho do arquivo passado pelo parametro
  if(!strcmp(nome_arq, "livros.bin")){
	  //reserva os 4 primeiros bytes para o contador
	  fwrite(&cont,sizeof(int),1, arq);//cont do arquivo de insercao
	}
	
	//escrevendo flag de atualização no arquivo de chave primaria
	if(!strcmp(nome_arq, "arq_chave_p.bin")){
	  //reserva os 4 primeiros bytes para o contador
	  fwrite(&atualizado,sizeof(int),1, arq);//cont do arquivo de insercao
	}
	
	//escrevendo flag de atualização no arquivo de chave secundaria
	if(!strcmp(nome_arq, "arq_chave_s.bin")){
	  //reserva os 4 primeiros bytes para o contador
	  fwrite(&atualizado,sizeof(int),1, arq);//cont do arquivo de insercao
	}

  fclose(arq);
  return 1;
}

//carrega arquivo biblioteca.bin e remove.bin
int carregar_arquivo(){
  char arq_cadastro[]="biblioteca.bin", arq_busca_p[]="busca_p.bin", arq_busca_s[]="busca_s.bin", abrir[] = "r+b";
  char arq_primaria[]= "arq_chave_p.bin", lista_invert[]="lista_invertida.bin";
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
  
  //carrega os dados do arquivo de chave primaria(isbn) no vetor de struct 
  if((arq = fopen(arq_primaria, "r+b")) != NULL){
  	int i = 0; 
		fseek(arq, 4, 0);
		//pegando os registros do arquivo de chave primaria
		while(fread(&est_isbn[i], sizeof(struct tipo_index) , 1, arq)){
			printf("isbn: %s\n", est_isbn[i].isbn);
			printf("posicao: %d\n", est_isbn[i].posicao);
			i++;
		}
		printf("Registros de chave primaria carrgados\n\n");
		fclose(arq);
	}
	
	 //carrega os dados do arquivo de chave secundaria(autor) no vetor de struct 
	if((arq = fopen("arq_chave_s.bin", "r+b")) != NULL){
  	int i = 0; 
		fseek(arq, 4, 0);
		//pegando os registros do arquivo de chave primaria
		while(fread(&est_autor[i], sizeof(index_autor) , 1, arq)){
			printf("autor: %s\n", est_autor[i].autor);
			printf("posicao da lista: %d\n", est_autor[i].p_list);
			i++;
		}
		printf("Registros de chave secundaria carrgados\n\n");
		fclose(arq);
		qtd_chave_s = i;
	}
	
	if((arq = fopen("lista_invertida.bin", "r+b")) != NULL){
  	int i = 0; 
		//pegando os registros do arquivo de chave primaria
		while(fread(&l_invertida[i], sizeof(invertida) , 1, arq)){
			printf("autor: %s\n", l_invertida[i].isbn);
			printf("proximo: %d\n",l_invertida[i].prox);
			i++;
		}
		printf("Registros da lista invertida carregados carrgados\n\n");
		fclose(arq);
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

	abrir_arquivo(arq, arq_nome, abrir);
	
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

int inserir(int qtd_livros_carregados){
	FILE *arq; 
	int cont_registro, tam_registro, posicao_registro;
	char registro[119], nome_arq[]="livros.bin", leitura[]= "r+b", atualizar[] = "a+b";
	
	system("cls");
	
  //testa se o arquivo existe senão, cria o arquivo
	if((fopen(nome_arq, "r+b")) == NULL){
		criar_arquivo(nome_arq);
    printf("\narquivo criado\n");
	}
	
	abrir_arquivo(&arq, nome_arq, leitura); 			//abrindo arquivo principal no modo de leitura
	//le o contador de inserção no arquivo
	fseek(arq,0,0);
  fread(&cont_registro,1,sizeof(int), arq);
  printf("contador: %d\n", cont_registro);
  fclose(arq);	
  
  if(cont_registro < qtd_livros_carregados){
		arq = fopen("livros.bin", atualizar);					//abrindo arquivo para atualizar
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
		posicao_registro = ftell(arq) - (tam_registro + 4);				//pegando a posição do registro no arquivo
		fclose(arq);
	
		InserindoNoArquivoDeChavePrimaria(cont_registro, posicao_registro);
		inserirNoArquivoChaveSecundaria(cont_registro, arq_livros[cont_registro].autor);
		
		//incrementando contador de inserção
		arq = fopen("livros.bin", leitura);
		fseek(arq, 0, 0);														//aponta para o contador
		cont_registro++;					
	  fwrite(&cont_registro,sizeof(int),1,arq);//reescreve o contador de inserção
		fclose(arq);														//fecha arquivo principal
	}else{
		printf("Nao ha mais livros a serem inseridos\n");
	}
	
  printf("\n\n");
  system("pause");
} 

//Inserindo no arquivo de chave primaria
void InserindoNoArquivoDeChavePrimaria(int contador_insercao, int posicao_registro){
	char nome_arq[] = "arq_chave_p.bin", atualizar[] = "ab", leitura[] = "r+b", escrever[] = "wb"; 
	FILE *arq;
	int i, atualizado;
	
	if((fopen(nome_arq, "r+b")) == NULL){
		criar_arquivo(nome_arq);
	}
	
	//abrindo para leitura do flag de atualização
	abrir_arquivo(&arq, nome_arq, leitura);
	fread(&atualizado, sizeof(int), 1, arq);
	
	
	//copiando o registro para a estrutura da chave primaria
	est_isbn[contador_insercao].posicao = posicao_registro;
	strcpy(est_isbn[contador_insercao].isbn, arq_livros[contador_insercao].isbn);
	//alterando flag de atualização pois copia do arquivo em memoria foi modificada
	atualizado = 0;
	fseek(arq,0,0);
	fwrite(&atualizado, sizeof(int), 1, arq);
	fclose(arq);
	
	//ordenando registros
	mergeSortParaArquivoDeChavePrimaria(est_isbn, 0, contador_insercao);

	//reescrevendo arquivo
	abrir_arquivo(&arq, nome_arq, escrever);
	//Colando flag de atualização em 1, pois arquivo foi reescrito
	atualizado = 1;
	fwrite(&atualizado, sizeof(int), 1, arq);	
	for(i = 0; i <= contador_insercao; i++)
		fwrite(&est_isbn[i], sizeof(struct tipo_index), 1, arq);
	fclose(arq);
}

void inserirNoArquivoChaveSecundaria(int contador_insercao, char autor[]){
	char nome_arq[] = "arq_chave_s.bin", escrever[]= "wb";
	FILE *arq;
	int i = 0, atualizado, achou = 0;
		
	if((fopen(nome_arq, "r+b")) == NULL){
		criar_arquivo(nome_arq);
	}
	
	//abrindo para leitura do flag de atualização
	abrir_arquivo(&arq, nome_arq, leitura);
	fread(&atualizado, sizeof(int), 1, arq);
	fclose(arq);
	
	//procurando se já possui um autor igual ao passado por paremetro 
	while(i < contador_insercao && achou != 1){
		
		if(!(strcmp(est_autor[i].autor, autor))) {
			achou = 1;
			printf("\nACHOU\n");
		}
		i++;
	}
	
	if(achou){
		int posicao_anterior;
		i--;
		posicao_anterior = est_autor[i].p_list;
		est_autor[i].p_list = inserir_lista_invertida(contador_insercao, arq_livros[contador_insercao].isbn, posicao_anterior);
				
	}else{ //se não achou nome igual insere normal
		//copiando o registro para o vetor de struct da chave secundaria
		est_autor[qtd_chave_s].p_list = inserir_lista_invertida(contador_insercao, arq_livros[contador_insercao].isbn, -1); 
		strcpy(est_autor[qtd_chave_s].autor, autor);
		//alterando flag de atualização, pois copia do arquivo em memoria foi modificada
		abrir_arquivo(&arq, nome_arq, leitura);
		atualizado = 0;
		fseek(arq,0,0);
		fwrite(&atualizado, sizeof(int), 1, arq);
		fclose(arq);
		qtd_chave_s++;
	}
	
	//ordenando
	mergeSortParaArquivoDeChaveSecundaria(est_autor, 0, qtd_chave_s - 1);
	
	//reescrevendo arquivo de chave secundaria
	abrir_arquivo(&arq, nome_arq, escrever);
	//Colando flag de atualização em 1, pois arquivo foi reescrito
	atualizado = 1;
	fwrite(&atualizado, sizeof(int), 1, arq);
	for(i = 0; i < qtd_chave_s; i++){		
		fwrite(&est_autor[i], sizeof(index_autor), 1, arq);
		printf("autor: %s\n", est_autor[i].autor);
		printf("posicao: %d\n", est_autor[i].p_list);
	}
	fechar_arquivo(&arq);	
}

/* escreve registro na lista invertida e devolve posição dele no arquivo, se o parametro "posicao_do_anterior" for maior ou 
igual a zero sifnifica q o nome passado já exixte no arquivo de chave secundaria assim, o atributo proximo receba a 
posicao do registro  de mesmo nome anterior a ele (está incrementando na lista encadeado) */
int inserir_lista_invertida(int contador_insercao, char isbn[], int posicao_do_anterior){
	char nome_arq[] = "lista_invertida.bin";
	FILE *arq;
	invertida est;
	int posicao;
		
	if((fopen(nome_arq, "r+b")) == NULL){
		criar_arquivo(nome_arq);
	}
	
	//copiando dados para o vetor de estrutura da lista invertida
	strcpy(l_invertida[contador_insercao].isbn, isbn); 
	
	if(posicao_do_anterior < 0)
		l_invertida[contador_insercao].prox = -1;
	else{
		l_invertida[contador_insercao].prox = posicao_do_anterior;
	}
	
	//escrevendo na lista invertida
	abrir_arquivo(&arq, nome_arq, atualizar);
	fwrite(&l_invertida[contador_insercao], sizeof(invertida), 1, arq);
	posicao = ftell(arq) - sizeof(invertida);					//pegando posicao do registro no arquivo
	fclose(arq);
	
	return posicao;
}

//ordenar arquivo de chave primaria(isbn)
void mergeSortParaArquivoDeChavePrimaria(index *est, int posicaoInicio, int posicaoFim){
    int i, j, k, metadeTamanho;
    if(posicaoInicio == posicaoFim) return;
    metadeTamanho = (posicaoInicio + posicaoFim ) / 2;
    index *est_temp;
    
    mergeSortParaArquivoDeChavePrimaria(est, posicaoInicio, metadeTamanho);
    mergeSortParaArquivoDeChavePrimaria(est, metadeTamanho + 1, posicaoFim);

    i = posicaoInicio;
    j = metadeTamanho + 1;
    k = 0;
    est_temp = (index *) malloc(sizeof(index) * (posicaoFim - posicaoInicio + 1));

    while(i < metadeTamanho + 1 || j  < posicaoFim + 1) {
        if (i == metadeTamanho + 1 ) { 
            strcpy(est_temp[k].isbn, est[j].isbn);
            est_temp[k].posicao = est[j].posicao;
            j++;
            k++;
        }
        else {
            if (j == posicaoFim + 1) {
                //vetorTemp[k] = vetor[i];
                strcpy(est_temp[k].isbn, est[i].isbn);
            		est_temp[k].posicao = est[i].posicao;
                i++;
                k++;
            }
            else {
                if (/*vetor[i] < vetor[j]*/  (strcmp(est[i].isbn, est[j].isbn)) < 0) {
                    //vetorTemp[k] = vetor[i];
                    strcpy(est_temp[k].isbn, est[i].isbn);
            				est_temp[k].posicao = est[i].posicao;
                    i++;
                    k++;
                }
                else {
                    //vetorTemp[k] = vetor[j];
                    strcpy(est_temp[k].isbn, est[j].isbn);
            				est_temp[k].posicao = est[j].posicao;
                    j++;
                    k++;
                }
            }
        }

    }
    for(i = posicaoInicio; i <= posicaoFim; i++) {
        //vetor[i] = vetorTemp[i - posicaoInicio];
        strcpy(est[i].isbn, est_temp[i - posicaoInicio].isbn);
      	est[i].posicao = est_temp[i - posicaoInicio].posicao;
    }
    free(est_temp);
}

//ordenar arquivo de chave secundaria(autor)
void mergeSortParaArquivoDeChaveSecundaria(index_autor *est, int posicaoInicio, int posicaoFim){
    int i, j, k, metadeTamanho;
    if(posicaoInicio == posicaoFim) return;
    metadeTamanho = (posicaoInicio + posicaoFim ) / 2;
    index_autor *est_temp;
    
    mergeSortParaArquivoDeChaveSecundaria(est, posicaoInicio, metadeTamanho);
    mergeSortParaArquivoDeChaveSecundaria(est, metadeTamanho + 1, posicaoFim);

    i = posicaoInicio;
    j = metadeTamanho + 1;
    k = 0;
    est_temp = (index_autor *) malloc(sizeof(index_autor) * (posicaoFim - posicaoInicio + 1));

    while(i < metadeTamanho + 1 || j  < posicaoFim + 1) {
        if (i == metadeTamanho + 1 ) { 
            strcpy(est_temp[k].autor, est[j].autor);
            est_temp[k].p_list = est[j].p_list;
            j++;
            k++;
        }
        else {
            if (j == posicaoFim + 1) {
                //vetorTemp[k] = vetor[i];
                strcpy(est_temp[k].autor, est[i].autor);
            		est_temp[k].p_list = est[i].p_list;
                i++;
                k++;
            }
            else {
                if ((strcmp(est[i].autor, est[j].autor)) < 0) {
                    //vetorTemp[k] = vetor[i];
                    strcpy(est_temp[k].autor, est[i].autor);
            				est_temp[k].p_list = est[i].p_list;
                    i++;
                    k++;
                }
                else {
                    //vetorTemp[k] = vetor[j];
                    strcpy(est_temp[k].autor, est[j].autor);
            				est_temp[k].p_list = est[j].p_list;
                    j++;
                    k++;
                }
            }
        }
    }
    for(i = posicaoInicio; i <= posicaoFim; i++) {
        //vetor[i] = vetorTemp[i - posicaoInicio];
        strcpy(est[i].autor, est_temp[i - posicaoInicio].autor);
      	est[i].p_list = est_temp[i - posicaoInicio].p_list;
    }
    free(est_temp);
}


