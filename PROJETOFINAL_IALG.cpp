/* Projeto realizado por Tobias Maugus Bueno Cougo e JoC#o Gabriel Salomao Baldim TURMA 10A
 * Tema: RPG
 * Campos do arquivo .csv: #int ID; string nome; int idade; int nivel; string classe
 * */

#include <fstream>
#include <iostream>
#include <locale.h>
#include <sstream>
#include <climits>
#include <cstring>

using namespace std;


/*Funcao que pega o registro vazio e passa o registro que o sucede para a sua posicao*/
template<typename TipoJogador>
TipoJogador* removeRegistro(TipoJogador* vetor, int posicoesPreenchidas, int tam) {
	for(int k=posicoesPreenchidas; k<tam-1; k++) {
		vetor[k]=vetor[k+1];
	}
	return vetor;
}

template<typename TipoJogador>
void exportarCSV(TipoJogador* vetor, int tam, string nomeArquivo) {
	/*Funcao que utiliza da escrita de dados "ofstream" para escrever as alteracoes
	em um arquivo .csv*/
	ofstream arquivo(nomeArquivo);
	arquivo << "ID,\"NOME\",IDADE,NIVEL,\"CLASSE\"\n";
	for (int posicoesPreenchidas = 0; posicoesPreenchidas < tam; ++posicoesPreenchidas) {
		if (vetor[posicoesPreenchidas].id != INT_MAX) {
			arquivo << vetor[posicoesPreenchidas].id << ",\""
			        << vetor[posicoesPreenchidas].nome << "\","
			        << vetor[posicoesPreenchidas].idade << ","
			        << vetor[posicoesPreenchidas].nivel << ",\""
			        << vetor[posicoesPreenchidas].classe << "\"\n";
		}
	}

	arquivo.close();
}

template<typename TipoJogador>
void exportarBinario(TipoJogador* vet, const string& nomeArquivo, int tam) {
	struct structAUX {
		int id;
		char nome[40];
		int idade;
		int nivel;
		char classe[40];
	};
	structAUX* vetAUX = new structAUX[tam];

// Copia de dados entre os vetores
	for (int i = 0; i < tam; i++) {
		vetAUX[i].id = vet[i].id;
		strncpy(vetAUX[i].nome, vet[i].nome.c_str(), sizeof(vetAUX[i].nome) - 1);
		vetAUX[i].idade = vet[i].idade;
		vetAUX[i].nivel = vet[i].nivel;
		strncpy(vetAUX[i].classe, vet[i].classe.c_str(), sizeof(vetAUX[i].classe) - 1);
	}

	ofstream arquivo(nomeArquivo, ios::binary);
	if (arquivo) {
		//escrita no arquivo .dat
		arquivo.write((const char*)(vetAUX), sizeof(structAUX) * tam);
		arquivo.close();
		cout << "arquivo "<<nomeArquivo<<" lido com sucesso."<<endl;
	} else {
		cout<<"Erro ao ler o arquivo!!"<<endl;
	}

	delete[] vetAUX;
}


