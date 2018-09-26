//Desenvolvedores: Bruno Domene e Thiago Polycarpo

//bibliotecas
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

//defines
#define TAM_STRUCT 100

//structs

//arquivo biblioteca.bin
struct livro{
  char isbn[14];
  char titulo[50];
  char autor[50];
  char ano[5];
}arq_livros[TAM_STRUCT];

//arquivo busca_p.bin
struct buscaPrimaria{
	char isbn[14];
}busca_p[TAM_STRUCT];

//arquivo busca_s.bin
struct buscaSecundaria{
	char autor[50];
}busca_s[TAM_STRUCT];

//arquivo  arq_chave_p.bin
typedef struct tipo_index{
  char isbn[14];
  int posicao;
}index_isbn;
index_isbn est_isbn[TAM_STRUCT];

//arquivo arq_chave_s.bin
typedef struct tipo_index_autor{
  char autor[50];
  int  p_list;
}index_autor;
index_autor est_autor[TAM_STRUCT];

//arquivo lista_invertida.bin
typedef struct lista_l_invertida{
  char isbn[14];
  int  prox;
}invertida;
invertida l_invertida[TAM_STRUCT];

//variaveis globais para abertura de arquivo
char atualizar[] = "ab", leitura[] = "r+b", escrever[] = "wb";
int qtd_chave_s = 0;

//funcoes do arquivo
int criar_arquivo(char nome_arq[]);
int abrir_arquivo(FILE **p_arq, char nome_arq[], char tipo_abertura[]);
int carregar_arquivo(int resp);
int exibir_arquivo(int resp);
int pegar_registro(FILE **p_arq, char *p_reg);
void fechar_arquivo(FILE **p_arq);
void reconstruir_arquivo(char nome_arq[], int quantidade_registro, int cont);
//funcoes de buscas
void buscar_chave_primaria();
void buscar_chave_secundaria();
//funcoes de insercoes
int inserir(int tam_vet_inserir);
int inserir_lista_invertida(int contador_insercao, char isbn[], int posicao_do_anterior);
void inserir_arq_chave_primaria(int contador_insercao, int posicao_registro);
void inserir_arq_chave_secundaria(int contador_insercao, char autor[]);
//funcoes de merges
void merge_sort_chave_primaria(index_isbn *est, int posicao_inicio, int posicao_fim);
void merge_sort_chave_secundaria(index_autor *est, int posicao_inicio, int posicao_fim);

//main
int main(){
  int resp, sair = 0, tam_vet_inserir = 0;
	char arq_livros[]="livros.bin";
	do{ 
		system("cls");
	 	printf("\n        Menu");
	  	printf("\n1 - Insercao");
	  	printf("\n2 - Procurar por ISBN (indice primario)");
	  	printf("\n3 - Procurar por Autor (indice secundario)");
	  	printf("\n4 - Carregar Arquivos");
	  	printf("\n5 - Dump Arquivo ");
	  	printf("\n6 - Sair");
	  	printf("\nOpcao: ");
	  	scanf("%d",&resp);
	
		switch(resp){
			case 1:{
		    	inserir(tam_vet_inserir);
				break;
			}
		  	case 2:{
		  		buscar_chave_primaria();
				break;
			}
		  	case 3: {
	  			buscar_chave_secundaria();
	  			break;
	  		}
	  		case 4:{
		  		resp=0;
			    tam_vet_inserir = carregar_arquivo(resp); 
			    printf("\nArquivos Carregados\n");
			    system("pause");
				break;
			}
	  		case 5:{
		  		system("cls");
		  		do{
					printf("\n\nDigite qual arquivo deseja exibir: ");
					printf("\n1 - Principal");
					printf("\n2 - Indice Primario");
					printf("\n3 - Indice Secundario");
					printf("\n4 - Lista Invertida");
					printf("\nopcao:");
					scanf("%d",&resp);
				}while(resp < 1 || resp > 4);
			    exibir_arquivo(resp);
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

//cria arquivo e adiciona o byte int do contador e o byte offset quando necessário
int criar_arquivo(char nome_arq[]){
	int cont=0;
  	FILE *arq;
  
  	if((arq = fopen(nome_arq,"w+b")) == NULL){
	    printf("impossivel criar o arquivo");
	    system("pause");
	    return 0;
  	}
  	//escrevendo o cabeçalho do arquivo passado pelo parametro
  	if((!strcmp(nome_arq, "livros.bin")) || (!strcmp(nome_arq, "arq_chave_p.bin")) || (!strcmp(nome_arq, "arq_chave_s.bin"))){
	  	fwrite(&cont,sizeof(int),1, arq);
	}
	//reserva mais 4 bytes pro contador de controle de insercao
	if((!strcmp(nome_arq, "arq_chave_p.bin")) || (!strcmp(nome_arq, "arq_chave_s.bin"))){
	  	fwrite(&cont,sizeof(int),1, arq);
	}
  	fclose(arq);
  	return 1;
}

//carrega todos arquivos
int carregar_arquivo(int resp){
	char arq_cadastro[]="biblioteca.bin", arq_busca_p[]="busca_p.bin", arq_busca_s[]="busca_s.bin", abrir[] = "r+b";
  	char arq_primaria[]= "arq_chave_p.bin", lista_invert[]="lista_invertida.bin";
  	FILE *arq;
  	int i, tam_vet_inserir = 0, todos=0;
  
  	system("cls");
  	if(resp != 0){
  		todos = 1;
  	}
  	//abre arquivo biblioteca, carrega em vetor de struct 
  	if(resp == 1 || todos == 0){
		if(abrir_arquivo(&arq, arq_cadastro, abrir)){
	  		i=0;
	    	while(fread(&arq_livros[i], sizeof(livro), 1, arq)){
	      		i++;  
	    	}
	    fechar_arquivo(&arq);
	  	tam_vet_inserir = i;  
	  }
  	}
  	//abre arquivo busca_p.bin, carrega em vetor de struct 
  	if(resp == 2 || todos == 0){
		if(abrir_arquivo(&arq, arq_busca_p, abrir)){
		    i=0;
			while(fread(&busca_p[i], sizeof(struct buscaPrimaria), 1, arq)){
				i++;
			}
			fechar_arquivo(&arq);
		}
  	} 
  	//abre arquivo busca_s.bin, carrega em vetor de struct 
  	if(resp == 3 || todos == 0){
	  	if(abrir_arquivo(&arq, arq_busca_s, abrir)){
	    	i=0;
			while(fread(&busca_s[i], sizeof(struct buscaSecundaria), 1, arq)){
			  i++;
			}
			fechar_arquivo(&arq);
	  	}
  	}
  	//carrega os dados do arquivo de chave primaria(isbn) no vetor de struct 
  	if(resp == 2 || todos ==0){
	  	if((arq = fopen(arq_primaria, "r+b")) != NULL){
	  	 i = 0; 
			fseek(arq, 8, 0);
			while(fread(&est_isbn[i], sizeof(struct tipo_index) , 1, arq)){
				i++;
			}
			fclose(arq);
		}
	}	
	 //carrega os dados do arquivo de chave secundaria(autor) no vetor de struct 
	if(resp == 3 || todos == 0){
		if((arq = fopen("arq_chave_s.bin", "r+b")) != NULL){
	  	 	i = 0; 
			fseek(arq, 8, 0);
			while(fread(&est_autor[i], sizeof(index_autor) , 1, arq)){
				i++;
			}
			fclose(arq);
			qtd_chave_s = i;
		}
		//pegando os registros do arquivo de lista invertida
		if((arq = fopen("lista_invertida.bin", "r+b")) != NULL){
	  	int i = 0; 
			while(fread(&l_invertida[i], sizeof(invertida) , 1, arq)){
				i++;
			}
			fclose(arq);
		}
	}
  	return tam_vet_inserir;
}

//faz o dump do arquivo passado por parametro.
int exibir_arquivo(int resp){
	char *pch, registro[119], arq_nome[16]="\0", abrir[] = "r+b";
	FILE *arq;
	int cont_insercao,tam_reg,i;
	system("cls");
	
	if(resp == 1){
		strcpy(arq_nome,"livros.bin");
	}else if (resp == 2){
		strcpy(arq_nome,"arq_chave_p.bin");
	}else if (resp == 3){
		strcpy(arq_nome,"arq_chave_s.bin");
	}else{
		strcpy(arq_nome,"lista_invertida.bin");
	}
	abrir_arquivo(&arq, arq_nome, abrir);
	if(resp == 1){	
		fseek(arq, 4, 0);
		tam_reg = pegar_registro(&arq,registro);
		while (tam_reg > 0){
			pch = strtok(registro,"|");
				while (pch != NULL){
					//exibindo o hexadecimal de cada caractere
					for(int i = 0; i <  strlen(pch); i++)
						printf("%X ", pch[i]);
					printf("- %s\n",pch);
					pch = strtok(NULL,"|");
				}
			printf("\n");
			tam_reg = pegar_registro(&arq,registro);
		}
	}else if(resp == 2){
		i = 0; 
		fseek(arq, 8, 0);
		while(fread(&est_isbn[i], sizeof(struct tipo_index) , 1, arq)){
			printf("isbn: %s\n", est_isbn[i].isbn);
			printf("posicao: %d\n", est_isbn[i].posicao);
			i++;
		}
	}else if(resp == 3){
		i=0;
		fseek(arq, 8, 0);
		while(fread(&est_autor[i], sizeof(struct tipo_index_autor),1, arq)){
			printf("autor: %s\n", est_autor[i].autor);
			printf("posicao da lista: %d\n", est_autor[i].p_list);
			i++;
		}
	}else{
		i = 0; 
		while(fread(&l_invertida[i], sizeof(invertida) , 1, arq)){
			printf("autor: %s\n", l_invertida[i].isbn);
			printf("proximo: %d\n",l_invertida[i].prox);
			i++;
		}
	}
	
	printf("\n");
	system("pause");
	fclose(arq);
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

//faz a busca pela chave primaria
void buscar_chave_primaria(){
  	FILE *arq;
  	int resp=2, cont_chave_p, i, cont_livros, tam_reg, atualizado, achou = 0;
  	char abrir[] = "r+b", arquivo_livros[]= "livros.bin", arq_chave_p[]="arq_chave_p.bin";
  	char *pch, registro[119];
  
  	//carrega arquivos arq_chave_p e busca_p no vetor de struct
  	carregar_arquivo(resp);
  	
  	//abre arq_chave_p pra pegar o contador de pesquisa do busca_p
  	abrir_arquivo(&arq, arq_chave_p, abrir);
  	fread(&atualizado, sizeof(int),1,arq);
  	printf("Atualizado: %d\n", atualizado);
  	fread(&cont_chave_p, sizeof(int),1,arq);
  	printf("Contador: %d\n", cont_chave_p);
  	fclose(arq);
  	
  	//abre o arquivo de livros pra pesquisa
  	abrir_arquivo(&arq, arquivo_livros, abrir);
		fread(&cont_livros, sizeof(int),1,arq);
		
	//se o arqivo não está atualizado com o arq principal ele reconstroi
  	if(atualizado == 0){
  		reconstruir_arquivo(arq_chave_p, cont_livros, cont_chave_p);
	}
		
	printf("ISBN a ser buscado: %s\n", busca_p[cont_chave_p].isbn);
  	
  	i=0;
  	while(i < cont_livros && achou != 1){
  		if(strcmp(busca_p[cont_chave_p].isbn, est_isbn[i].isbn) == 0){
  			printf("\nISBN encontrado!!\n");
  			printf("Livro:\n");
  			achou = 1;
	  		fseek(arq,est_isbn[i].posicao,0);
	  		tam_reg = pegar_registro(&arq,registro);
				pch = strtok(registro,"|");
				while (pch != NULL){
					printf("- %s\n",pch);
					pch = strtok(NULL,"|");
				}	
	      break;	
  		}
  		i++;
  	}
  	
  	if(achou == 0){
  		printf("\nISBN %s nao encontrado!!\n", busca_p[cont_chave_p].isbn);
	}
		
  	fclose(arq);
  	//abre arq_chave_p pra atualizar o contador da pesquisa
  	if(abrir_arquivo(&arq, arq_chave_p, abrir)){
	  	fseek(arq,4,0);
	  	cont_chave_p++;
	  	fwrite(&cont_chave_p,sizeof(int),1,arq);
	  	fclose(arq);
  	}
  	system("pause");
}

//faz a busca pela chave secundaria
void buscar_chave_secundaria(){
	FILE *arq, *arq_l;
	int resp=3, cont_chave_s, i = 0, prox = 0, k, cont_livros, tam_reg, atualizado, compara = 1;
	char arquivo_livros[]= "livros.bin", arq_chave_s[]="arq_chave_s.bin", arq_l_invertida[]="lista_invertida.bin";
	char *pch, registro[119];
	invertida aux;

	//carrega arquivos arq_chave_s e busca_s e lista invertida no vetor de struct
	carregar_arquivo(resp);
	resp = 2;
	carregar_arquivo(resp);
	
	//abre arq_chave_s pra pegar o contador de pesquisa do arq_chave_s.bin
	abrir_arquivo(&arq, arq_chave_s, leitura);
	fread(&atualizado, sizeof(int),1,arq);
	printf("Atualizado: %d\n", atualizado);
	fread(&cont_chave_s, sizeof(int),1,arq);
	printf("Contador Secundario: %d\n", cont_chave_s);
	fclose(arq);
	
	//se o arqivo não está atualizado com o arq principal ele reconstroi
	if(atualizado == 0){
		reconstruir_arquivo(arq_chave_s, qtd_chave_s, cont_chave_s);
	}
	
	//abre o arquivo de livros pra pesquisa
	abrir_arquivo(&arq, arquivo_livros, leitura);
	fread(&cont_livros, sizeof(int),1,arq);
	printf("Autor a ser buscado: %s\n", busca_s[cont_chave_s].autor);
	
	while(i< qtd_chave_s && compara > 0){		
	
		compara = strcmp(busca_s[cont_chave_s].autor,est_autor[i].autor);
		if(compara == 0){		
			printf("\nAutor encontrado\n");
			printf("Os livros desse autor:\n");
			if(abrir_arquivo(&arq_l, arq_l_invertida, leitura)){
	  			fseek(arq_l, est_autor[i].p_list, 0);
	  			
	  			prox = 0;
	  			while(prox != -1){
	  				fread(&aux, sizeof(invertida), 1, arq_l);
	  				printf("\nLivro:\n");
	  				
					k=0;
	  				while(k < cont_livros){
	  					if(strcmp(aux.isbn, est_isbn[k].isbn) == 0){
		  					fseek(arq, est_isbn[k].posicao, 0);
					  		tam_reg = pegar_registro(&arq, registro);
								pch = strtok(registro,"|");
								while (pch != NULL){
									printf("- %s\n",pch);
									pch = strtok(NULL,"|");
								}
							}
	  					k++;
	  				}
	  				
	  				prox = aux.prox;
						  				
	  				if(prox != -1){
	  					fseek(arq_l, prox, 0);
					}
	  			}
  				fclose(arq_l);
			}
		}
		i++;
	}
	
	if(compara != 0){
		printf("\nAutor %s nao encontrado\n", busca_s[cont_chave_s].autor);
	}
	
	fclose(arq);
	//abre arq_chave_p pra atualizar o contador da pesquisa
	if(abrir_arquivo(&arq, arq_chave_s, leitura)){
  	fseek(arq,4,0);
  	cont_chave_s++;
  	fwrite(&cont_chave_s,sizeof(int),1,arq);
  	fclose(arq);
	}
	
	system("pause");
}

//inserir
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
	
		inserir_arq_chave_primaria(cont_registro, posicao_registro);
		inserir_arq_chave_secundaria(cont_registro, arq_livros[cont_registro].autor);
		
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
  	return 1;
} 

//insere no arquivo de chave primaria
void inserir_arq_chave_primaria(int contador_insercao, int posicao_registro){
	char nome_arq[] = "arq_chave_p.bin", atualizar[] = "ab", leitura[] = "r+b", escrever[] = "wb"; 
	FILE *arq;
	int i, atualizado, cont_chave_p;
	
	if((fopen(nome_arq, "r+b")) == NULL){
		criar_arquivo(nome_arq);
	}
	
	//abrindo para leitura do flag de atualização
	abrir_arquivo(&arq, nome_arq, leitura);
	fread(&atualizado, sizeof(int), 1, arq);
	fread(&cont_chave_p, sizeof(int), 1, arq);
	
	//copiando o registro para a estrutura da chave primaria
	est_isbn[contador_insercao].posicao = posicao_registro;
	strcpy(est_isbn[contador_insercao].isbn, arq_livros[contador_insercao].isbn);
	//alterando flag de atualização pois copia do arquivo em memoria foi modificada
	atualizado = 0;
	fseek(arq,0,0);
	fwrite(&atualizado, sizeof(int), 1, arq);
	fclose(arq);
	
	//ordenando registros
	merge_sort_chave_primaria(est_isbn, 0, contador_insercao);
	
	reconstruir_arquivo(nome_arq, contador_insercao + 1, cont_chave_p);
}

//insere no arquivo de chave secundaria
void inserir_arq_chave_secundaria(int contador_insercao, char autor[]){
	char nome_arq[] = "arq_chave_s.bin", escrever[]= "wb";
	FILE *arq;
	int i = 0, atualizado,cont_chave_s, achou = 0;
		
	if((fopen(nome_arq, "r+b")) == NULL){
		criar_arquivo(nome_arq);
	}
	
	//abrindo para leitura do flag de atualização
	abrir_arquivo(&arq, nome_arq, leitura);
	fread(&atualizado, sizeof(int), 1, arq);
	fread(&cont_chave_s, sizeof(int), 1, arq);
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
		abrir_arquivo(&arq, nome_arq, escrever);
		atualizado = 0;
		fseek(arq,0,0);
		fwrite(&atualizado, sizeof(int), 1, arq);
		fclose(arq);
		qtd_chave_s++;
	}
	
	//ordenando
	merge_sort_chave_secundaria(est_autor, 0, qtd_chave_s - 1);
	
	reconstruir_arquivo(nome_arq, qtd_chave_s, cont_chave_s);
}

/* escreve registro na lista invertida e devolve posição dele no arquivo, se o parametro "posicao_do_anterior" for maior ou 
igual a zero sifnifica q o nome passado já exixte no arquivo de chave secundaria assim, o atributo proximo receba a 
posicao do registro  de mesmo nome anterior a ele (está incrementando na lista encadeado) */
int inserir_lista_invertida(int contador_insercao, char isbn[], int posicao_do_anterior){
	char nome_arq[] = "lista_invertida.bin";
	FILE *arq;
	invertida est;
	int posicao, cont_chave_s;
		
	if((fopen(nome_arq, "r+b")) == NULL){
		criar_arquivo(nome_arq);
	}
	
	//copiando dados para o vetor de estrutura da lista invertida
	strcpy(l_invertida[contador_insercao].isbn, isbn); 
	
	l_invertida[contador_insercao].prox = posicao_do_anterior;
	
	//escrevendo na lista invertida
	abrir_arquivo(&arq, nome_arq, atualizar);
	fwrite(&l_invertida[contador_insercao], sizeof(invertida), 1, arq);
	//fread(&cont_chave_s, sizeof(int), 1, arq);
	posicao = ftell(arq) - sizeof(invertida);					//pegando posicao do registro no arquivo
	fclose(arq);
	
	return posicao;
}

//ordenar arquivo de chave primaria(isbn)
void merge_sort_chave_primaria(index_isbn *est, int posicao_inicio, int posicao_fim){
    int i, j, k, metade_tamanho;
    if(posicao_inicio == posicao_fim) return;
    metade_tamanho = (posicao_inicio + posicao_fim ) / 2;
    index_isbn *est_temp;
    
    merge_sort_chave_primaria(est, posicao_inicio, metade_tamanho);
    merge_sort_chave_primaria(est, metade_tamanho + 1, posicao_fim);

    i = posicao_inicio;
    j = metade_tamanho + 1;
    k = 0;
    est_temp = (index_isbn *) malloc(sizeof(index_isbn) * (posicao_fim - posicao_inicio + 1));

    while(i < metade_tamanho + 1 || j  < posicao_fim + 1) {
        if (i == metade_tamanho + 1 ) { 
            strcpy(est_temp[k].isbn, est[j].isbn);
            est_temp[k].posicao = est[j].posicao;
            j++;
            k++;
        }
        else {
            if (j == posicao_fim + 1) {
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
    for(i = posicao_inicio; i <= posicao_fim; i++) {
        //vetor[i] = vetorTemp[i - posicao_inicio];
        strcpy(est[i].isbn, est_temp[i - posicao_inicio].isbn);
      	est[i].posicao = est_temp[i - posicao_inicio].posicao;
    }
    free(est_temp);
}

//ordenar arquivo de chave secundaria(autor)
void merge_sort_chave_secundaria(index_autor *est, int posicao_inicio, int posicao_fim){
    int i, j, k, metade_tamanho;
    if(posicao_inicio == posicao_fim) return;
    metade_tamanho = (posicao_inicio + posicao_fim ) / 2;
    index_autor *est_temp;
    
    merge_sort_chave_secundaria(est, posicao_inicio, metade_tamanho);
    merge_sort_chave_secundaria(est, metade_tamanho + 1, posicao_fim);

    i = posicao_inicio;
    j = metade_tamanho + 1;
    k = 0;
    est_temp = (index_autor *) malloc(sizeof(index_autor) * (posicao_fim - posicao_inicio + 1));

    while(i < metade_tamanho + 1 || j  < posicao_fim + 1) {
        if (i == metade_tamanho + 1 ) { 
            strcpy(est_temp[k].autor, est[j].autor);
            est_temp[k].p_list = est[j].p_list;
            j++;
            k++;
        }
        else {
            if (j == posicao_fim + 1) {
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
    for(i = posicao_inicio; i <= posicao_fim; i++) {
        //vetor[i] = vetorTemp[i - posicao_inicio];
        strcpy(est[i].autor, est_temp[i - posicao_inicio].autor);
      	est[i].p_list = est_temp[i - posicao_inicio].p_list;
    }
    free(est_temp);
}

//reconstroe casa programa seja interrompido 
void reconstruir_arquivo(char nome_arq[], int quantidade_registro, int cont){
	int atualizado;
	FILE *arq;
	int i;
	
	//reescrevendo arquivo
	abrir_arquivo(&arq, nome_arq, escrever);
	//Colando flag de atualização em 1, pois arquivo foi reescrito
	atualizado = 1;
	fwrite(&atualizado, sizeof(int), 1, arq);
	fwrite(&cont, sizeof(int), 1, arq);
	for(i = 0; i < quantidade_registro; i++){
		if(!(strcmp(nome_arq, "arq_chave_p.bin")))
			fwrite(&est_isbn[i], sizeof(tipo_index), 1, arq);
		else if(!(strcmp(nome_arq, "arq_chave_s.bin")))
			fwrite(&est_autor[i], sizeof(index_autor), 1, arq);	
	}
	fclose(arq);
}