template<typename TipoJogador>
TipoJogador* importarBIN(const string& nomeArq, int& tam, int&posicoesPreenchidas) {
	struct structAUX {
		int id;
		char nome[40];
		int idade;
		int nivel;
		char classe[40];
	};
	posicoesPreenchidas=0;
	int tamanhoinicial=40;
	ifstream arquivo(nomeArq, ios::binary);

	if(arquivo) {
		arquivo.seekg(0, ios::end);
		streampos arquivoTAM = arquivo.tellg();
		arquivo.seekg(0, ios::beg);

		tam = arquivoTAM / sizeof(structAUX);

		//aloca a memoria do vetor auxiliar e calcula o tamanho do arquivo previamente
		structAUX* vetAUX = new structAUX[tam];
		arquivo.read(reinterpret_cast<char*>(vetAUX), sizeof(structAUX) * tam);
		bool controle=true;
		TipoJogador* vet = new TipoJogador[tam];


		while(posicoesPreenchidas <= tam and controle==true) {
			if(posicoesPreenchidas>=tamanhoinicial) {
				//redimensionando
				vet=redimensionaVetor(vet, tamanhoinicial, 10);
				tamanhoinicial = tamanhoinicial+10;
			}
			if(string(vetAUX[posicoesPreenchidas].nome)=="ZZZ POSICAO VAZIA ZZZ") {
				controle=false;
			}
			//adiciona as posicoes do vetor Auxiliar no vetor Principal
			vet=adicionaRegistro(vet, posicoesPreenchidas, vetAUX[posicoesPreenchidas].id,
			                     string(vetAUX[posicoesPreenchidas].nome),
			                     vetAUX[posicoesPreenchidas].idade,
			                     vetAUX[posicoesPreenchidas].nivel,
			                     string(vetAUX[posicoesPreenchidas].classe));
			posicoesPreenchidas++;
		}
		//inativa posicoes que nao serao preenchidas
		for(int k=posicoesPreenchidas; k<tamanhoinicial; k++) {
			vet=InativarPosicao(vet, k);
		}
		delete[] vetAUX;

		arquivo.close();
		tam=tamanhoinicial;
		return vet;
	} else {
		return nullptr;
		tam=0;
		posicoesPreenchidas=0;
	}
}

template<typename TipoJogador>
TipoJogador* InativarPosicao(TipoJogador* vetor, int posicoesPreenchidas) {
	vetor[posicoesPreenchidas].id=INT_MAX;
	vetor[posicoesPreenchidas].nome="ZZZ POSICAO VAZIA ZZZ";
	vetor[posicoesPreenchidas].idade=0;
	vetor[posicoesPreenchidas].nivel=0;
	vetor[posicoesPreenchidas].classe="VAZIO";
	/*Funcao que caso chamada, inativa uma posicao do vetor, alterando assim
	todos os valores naquela posicao para um valor nulo impossivel de ser lido por qualquer
	outra funcao no codigo.*/
	return vetor;
}

template<typename TipoJogador>
TipoJogador* ordenarId(TipoJogador* vetor, int tam) {
	/*Essa funcao utiliza do metodo de ordenacao de vetor "Shell sort" com o Id dos jogadores,
	 o gap utilizado sempre eh divido por 2 assim acelerando o processo de ordenacao*/
	for (int gap = tam/2; gap > 0; gap /= 2) {
		/*Funcao for para a reducao do Gap*/
		for (int posicoesPreenchidas = gap; posicoesPreenchidas < tam; posicoesPreenchidas += 1) {
			TipoJogador AUX;
			AUX = vetor[posicoesPreenchidas];
			int j;

			for (j = posicoesPreenchidas; j >= gap && vetor[j - gap].id > AUX.id; j -= gap) {
				vetor[j] = vetor[j - gap];
			}

			vetor[j] = AUX;
		}
	}
	return vetor;
}

template<typename TipoJogador>
int buscaBinariaId(TipoJogador* vetor, int idBuscado, int posInicial, int posFinal) {
	/*Essa funcao utiliza a busca binaria para achar qual a posicao de um Id*/
	vetor=ordenarId(vetor, posFinal);
	/*Definicao do Pivo como o valor medio do vetor*/
	while (posInicial <= posFinal) {
		int meio = (posInicial + posFinal)/2;
		if (idBuscado == vetor[meio].id) {
			return meio;
		} else {
			if (idBuscado > vetor[meio].id) {
				posInicial = meio + 1;
			}
			else {
				posFinal = meio - 1;
			}
		}
	}
	return -1;
}

template<typename TipoJogador>
TipoJogador* ordenarNome(TipoJogador* vetor, int tam) {
	/*Essa funcao, assim como a de Ordenar por ID,
	  utiliza do metodo de ordenacao de vetor "Shell sort" com o nome dos jogadores,
	o gap utilizado sempre eh divido por 2 assim acelerando o processo de ordenacao*/
	for (int gap = tam/2; gap > 0; gap /= 2) {
		/*Funcao " for "para a reducao do Gap*/
		for (int posicoesPreenchidas = gap; posicoesPreenchidas < tam; posicoesPreenchidas += 1) {
			TipoJogador AUX;
			AUX = vetor[posicoesPreenchidas];
			int j;

			for (j = posicoesPreenchidas; j >= gap && vetor[j - gap].nome > AUX.nome; j -= gap) {
				vetor[j] = vetor[j - gap];
			}

			vetor[j] = AUX;
		}
	}
	return vetor;
}
template<typename TipoJogador>
int buscaBinariaNome(TipoJogador* vetor, string nomeBuscado, int posInicial, int posFinal) {
	/*Essa funcao assim como a de ID utiliza a busca binaria para achar qual a posicao de um nome*/
	vetor=ordenarNome(vetor, posFinal);
	while (posInicial <= posFinal) {
		int meio = (posInicial + posFinal)/2;
		/*utiliza tambem do valor medio para a funcao de pivo*/
		if (nomeBuscado == vetor[meio].nome) {
			return meio;
		} else {
			if (nomeBuscado > vetor[meio].nome) {
				posInicial = meio + 1;
			}
			else {
				posFinal = meio - 1;
			}
		}
	}
	return -1;
}


template<typename TipoJogador>
TipoJogador* adicionaRegistro(TipoJogador* vetor, int posicoesPreenchidas, int id, string nome, int idade, int nivel, string classe) {
	/*Funcao que adiciona registros na structure principal*/
	vetor[posicoesPreenchidas].id=id;
	vetor[posicoesPreenchidas].nome=nome;
	vetor[posicoesPreenchidas].idade=idade;
	vetor[posicoesPreenchidas].nivel=nivel;
	vetor[posicoesPreenchidas].classe=classe;
	return vetor;
}

template<typename TipoJogador>
TipoJogador* redimensionaVetor(TipoJogador* vetor, int tam, int incremento) {
	/*Funcao que diminui ou aumenta o vetor baseado no valor da variavel incremento*/
	TipoJogador* novoVetor = new TipoJogador[tam+incremento];
	for (int posicoesPreenchidas = 0; posicoesPreenchidas < tam; posicoesPreenchidas++) {
		novoVetor[posicoesPreenchidas] = vetor[posicoesPreenchidas];
	}
	for (int posicoesPreenchidas = tam; posicoesPreenchidas < tam+incremento; posicoesPreenchidas++) {
		novoVetor=InativarPosicao(novoVetor, posicoesPreenchidas);
		/*Transforma em valores nulos caso o incremento seja negativo, e
		cria novos espacos nulos se o incremento for positivo*/
	}
	delete[] vetor;
	return novoVetor;
}


template<typename TipoJogador>
void printVetor(TipoJogador* vetor, int posInicial, int posFinal) {
	cout<<"ID - NOME - IDADE - NIVEL - CLASSE"<<endl;
	/*Funcao que printa a parte desejada do vetor*/
	if(posInicial==posFinal) {
		/*caso seja necessario printar somente um registro*/
		cout<<vetor[posInicial].id<<" - "<<vetor[posInicial].nome<<" - "<<vetor[posInicial].idade<<" - "<<vetor[posInicial].nivel<<" - "<<vetor[posInicial].classe<<endl;
	} else {
		for(int posicoesPreenchidas=posInicial; posicoesPreenchidas<posFinal; posicoesPreenchidas++) {
			cout<<vetor[posicoesPreenchidas].id<<" - "<<vetor[posicoesPreenchidas].nome<<" - "<<vetor[posicoesPreenchidas].idade<<" - "<<vetor[posicoesPreenchidas].nivel<<" - "<<vetor[posicoesPreenchidas].classe<<endl;
		}
	}
}

void telaOpcoes() {
	/*Menu que demosntra todas as opcoes e consequentemente suas acoes
	(note que essa funcao nao recebe nenhum valor do usuario.) */
	cout<<"-------------------------------------------------------"<<endl;
	cout<<"|1 - ESCOLHER ARQUIVO A SER LIDO                      |"<<endl;
	cout<<"|2 - LISTAR REGISTROS                                 |"<<endl;
	cout<<"|3 - ADICIONAR REGISTRO                               |"<<endl;
	cout<<"|4 - EDITAR REGISTRO                                  |"<<endl;
	cout<<"|5 - REMOVER REGISTRO                                 |"<<endl;
	cout<<"|6 - BUSCAR REGISTRO POR ID                           |"<<endl;
	cout<<"|7 - BUSCAR REGISTRO POR NOME                         |"<<endl;
	cout<<"|8 - EXPORTAR OS REGISTROS EM ARQUIVO                 |"<<endl;
	cout<<"-------------------------------------------------------"<<endl;
	cout<<"|9 - SAIR DO PROGRAMA                                 |"<<endl;
	cout<<"-------------------------------------------------------"<<endl;
	cout<<"Digite o numero da acao que deseja realizar: ";
}


int main()
{
	/*Funcao principal do codigo, onde todas as interacoes com ele vao acontecer*/
	setlocale(LC_ALL, "portuguese");
	/*Definicao da struct principal*/
	typedef struct {
		int id;
		string nome;
		int idade;
		int nivel;
		string classe;
	} Jogador;
	Jogador *Jogadores;
	/*declaracao da leitura e escrita no arquivo selecionado*/
	string nomeArquivo;
	ifstream arquivo2(nomeArquivo);
	/*Declaracao de outras variaveis utilizadas*/
	int posicoesPreenchidas = 0;
	int tam = 40;
	int incremento = 10;
	Jogadores = new Jogador[tam];
	string idAUX="";
	string nomeAUX="";
	string idadeAUX="";
	string nivelAUX="";
	string classeAUX="";
	string linha;


	int opcao=0, opcao2=0, CASEid, CASEidade, CASEnivel, CASEposFinal, CASEposInicial;
	string CASEnome, CASEclasse, arquivoAUX;
	do {
		system("clear");
		telaOpcoes();
		cin>>opcao;
		switch(opcao) {
		case 1:
			cout<<"1-ler csv"<<endl;
			cout<<"2-ler binario"<<endl;
			cout<<"Digite sua opcao: "<<endl;
			cin>>opcao2;
			if(opcao2==1) {
				cout<<"Digite o nome do arquivo que sera lido(formato deve ser csv e deve possuir um cabecalho de uma linha): ";
				cin>>nomeArquivo;
				arquivo2.open(nomeArquivo);


				if (arquivo2) {
					string linha2;
					std::getline(arquivo2, linha2);  // Descartar a primeira linha
					arquivo2.tellg();

					posicoesPreenchidas = 0;
					tam = 40;
					delete [] Jogadores;
					Jogadores = nullptr;
					Jogadores = new Jogador[tam];


					while(getline(arquivo2, linha2)) {
						if (linha2.empty()) {
							continue;
						}
						stringstream ss2(linha2);
						getline(ss2,idAUX,';');
						getline(ss2,nomeAUX,';');
						getline(ss2,idadeAUX,';');
						getline(ss2,nivelAUX,';');
						getline(ss2,classeAUX);

						nomeAUX.erase(0,1);
						nomeAUX.pop_back();
						classeAUX.erase(0,1);
						classeAUX.pop_back();

						if(posicoesPreenchidas>=tam) {
							Jogadores=redimensionaVetor(Jogadores, tam, incremento);
							tam = tam+incremento;
						}
						Jogadores=adicionaRegistro(Jogadores, posicoesPreenchidas, stoi(idAUX), nomeAUX, stoi(idadeAUX), stoi(nivelAUX), classeAUX);

						posicoesPreenchidas++;
					}
					cout<<"Arquivo "<< nomeArquivo <<" lido com sucesso!!"<<endl;
					arquivo2.close();
				} else {
					cout<<"Arquivo "<<nomeArquivo<<" nao pode ser aberto ";
				}
			} else if(opcao2==2) {
				tam=40;
				posicoesPreenchidas=0;
				cout<<"Digite o nome do arquivo a ser lido: "<<endl;
				cin>>nomeAUX;
				Jogadores=importarBIN<Jogador>(nomeAUX, tam, posicoesPreenchidas);
			} else {
				cout<<"opcao invalida!!!!"<<endl;
			}
			break;

		case 2:
			/*Caso seja selecionado mostrar registros do vetor*/
			cout<<"----------------------------------------------------"<< endl;
			cout<<"1-Mostrar registros ordenados por ID"<<endl;
			cout<<"2-Mostrar registros ordenados por NOME"<<endl;
			cout<<"----------------------------------------------------"<< endl;
			cin>>opcao2;

			if(opcao2==1) {
				system("clear");
				cout<<"----------------------------------------------------"<< endl;
				cout<<"1-Mostrar apenas um registro"<<endl;
				cout<<"2-Mostrar uma parte dos registros"<<endl;
				cout<<"3-Mostrar todos os registros"<<endl;
				cout<<"----------------------------------------------------"<< endl;
				/*recebe o intervalo e o direciona para as funcoes "buscaBinaria" e print vetor*/
				cin>>opcao2;
				if(opcao2==1) {
					cout<<"----------------------------------------------------"<< endl;
					cout<<"Digite o id do registro a ser mostrado(inteiro): ";
					cin>>CASEid;
					CASEposInicial=buscaBinariaId(Jogadores, CASEid, 0, tam);
					CASEposFinal=CASEposInicial;
					cout<<endl;
					printVetor(Jogadores, CASEposInicial, CASEposFinal);
				} else if(opcao2==2) {
					Jogadores=ordenarId(Jogadores, tam);
					cout<<"Digite o id do primeiro registro a ser mostrado (inteiro): ";
					cin>>CASEid;
					CASEposInicial=buscaBinariaId(Jogadores, CASEid, 0, tam);
					cout<<"Digite o id do ulltimo registro a ser mostrado(inteiro): ";
					cin>>CASEid;
					CASEposFinal=buscaBinariaId(Jogadores, CASEid, 0, tam);
					if(CASEposFinal<tam) {
						CASEposFinal++;
					}
					if(CASEposFinal!=-1 and CASEposInicial!=-1) {
						cout<<endl;
						printVetor(Jogadores, CASEposInicial, CASEposFinal);
					} else {
						cout<<"id invalido!"<<endl;
					}
				} else if(opcao2==3) {
					Jogadores=ordenarId(Jogadores, tam);
					cout<<endl;
					printVetor(Jogadores, 0, tam);
				} else {
					cout<<"Opcao invalida!"<<endl;
				}
			} else if(opcao2==2) {
				Jogadores = ordenarNome(Jogadores, tam);
				cout<<endl;
				printVetor(Jogadores, 0, tam);
			} else {
				cout<<"Opcao invalida!";
			}
			break;

		case 3:
			/*Caso a opcao de inserir um registro seja selecionado,
			serC#o colocadas em um espaco disponnivel as novas informacoes,
			todavia se esse espaco nao existir, um novo sera criado
			com a funcao redimensionaVetor*/
			cout<<"----------------------------------------------------"<< endl;
			cout<<"Digite o id do registro que sera inserido: "<<endl;
			cout<<"----------------------------------------------------"<< endl;
			cin>>CASEid;
			cin.ignore();
			cout<<"----------------------------------------------------"<< endl;
			cout<<endl<<"Digite o nome do registro que sera inserido: "<<endl;
			cout<<"----------------------------------------------------"<< endl;
			getline(cin, CASEnome);
			cout<<"----------------------------------------------------"<< endl;
			cout<<endl<<"Digite a idade do registro que sera inserido: "<<endl;
			cout<<"----------------------------------------------------"<< endl;
			cin>>CASEidade;
			cout<<"----------------------------------------------------"<< endl;
			cout<<endl<<"Digite o nivel do registro que sera inserido: "<<endl;
			cout<<"----------------------------------------------------"<< endl;
			cin>>CASEnivel;
			cin.ignore();
			cout<<"----------------------------------------------------"<< endl;
			cout<<endl<<"Digite a classe do registro que sera inserido: "<<endl;
			cout<<"----------------------------------------------------"<< endl;
			getline(cin, CASEclasse);
			if(posicoesPreenchidas>=tam) {
				Jogadores=redimensionaVetor(Jogadores, tam, incremento);
				tam = tam+incremento;
			}
			Jogadores=adicionaRegistro(Jogadores, posicoesPreenchidas, CASEid, CASEnome, CASEidade, CASEnivel, CASEclasse);
			posicoesPreenchidas++;
			cout<<endl<<"Registro inserido com sucesso!";
			break;

		case 4:
			/*Caso seja escolhida a opcao de editar um registro,
			 o usuario deve informar qual o ID desejado, e assim editar todos os valores da structure*/
			cout<<"----------------------------------------------------"<< endl;
			cout<<"Digite o id do registro que sera modificado: "<<endl;
			cout<<"----------------------------------------------------"<< endl;
			cin>>CASEid;
			CASEposInicial=buscaBinariaId(Jogadores, CASEid, 0, tam);
			cout<<"----------------------------------------------------"<< endl;
			cout<<"Digite o id novo: "<<endl;
			cin>>CASEid;
			cin.ignore();
			cout<<"----------------------------------------------------"<< endl;
			cout<<endl<<"Digite o nome novo: "<<endl;
			getline(cin, CASEnome);
			cout<<"----------------------------------------------------"<< endl;
			cout<<endl<<"Digite a idade nova: "<<endl;
			cin>>CASEidade;
			cout<<"----------------------------------------------------"<< endl;
			cout<<endl<<"Digite o nivel novo: "<<endl;
			cin>>CASEnivel;
			cout<<"----------------------------------------------------"<< endl;
			cin.ignore();
			cout<<endl<<"Digite a classe nova: "<<endl;
			getline(cin, CASEclasse);
			cout<<"----------------------------------------------------"<< endl
			    /*Adiciona o Registro editado no vetor*/;
			Jogadores=adicionaRegistro(Jogadores, CASEposInicial, CASEid, CASEnome, CASEidade, CASEnivel, CASEclasse);
			cout<<endl<<"Registro modificado com sucesso!";
			break;

		case 5:
			/* Caso seja escolhida a opcao de remover um registro,
			 primeiramente eh dada ao usuC!rio a escolha entre buscar o registro
			 a ser deletado via duas opcoses: (ID/NOME), assim com essas informacoeses
			 a funcao de busca da respectiva opco eh chamada e retorna sua posicoes no arquivo
			 permitindo assim a funcao removeRegistro remove-la utilizando essa informacao*/
			cout<<"----------------------------------------------------"<< endl;
			cout<<"1-Remover por ID"<<endl;
			cout<<"2-Remover por NOME"<<endl;
			cout<<"----------------------------------------------------"<< endl;
			cin>>opcao2;
			if(opcao2==1) {
				cout<<"----------------------------------------------------"<< endl;
				cout<<"Digite o id a ser removido: ";
				cin>>CASEid;
				CASEposInicial=buscaBinariaId(Jogadores, CASEid, 0, tam);
				Jogadores=removeRegistro(Jogadores, CASEposInicial, tam);
				Jogadores = InativarPosicao(Jogadores, posicoesPreenchidas-1);
				posicoesPreenchidas--;
			} else if(opcao2==2) {
				cout<<"----------------------------------------------------"<< endl;
				cout<<"Digite o nome a ser removido: ";
				cin>>CASEnome;
				CASEposInicial=buscaBinariaNome(Jogadores, CASEnome, 0, tam);
				Jogadores=removeRegistro(Jogadores, CASEposInicial, tam);
				Jogadores = InativarPosicao(Jogadores, posicoesPreenchidas-1);
				posicoesPreenchidas--;
			} else {
				cout<<"opcao invalida!";
			}
			if(tam-posicoesPreenchidas>=incremento) {
				Jogadores=redimensionaVetor(Jogadores, tam, tam-incremento);
				tam-=incremento;
			}
			break;

		case 6:
			/*A busca por ID utiliza a funcao de busca binaria respectiva,
			 para mostrar o registro desejado*/
			cout<<"----------------------------------------------------"<< endl;
			cout<<"Digite o id a ser buscado: "<<endl;
			cin>>CASEid;
			CASEposInicial=buscaBinariaId(Jogadores, CASEid, 0, tam);
			printVetor(Jogadores, CASEposInicial, CASEposInicial);
			break;

		case 7:
			/*A busca por nome utiliza a funcao de busca binaria respectiva,
			 para mostrar o registro desejado*/
			cout<<"----------------------------------------------------"<< endl;
			cout<<"Digite o nome a ser buscado: "<<endl;
			cin>>CASEnome;
			CASEposInicial=buscaBinariaNome(Jogadores, CASEnome, 0, tam);
			printVetor(Jogadores, CASEposInicial, CASEposInicial);
			break;

		case 8:
			cout<<"1-exportar em CSV"<<endl;
			cout<<"2-exportar em Binario"<<endl;
			cin>>opcao2;
			cout<<"Digite o nome do arquivo a ser exportado com a extensao correspondente: ";
			cin>>arquivoAUX;
			if(opcao2==1) {
				exportarCSV(Jogadores, tam, arquivoAUX);
			} else {
				exportarBinario(Jogadores, arquivoAUX, tam);
			}
			cout<<"ARQUIVO EXPORTADO COM SUCESSO!"<<endl;
			break;

		case 9:
			/*essa funcao quebra o loop necessario para que a
			 tela de opcoes apareca depois de uma acao ser concluida
			e pergunta ao usurio se o mesmo deseja salvar as alteracoes*/
			cout<<"----------------------------------------------------"<< endl;
			cout<<"DESEJA SALVAR AS ALTERACOES?:"<<endl;
			cout<<"1 - SIM"<<endl;
			cout<<"2 - NAO"<<endl;
			cout<<"----------------------------------------------------"<< endl;
			cin >> opcao2;
			if (opcao2 == 1) {
				int opcao3;
				cout<<"1-exportar em CSV"<<endl;
				cout<<"2-exportar em Binario"<<endl;
				cin>>opcao3;
				cout<<"Digite o nome do arquivo a ser exportado com a extensao correspondente: ";
				cin>>arquivoAUX;
				if(opcao3==1) {
					exportarCSV(Jogadores, tam, arquivoAUX);
				} else {
					exportarBinario(Jogadores, arquivoAUX, tam);
				}
				cout<<"ARQUIVO EXPORTADO COM SUCESSO!"<<endl;
				cout<<endl<<"--FIM DO PROGRAMA--"<<endl;
				break;
			} else {
				cout<<endl<<"--FIM DO PROGRAMA--"<<endl;
				break;
			}

		default:
			cout<<"OPCAO INVALIDA!";
		}
		cout<<"\nTecle ENTER para continuar...";
		cin.ignore().get();
	} while(opcao!=9);


	delete [] Jogadores;
	return 0;
}

